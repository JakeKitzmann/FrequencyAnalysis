#include <itkImage.h>

using PixelType = float;
using ImageType = itk::Image<PixelType, 3>;
using SliceType = itk::Image<PixelType, 2>;
using Float2D = itk::Image<float, 2>;
using Complex2D = itk::Image<std::complex<float>, 2>;
