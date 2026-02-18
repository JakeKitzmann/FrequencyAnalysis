#include "ImageTypes.h"

class FFTShift
{
    private:
        Float2D::Pointer image;
    public:
        FFTShift(Float2D::Pointer imageIn);
        Float2D::Pointer execute() const;
};