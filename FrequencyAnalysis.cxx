// C++
#include <iostream>
#include <fstream>
#include <filesystem>

// my classes
#include "src/RecistROI.h"
#include "src/FFTCompatable.h"
#include "src/ImageTypes.h"
#include "src/HannWindow.h"
#include "src/FFTShift.h"
#include "src/BandPassFilter.h"
#include "src/LowPassFilter.h"
#include "src/ImageEnergy.h"
#include "src/ParseMTF.h"

// ITK
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkCastImageFilter.h>
#include <itkForwardFFTImageFilter.h>
#include <itkExtractImageFilter.h>
#include <itkImageDuplicator.h>
#include <itkInverseFFTImageFilter.h>
#include <itkMultiplyImageFilter.h>
#include <itkComplexToModulusImageFilter.h>
#include <itkAddImageFilter.h>
#include <itkLogImageFilter.h>

// sem
#include "FrequencyAnalysisCLP.h"

namespace fs = std::filesystem;
const int order = 4;

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

    PARSE_ARGS;
    if (debug) std::cout << "[1] Args parsed" << std::endl;

    ParseMTF parseMTF(modLow, modHigh, inputCSVName);
    std::array<double, 2> frequencies = parseMTF.execute();
    if (debug) std::cout << "[2] ParseMTF done" << std::endl;

    double flow = frequencies[0];
    double fhigh = frequencies[1];
    if (debug) std::cout << "[3] flow=" << flow << " fhigh=" << fhigh << std::endl;

    // "read" image (we're not actually reading because we only need one slice)
    using ReaderType = itk::ImageFileReader<ImageType>;

    auto reader = ReaderType::New();
    reader->SetFileName(inputImageName);

    reader->UpdateOutputInformation();
    if (debug) std::cout << "[4] Image info read" << std::endl;

    auto region = reader->GetOutput()->GetLargestPossibleRegion();
    auto size = region.GetSize();
    auto start = region.GetIndex();

    start[2] = slice;
    size[2] = 0;

    // XML arg validation
    // physical point to index already handled by bool later

    if (modLow < 0 || modLow > 1.0 || modHigh < 0.0 || modHigh > 1.0)
    {
        std::cout << "ERROR: Invalid modulation" << std::endl;
        return EXIT_FAILURE;
    }

    if(roiMult < 0.0)
    {
        std::cout << "ERROR: Invalid roiMult" << std::endl;
        return EXIT_FAILURE; 
    }


    ImageType::RegionType desiredRegion;
    desiredRegion.SetIndex(start);
    desiredRegion.SetSize(size);

    using ExtractFilterType = itk::ExtractImageFilter<ImageType, SliceType>;
    auto extractor = ExtractFilterType::New();
    extractor->SetInput(reader->GetOutput());
    extractor->SetExtractionRegion(desiredRegion);
    extractor->SetDirectionCollapseToSubmatrix();

    if (debug) std::cout << "[5] Extracting slice " << slice << std::endl;
    extractor->Update();
    SliceType::Pointer sliceImage = extractor->GetOutput();
    if (debug) std::cout << "[6] Slice extracted" << std::endl;
    sliceImage->DisconnectPipeline();

    // convert to slice index
    // note that we don't need S because it's a 2D image now
    // multiply by -1 for ras -> lps
    SliceType::IndexType aIdx;
    SliceType::PointType a;
    a[0] = aR;
    a[1] = -1 * aA;
    if (debug) std::cout << "[7] Transforming point A (" << a[0] << ", " << a[1] << ") to index" << std::endl;
    bool oob = sliceImage->TransformPhysicalPointToIndex(a, aIdx);
    if (debug) std::cout << "[8] Point A index: (" << aIdx[0] << ", " << aIdx[1] << ") in-bounds=" << oob << std::endl;
    if(!oob)
    {
        std::cout << "point A outside image slice" << std::endl;
        std::cout << "aR: " << aR << std::endl;
        std::cout << "aA: " << aA << std::endl;
        return EXIT_FAILURE;
    }

    SliceType::IndexType bIdx;
    SliceType::PointType b;
    b[0] = bR;
    b[1] = -1 * bA;
    if (debug) std::cout << "[9] Transforming point B (" << b[0] << ", " << b[1] << ") to index" << std::endl;
    oob = sliceImage->TransformPhysicalPointToIndex(b, bIdx);
    if (debug) std::cout << "[10] Point B index: (" << bIdx[0] << ", " << bIdx[1] << ") in-bounds=" << oob << std::endl;
    if(!oob)
    {
        std::cout << "point B outside image slice" << std::endl;
        return EXIT_FAILURE;
    }

    // get slice roi
    if (debug) std::cout << "[11] Computing RecistROI" << std::endl;
    RecistROI recistROI(sliceImage, aIdx[0], aIdx[1], bIdx[0], bIdx[1], roiMult);
    SliceType::Pointer f = recistROI.execute();
    if (debug) std::cout << "[12] RecistROI done, size: " << f->GetLargestPossibleRegion().GetSize() << std::endl;

    if (outputImages) {
        WriteImageT<Float2D>(f, roi);

    }

    // ensure fft compatability
    if (debug) std::cout << "[13] FFTCompatable" << std::endl;
    FFTCompatable fftCompatable(f);
    f = fftCompatable.execute();
    if (debug) std::cout << "[14] FFTCompatable done, size: " << f->GetLargestPossibleRegion().GetSize() << std::endl;

    // Hann window
    if (debug) std::cout << "[15] HannWindow" << std::endl;
    HannWindow hannWindow(f, 0.5, 0.5);
    f = hannWindow.execute();
    if (debug) std::cout << "[16] HannWindow done" << std::endl;

    // FFT shift -- caveat with this is that image must be even sized, but
    // that is taken care of by fftCompatable
    if (debug) std::cout << "[17] FFTShift (pre-FFT)" << std::endl;
    FFTShift fftShift(f);
    f = fftShift.execute();
    if (debug) std::cout << "[18] FFTShift done" << std::endl;

    // Forward FFT
    if (debug) std::cout << "[19] Forward FFT" << std::endl;
    auto fft = itk::ForwardFFTImageFilter<Float2D>::New();
    fft->SetInput(f);
    fft->Update();
    Complex2D::Pointer F = fft->GetOutput();
    if (debug) std::cout << "[20] Forward FFT done" << std::endl;

    // duplicate for band and low pass images
    using DupType = itk::ImageDuplicator<Complex2D>;
    auto duplicator = DupType::New();
    duplicator->SetInputImage(F);
    duplicator->Update();
    Complex2D::Pointer Flp = duplicator->GetOutput();
    Flp->DisconnectPipeline();
    F->DisconnectPipeline();

    // band pass
    if (debug) std::cout << "[21] BandPassFilter" << std::endl;
    BandPassFilter bandPassFilter(F, flow, fhigh, order);
    Float2D::Pointer bandpass = bandPassFilter.execute();
    if (debug) std::cout << "[22] BandPassFilter done" << std::endl;

    // low pass
    if (debug) std::cout << "[23] LowPassFilter" << std::endl;
    LowPassFilter lowPassFilter(Flp, flow, order);
    Float2D::Pointer lowpass = lowPassFilter.execute();
    if (debug) std::cout << "[24] LowPassFilter done" << std::endl;

    // apply mask
    if (debug) std::cout << "[25] Applying bandpass mask" << std::endl;
    using MultiplyImage = itk::MultiplyImageFilter<Complex2D, Float2D, Complex2D>;
    auto multiplyBandpass = MultiplyImage::New();
    multiplyBandpass->SetInput1(F);
    multiplyBandpass->SetInput2(bandpass);
    multiplyBandpass->Update();
    if (debug) std::cout << "[26] Applying lowpass mask" << std::endl;
    auto multiplyLowpass = MultiplyImage::New();
    multiplyLowpass->SetInput1(Flp);
    multiplyLowpass->SetInput2(lowpass);
    multiplyLowpass->Update();
    if (debug) std::cout << "[27] Masks applied" << std::endl;

    auto FBandpassFiltered = multiplyBandpass->GetOutput();
    auto FLowpassFiltered = multiplyLowpass->GetOutput();

    // calculate energy and power
    if (debug) std::cout << "[28] Computing energies" << std::endl;
    ImageEnergy bandpassEnergy(FBandpassFiltered);
    double bandpassE = bandpassEnergy.execute();

    ImageEnergy lowpassEnergy(FLowpassFiltered);
    double lowpassE = lowpassEnergy.execute();
    double energyFraction = bandpassE / (bandpassE + lowpassE);
    if (debug) std::cout << "[29] bandpassE=" << bandpassE << " lowpassE=" << lowpassE << " ef=" << energyFraction << std::endl;

    // ifft
    if (debug) std::cout << "[30] Inverse FFT (bandpass)" << std::endl;
    using IFFTFilter = itk::InverseFFTImageFilter<Complex2D, Float2D>;
    auto ifftBandPass = IFFTFilter::New();
    ifftBandPass->SetInput(FBandpassFiltered);
    ifftBandPass->Update();
    Float2D::Pointer fBandpassFiltered = ifftBandPass->GetOutput();
    fBandpassFiltered->DisconnectPipeline();
    if (debug) std::cout << "[31] Inverse FFT (lowpass)" << std::endl;
    auto ifftLowPass = IFFTFilter::New();
    ifftLowPass->SetInput(FLowpassFiltered);
    ifftLowPass->Update();
    Float2D::Pointer fLowpassFiltered = ifftLowPass->GetOutput();
    fLowpassFiltered->DisconnectPipeline();
    if (debug) std::cout << "[32] Inverse FFTs done" << std::endl;

    // unshift
    if (debug) std::cout << "[33] FFTShift (post-IFFT)" << std::endl;
    FFTShift bpIfftShift(fBandpassFiltered);
    fBandpassFiltered = bpIfftShift.execute();
    FFTShift lpIfftShift(fLowpassFiltered);
    fLowpassFiltered = lpIfftShift.execute();
    if (debug) std::cout << "[34] FFTShift done" << std::endl;

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
    if (debug) std::cout << "[35] Writing CSV to " << outPath << std::endl;
	std::ofstream out(outPath);

    if( !out.is_open()){
        std::cout << "ERROR: could not open output file: " << outPath << std::endl;
        return 1;
    }

    out << "name,slice,fxmax,fymax,aR,aA,aI,aJ,bR,bA,bI,bJ,size,bpe,lpe,ef" << std::endl;

    const auto finalSz = f->GetLargestPossibleRegion().GetSize();
    const long outSize = static_cast<long>(finalSz[1]);
    const std::string name = fs::path(inputImageName).filename().string();

    out << name << ","
    << slice << ","
    << fxMax << "," << fyMax << ","
    << aR << "," << aA << ","
    << aIdx[0] << "," << aIdx[1] << ","
    << bR << "," << bA << ","
    << bIdx[0] << "," << bIdx[1] << ","
    << outSize << ","
    << bandpassE << ","
    << lowpassE << ","
    << energyFraction
	<< std::endl;

    if (debug) std::cout << "[36] CSV written" << std::endl;

    if (outputImages) {
        if (debug) std::cout << "[37] Writing output images" << std::endl;
        WriteImageT<Float2D>(logFilter->GetOutput(), outputF);
        WriteImageT<Float2D>(bandpass,               outputFilterBP);
        WriteImageT<Float2D>(lowpass,                outputFilterLP);
        WriteImageT<Float2D>(fBandpassFiltered,      outputImageBP);
        WriteImageT<Float2D>(fLowpassFiltered,       outputImageLP);
        if (debug) std::cout << "[38] Output images written" << std::endl;
    }

    if (debug) std::cout << "[39] Done" << std::endl;
    return EXIT_SUCCESS;
}
