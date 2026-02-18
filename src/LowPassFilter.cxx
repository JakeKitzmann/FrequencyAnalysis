#include "LowPassFilter.h"
#include <itkImageRegionIterator.h>

LowPassFilter::LowPassFilter(Complex2D::Pointer imageIn, double flowIn, int orderIn)
{
    image = imageIn;
    flow = flowIn;
    order = orderIn;
}

Float2D::Pointer LowPassFilter::execute()
{
    // create frequency domain filters
    const auto spacing = image->GetSpacing();
    const double dx = spacing[0];
    const double dy = spacing[1];

    const auto frequencyRegion = image->GetLargestPossibleRegion();
    const auto frequencySize = frequencyRegion.GetSize();
    const auto frequencyIndex = frequencyRegion.GetIndex();

    const long Nu = static_cast<long>(frequencySize[0]);
    const long Nv = static_cast<long>(frequencySize[1]);
        
    const long u0 = Nu / 2;
    const long v0 = Nv / 2;

    auto lowpass = Float2D::New();
    lowpass->SetRegions(frequencyRegion);
    lowpass->CopyInformation(image);
    lowpass->Allocate();
    lowpass->FillBuffer(0.0f);

    itk::ImageRegionIterator<Float2D> lowpassIterator(lowpass, frequencyRegion);
    for(lowpassIterator.GoToBegin(); !lowpassIterator.IsAtEnd(); ++lowpassIterator){
        const auto idx = lowpassIterator.GetIndex();

        const double x = static_cast<double>(idx[0] - frequencyIndex[0]); // pixel coordinates
        const double y = static_cast<double>(idx[1] - frequencyIndex[1]);

        const double u = (static_cast<double>(x) - u0) / (static_cast<double>(Nu) * dx);
        const double v = (static_cast<double>(y) - v0) / (static_cast<double>(Nv) * dy);

        const double R = std::sqrt(u*u + v*v); // radial distance to center

        // Butterworth low-pass
        const double ratio = R / flow;
        const double H = 1.0 / (1.0 + std::pow(ratio, 2.0 * static_cast<double>(order)));

        lowpassIterator.Set(static_cast<float>(H));
    }

    return lowpass;
}