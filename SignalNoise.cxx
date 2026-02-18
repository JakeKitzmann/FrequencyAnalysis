#include "SignalNoise.h"
#include <itkRegionOfInterestImageFilter.h>
#include <itkStatisticsImageFilter.h>
#include <cstdlib>

SignalNoise::SignalNoise(SliceType::Pointer imageIn, int xIn, int yIn, int sizeIn)
{
    image = imageIn;
    x = xIn;
    y = yIn;
    size = sizeIn;
}

SliceType::Pointer SignalNoise::getROI()
{
    using ROI2D = itk::RegionOfInterestImageFilter<SliceType, SliceType>;
    auto roi = ROI2D::New();

    SliceType::IndexType roiStart;
    roiStart[0] = x;
    roiStart[1] = y;

    SliceType::SizeType roiSize; 
    roiSize[0] = size;
    roiSize[1] = size;

    SliceType::RegionType roiRegion;
    roiRegion.SetIndex(roiStart);
    roiRegion.SetSize(roiSize);

    roi->SetInput(image);
    roi->SetRegionOfInterest(roiRegion);
    roi->Update();

    region = roi->GetOutput();
    return region;
}

double SignalNoise::execute()
{
    // snrdB = 10 * log10(mean / std)
    using StatisticsFilterType = itk::StatisticsImageFilter<SliceType>;
    auto statisticsImageFilter = StatisticsFilterType::New();

    statisticsImageFilter->SetInput(region);
    statisticsImageFilter->Update();

    double sd = statisticsImageFilter->GetSigma();
    double mean = statisticsImageFilter->GetMean();

    double snr = std::abs(mean) / (0.00001 + std::abs(sd));

    return snr;
}