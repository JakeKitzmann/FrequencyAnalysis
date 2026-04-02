#include "ImageTypes.h"

class HannWindow
{
    private:
        Float2D::Pointer image;
        double a;
        double b;
    public:
        HannWindow(Float2D::Pointer imageIn, double aIn, double bIn);
        Float2D::Pointer execute() const;
};
