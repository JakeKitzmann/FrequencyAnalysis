#include "HannWindow.h"
#include <itkImageRegionIterator.h>

HannWindow::HannWindow(Float2D::Pointer imageIn, double aIn, double bIn)
{
    image = imageIn;
    a = aIn;
    b = bIn;
}

Float2D::Pointer HannWindow::execute() const
{
    const double twoPi = 2.0 * std::acos(-1.0);


    // --- your block (correct) ---
    const auto windowRegion = image->GetLargestPossibleRegion();
    const auto windowSize   = windowRegion.GetSize();
    const auto windowIndex  = windowRegion.GetIndex();

    const long windowN = static_cast<long>(windowSize[0]); // columns (x)
    const long windowM = static_cast<long>(windowSize[1]); // rows (y)
    // ----------------------------

    itk::ImageRegionIterator<Float2D> it(image, windowRegion);
    for (it.GoToBegin(); !it.IsAtEnd(); ++it) {
        const auto idx = it.GetIndex();

        const double x = static_cast<double>(idx[0] - windowIndex[0]);
        const double y = static_cast<double>(idx[1] - windowIndex[1]);

        // Hann / generalized Hamming
        const double wx = a - b * std::cos(twoPi * x / (double(windowN) - 1.0));
        const double wy = a - b * std::cos(twoPi * y / (double(windowM) - 1.0));

        it.Set(it.Get() * static_cast<float>(wx * wy));
    }

    return image;
}
