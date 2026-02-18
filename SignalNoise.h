#include "ImageTypes.h"

class SignalNoise
{
    private:
        SliceType::Pointer image;
        int x;
        int y;
        int size;
        SliceType::Pointer region;
    public:
        SignalNoise(SliceType::Pointer imageIn, int xIn, int yIn, int sizeIn);
        double execute();
        SliceType::Pointer getROI();
};