#include <itkImage.h>
#include "ImageTypes.h"

class GetSlice
{
    private:
        ImageType::Pointer image;
        int slice;

    public:
        GetSlice(ImageType::Pointer imageIn, int sliceIn);
        SliceType::Pointer execute() const;
};