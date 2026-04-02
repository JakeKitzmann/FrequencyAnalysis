#include "ImageTypes.h"

class BandPassFilter
{
    private:
        Complex2D::Pointer image;
        double flow;
        double fhigh;
        int order;
    public:
        BandPassFilter(Complex2D::Pointer imageIn, double flowIn, double fhighIn, int orderIn);
        Float2D::Pointer execute();
};