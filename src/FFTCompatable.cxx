#include "FFTCompatable.h"
#include <itkRegionOfInterestImageFilter.h>

FFTCompatable::FFTCompatable(Float2D::Pointer imageIn)
{
    image = imageIn;
}

Float2D::Pointer FFTCompatable::execute()
{
    // make image compatable with ITK FFT
    const long W = static_cast<long>(image->GetLargestPossibleRegion().GetSize()[0]);
    const long H = static_cast<long>(image->GetLargestPossibleRegion().GetSize()[1]);
    const auto idx = image->GetLargestPossibleRegion().GetIndex();

    const long compatableW = FFTCompatable::makeFFTCompatable(W);
    const long compatableH = FFTCompatable::makeFFTCompatable(H);

    // crop to largest compatable size if not compatable
    if(compatableH != H || compatableW != W){

        // get corner of compatable image for ROI index
        const long x0 = static_cast<long>(idx[0]) + (W - compatableW) / 2;
        const long y0 = static_cast<long>(idx[1]) + (H - compatableH) / 2;

        Float2D::IndexType compatableIdx;
        compatableIdx[0] = static_cast<Float2D::IndexType::IndexValueType>(x0);
        compatableIdx[1] = static_cast<Float2D::IndexType::IndexValueType>(y0);

        Float2D::SizeType compatableSize;
        compatableSize[0] = static_cast<Float2D::SizeType::SizeValueType>(compatableW);
        compatableSize[1] = static_cast<Float2D::SizeType::SizeValueType>(compatableH);

        Float2D::RegionType cropRegion;
        cropRegion.SetIndex(compatableIdx);
        cropRegion.SetSize(compatableSize);

        using Crop2D = itk::RegionOfInterestImageFilter<Float2D, Float2D>;
        auto crop = Crop2D::New();
        crop->SetInput(image);
        crop->SetRegionOfInterest(cropRegion);
        crop->Update();

        Float2D::Pointer out = Float2D::New();
        out->Graft(crop->GetOutput());
        out->DisconnectPipeline();

        return out;
    }
    return image;
}

int FFTCompatable::makeFFTCompatable(int n)
{
    for (long k = n; k >= 1; k--){
        if(FFTCompatable::isFFTCompatable(k)) return k;
    }
    return 1;
}

bool FFTCompatable::isFFTCompatable(int n){
    if(n <= 0) return false;

    // iteratively determine divisible by 2, 3, and 5.
    while(n % 2 == 0) n /= 2;
    while(n % 3 == 0) n /= 3;
    while(n % 5 == 0) n /= 5; 

    // only 1 if divisible by some combination or factor of these 3
    return n == 1;
}