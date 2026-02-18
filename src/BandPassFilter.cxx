#include "BandPassFilter.h"
#include <itkImageRegionIterator.h>

BandPassFilter::BandPassFilter(Complex2D::Pointer imageIn, double flowIn, double fhighIn, int orderIn)
{
    image = imageIn;
    flow = flowIn;
    fhigh = fhighIn;
    order = orderIn;
}

Float2D::Pointer BandPassFilter::execute()
{
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

    auto bandpass = Float2D::New();
    bandpass->SetRegions(frequencyRegion);
    bandpass->CopyInformation(image);
    bandpass->Allocate();
    bandpass->FillBuffer(0.0f);

        // create band pass mask
    itk::ImageRegionIterator<Float2D> bandpassIterator(bandpass, frequencyRegion);
    for(bandpassIterator.GoToBegin(); !bandpassIterator.IsAtEnd(); ++bandpassIterator){
        const auto idx = bandpassIterator.GetIndex();

        // pixel coordinates relative to region index
        const double x = static_cast<double>(idx[0] - frequencyIndex[0]);
        const double y = static_cast<double>(idx[1] - frequencyIndex[1]);

        // cycles/mm
        const double u = (x - u0) / (static_cast<double>(Nu) * dx);
        const double v = (y - v0) / (static_cast<double>(Nv) * dy);

        const double R = std::sqrt(u*u + v*v);

        // Butterworth LP (cutoff fh)
        const double lp = 1.0 / (1.0 + std::pow(R / fhigh, 2.0 * static_cast<double>(order)));

        // Butterworth HP (cutoff fl)
        // hp = 1 / (1 + (fl/R)^(2n)), with hp(0) = 0
        double hp = 0.0;
        if (R > 0.0) {
            hp = 1.0 / (1.0 + std::pow(flow / R, 2.0 * static_cast<double>(order)));
        }

        const double H = hp * lp;

        bandpassIterator.Set(static_cast<float>(H));

    }

    return bandpass;
}