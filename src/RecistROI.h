#include <itkImage.h>
#include "ImageTypes.h"

class RecistROI
{
    private:
        SliceType::Pointer image;
        int aX;
        int aY;
        int bX;
        int bY;
        double roiMult;
    public:
        RecistROI(SliceType::Pointer imageIn, int aXIn, int aYIn, int bXIn, int bYIn, double roiMultIn);
        SliceType::Pointer execute() const;
};