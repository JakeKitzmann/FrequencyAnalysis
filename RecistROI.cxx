#include "RecistROI.h"
#include <itkRegionOfInterestImageFilter.h>

SliceType::Pointer RecistROI::execute() const 
{
    using ROI = itk::RegionOfInterestImageFilter<SliceType, SliceType>;
    auto roi = ROI::New();

    const int midpointX = 0.5 * (aX + bX);
    const int midpointY = 0.5 * (aY + bY);

    const int bigX = std::max(aX, bX);
    const int bigY = std::max(aY, bY);
    const int littleX = std::min(aX, bX);
    const int littleY = std::min(aY, bY);

    const int roiRadius = std::hypot((bigX-littleX), (bigY-littleY));

    SliceType::IndexType roiStart;
    roiStart[0] = midpointX - roiRadius;
    roiStart[1] = midpointY - roiRadius;
 
    SliceType::SizeType roiSize;
    roiSize[0] = 1 + roiMult * 2 * roiRadius;
    roiSize[1] = 1 + roiMult * 2 * roiRadius;

    SliceType::RegionType roiRegion;
    roiRegion.SetIndex(roiStart);
    roiRegion.SetSize(roiSize);

    roi->SetInput(image);
    roi->SetRegionOfInterest(roiRegion);

    roi->Update();

    auto out = SliceType::New();
    out->Graft(roi->GetOutput());
    out->DisconnectPipeline();
    return out;
}

RecistROI::RecistROI(SliceType::Pointer imageIn, int aXIn, int aYIn, int bXIn, int bYIn, double roiMultIn)
{
    image = imageIn;
    aX = aXIn;
    aY = aYIn;
    bX = bXIn;
    bY = bYIn;
    roiMult = roiMultIn;
}

