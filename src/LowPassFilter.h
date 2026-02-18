#include "ImageTypes.h"

class LowPassFilter
{
    private:
        Complex2D::Pointer image;
        double flow;
        int order;
    public:
        LowPassFilter(Complex2D::Pointer imageIn, double flowIn, int orderIn);
        Float2D::Pointer execute();
};