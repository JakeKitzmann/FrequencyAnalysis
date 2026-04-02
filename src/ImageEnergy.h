#include "ImageTypes.h"

class ImageEnergy
{
    private:
        Complex2D::Pointer image;
    public:
        ImageEnergy(Complex2D::Pointer imageIn);
        double execute();
};