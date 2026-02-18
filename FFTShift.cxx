#include "FFTShift.h"
#include <itkImageRegionIterator.h>

FFTShift::FFTShift(Float2D::Pointer imageIn)
{
    image = imageIn;
}

Float2D::Pointer FFTShift::execute() const{
    const auto shiftRegion = image->GetLargestPossibleRegion();
    const auto shiftSize = shiftRegion.GetSize();
    const auto shiftIndex = shiftRegion.GetIndex();

    itk::ImageRegionIterator<Float2D> shiftIterator(image, shiftRegion);
    for(shiftIterator.GoToBegin(); !shiftIterator.IsAtEnd(); ++shiftIterator){
        const auto index = shiftIterator.GetIndex();

        const long x = static_cast<long>(index[0] - shiftIndex[0]);
        const long y = static_cast<long>(index[1] - shiftIndex[1]);

        const float mask = ((x + y) & 1) ? -1.0f : 1.0f; // equivalent of (-1)^(x+y)
        shiftIterator.Set(shiftIterator.Get() * mask);
    }

    return image;
}