// C++
#include <iostream>
#include <fstream>
#include <filesystem>

// my classes
#include "GetSlice.h"
#include "RecistROI.h"
#include "FFTCompatable.h"
#include "ImageTypes.h"
#include "HannWindow.h"
#include "FFTShift.h"
#include "BandPassFilter.h"
#include "LowPassFilter.h"
#include "ImageEnergy.h"
#include "ParseMTF.h"

// ITK
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkCastImageFilter.h>
#include <itkForwardFFTImageFilter.h>
#include <itkImageRegionIterator.h>
#include <itkImageDuplicator.h>
#include <itkInverseFFTImageFilter.h>
#include <itkMultiplyImageFilter.h>
#include <itkComplexToModulusImageFilter.h>
#include <itkAddImageFilter.h>
#include <itkLogImageFilter.h>
#include <itkNiftiImageIOFactory.h>


namespace fs = std::filesystem;
const int snrSizeMm = 10;
const int order = 4;

template <typename TImage>
typename TImage::Pointer ReadImageT(const std::string& fn)
{
  using Reader = itk::ImageFileReader<TImage>;
  auto r = Reader::New();
  r->SetFileName(fn);
  r->Update();
  return r->GetOutput();
}

template <typename TImage>
void WriteImageT(typename TImage::Pointer img, const std::string& filename)
{
    using WriterType = itk::ImageFileWriter<TImage>;
    auto writer = WriterType::New();
    writer->SetFileName(filename);
    writer->SetInput(img);
    writer->Update();
}

int main(int argc, char * argv []){

    // arg check 
    if (argc != 12) {
        std::cerr
        << "Usage:\n  " << argv[0]
        << " <inputImagePath> <inputMTFCSVPath> <roiAxialSlice>"
        << " <recistAX> <recistAY> <recistBX> <recistBY>"
        << " <roiMultiplier> <lowMod> <highMod> <outputPath>\n"
        << "Got argc=" << argc << "\n";
        return EXIT_FAILURE;
    }

    itk::NiftiImageIOFactory::RegisterOneFactory();

    // arg read
    std::size_t pos = 0;
    const std::string inputImageName = argv[1];
    const std::string inputCSVName = argv[2];
    const int slice = std::stoi(argv[3], &pos);
    const int aX = std::stoi(argv[4], &pos);
    const int aY = std::stoi(argv[5], &pos);
    const int bX = std::stoi(argv[6], &pos);
    const int bY = std::stoi(argv[7], &pos);
    const double roiMult = std::stod(argv[8], &pos);
    double modLow = std::stod(argv[9], &pos);
    double modHigh = std::stod(argv[10], &pos);
    const std::string outPath = argv[11];

    fs::path outDir(outPath);
    std::error_code ec;
    fs::create_directories(outDir, ec);

    if (ec) {
        std::cerr << "Failed to create output directory: " << outDir
                  << " : " << ec.message() << "\n";
        return EXIT_FAILURE;
    }

    // read MTF for frequencies
    ParseMTF parseMTF(modLow, modHigh, inputCSVName);
    std::array<double, 2> frequencies = parseMTF.execute();

    double flow = frequencies[0];
    double fhigh = frequencies[1];

    // read image
    auto image = ReadImageT<ImageType>(inputImageName);

    // extract slice
    GetSlice getSlice(image, slice);
    SliceType::Pointer imageSlice = getSlice.execute();

    // get slice roi
    RecistROI recistROI(imageSlice, aX, aY, bX, bY, roiMult);
    SliceType::Pointer f = recistROI.execute();

    // ensure fft compatability
    FFTCompatable fftCompatable(f);
    f = fftCompatable.execute();

    // Hann window
    HannWindow hannWindow(f, 0.5, 0.5);
    f = hannWindow.execute();

    // FFT shift -- caveat with this is that image must be even sized, but
    // that is taken care of by fftCompatable
    FFTShift fftShift(f);
    f = fftShift.execute();

    // Forward FFT
    auto fft = itk::ForwardFFTImageFilter<Float2D>::New();
    fft->SetInput(f);
    fft->Update();
    Complex2D::Pointer F = fft->GetOutput();

    // duplicate for band and low pass images
    using DupType = itk::ImageDuplicator<Complex2D>;
    auto duplicator = DupType::New();
    duplicator->SetInputImage(F);
    duplicator->Update();
    Complex2D::Pointer Flp = duplicator->GetOutput();
    Flp->DisconnectPipeline();
    F->DisconnectPipeline();

    // band pass
    BandPassFilter bandPassFilter(F, flow, fhigh, order);
    Float2D::Pointer bandpass = bandPassFilter.execute();

    // low pass
    LowPassFilter lowPassFilter(Flp, flow, order);
    Float2D::Pointer lowpass = lowPassFilter.execute();

    // apply mask 
    using MultiplyImage = itk::MultiplyImageFilter<Complex2D, Float2D, Complex2D>;
    auto multiplyBandpass = MultiplyImage::New();
    multiplyBandpass->SetInput1(F);
    multiplyBandpass->SetInput2(bandpass);
    multiplyBandpass->Update();
    auto multiplyLowpass = MultiplyImage::New();
    multiplyLowpass->SetInput1(Flp);
    multiplyLowpass->SetInput2(lowpass);
    multiplyLowpass->Update();

    auto FBandpassFiltered = multiplyBandpass->GetOutput();
    auto FLowpassFiltered = multiplyLowpass->GetOutput();

    // calculate energy and power
    ImageEnergy bandpassEnergy(FBandpassFiltered);
    double bandpassE = bandpassEnergy.execute();

    ImageEnergy lowpassEnergy(FLowpassFiltered);
    double lowpassE = lowpassEnergy.execute();
    double energyFraction = bandpassE / (bandpassE + lowpassE);

    // ifft
    using IFFTFilter = itk::InverseFFTImageFilter<Complex2D, Float2D>;
    auto ifftBandPass = IFFTFilter::New();
    ifftBandPass->SetInput(FBandpassFiltered);
    ifftBandPass->Update();
    Float2D::Pointer fBandpassFiltered = ifftBandPass->GetOutput();
    fBandpassFiltered->DisconnectPipeline();

    auto ifftLowPass = IFFTFilter::New();
    ifftLowPass->SetInput(FLowpassFiltered);
    ifftLowPass->Update();
    Float2D::Pointer fLowpassFiltered = ifftLowPass->GetOutput();
    fLowpassFiltered->DisconnectPipeline();

    // unshift
    FFTShift bpIfftShift(fBandpassFiltered);
    fBandpassFiltered = bpIfftShift.execute();
    FFTShift lpIfftShift(fLowpassFiltered);
    fLowpassFiltered = lpIfftShift.execute();

    // get magnitude of fourier spectrum for visualization
    using MagnitudeFilter = itk::ComplexToModulusImageFilter<Complex2D, Float2D>;
    auto magnitudeFilter = MagnitudeFilter::New();
    magnitudeFilter->SetInput(F);
    using AddImageFilter = itk::AddImageFilter<Float2D>;
    auto addFilter = AddImageFilter::New();
    addFilter->SetInput(magnitudeFilter->GetOutput());
    addFilter->SetConstant(1e-6f);
    using LogFilter = itk::LogImageFilter<Float2D, Float2D>;
    auto logFilter = LogFilter::New();
    logFilter->SetInput(addFilter->GetOutput());

	const double fxMax = 0.5 / f->GetSpacing()[0];
    const double fyMax = 0.5 / f->GetSpacing()[1]; 

    // output results
    std::ofstream out((outDir / "stats.csv").string());

    if( !out.is_open()){
        return 1;
    }

    out << "name,slice,fxmax,fymax,ax,ay,bx,by,size,bpe,lpe,ef" << std::endl;

    const auto finalSz = f->GetLargestPossibleRegion().GetSize();
    const long outSize = static_cast<long>(finalSz[1]);
    const std::string name = fs::path(inputImageName).filename().string();

    out << name << ","
    << slice << ","
    << fxMax << "," << fyMax << ","
    << aX << "," << aY << ","
    << bX << "," << bY << ","
    << outSize << ","
    << bandpassE << ","
    << lowpassE << ","
    << energyFraction << ","
	<< std::endl;

    WriteImageT<Float2D>(logFilter->GetOutput(), (outDir / "F.nii.gz").string());
    WriteImageT<Float2D>(bandpass,  (outDir / "filter_bp.nii.gz").string());
    WriteImageT<Float2D>(lowpass,   (outDir / "filter_lp.nii.gz").string());
    WriteImageT<Complex2D>(FBandpassFiltered, (outDir / "frequency_bp.nii.gz").string());
    WriteImageT<Complex2D>(FLowpassFiltered, (outDir / "frequency_lp.nii.gz").string());
    WriteImageT<Float2D>(fBandpassFiltered, (outDir / "image_bp.nii.gz").string());
    WriteImageT<Float2D>(fLowpassFiltered, (outDir / "image_lp.nii.gz").string());

    std::cout << "Finished." << std::endl;

    return EXIT_SUCCESS;
}
