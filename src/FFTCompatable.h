#include <itkImage.h>
#include "ImageTypes.h"

class FFTCompatable
{
    private:
        Float2D::Pointer image;
        int makeFFTCompatable(int n);
        bool isFFTCompatable(int n);
    public:
        FFTCompatable(Float2D::Pointer imageIn);
        Float2D::Pointer execute();

};