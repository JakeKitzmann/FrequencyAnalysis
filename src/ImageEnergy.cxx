#include "ImageEnergy.h"
#include <itkImageRegionIterator.h>

ImageEnergy::ImageEnergy(Complex2D::Pointer imageIn)
{
    image = imageIn;
}

double ImageEnergy::execute()
{
    const auto region = image->GetLargestPossibleRegion();
    double energy = 0.0;

    // power image
    auto power = Float2D::New();
    power->CopyInformation(image);
    power->SetRegions(region);
    power->Allocate();
    power->FillBuffer(0.0f);

    itk::ImageRegionIterator<Complex2D> imageIterator(image, region);
    itk::ImageRegionIterator<Float2D> powerIterator(power, region);
    for(imageIterator.GoToBegin(), powerIterator.GoToBegin(); !imageIterator.IsAtEnd() && !powerIterator.IsAtEnd(); ++imageIterator, ++powerIterator){
        const auto pxl = imageIterator.Get();

        const double real = static_cast<double>(pxl.real());
        const double imag = static_cast<double>(pxl.imag());

        powerIterator.Set(static_cast<float>(real*real + imag*imag));
        }

    // sum power image for energy
    itk::ImageRegionIterator<Float2D> energyIterator(power, region);
    for(energyIterator.GoToBegin(); !energyIterator.IsAtEnd(); ++energyIterator){
        energy += static_cast<double>(energyIterator.Get());
    }

    return energy;
}
