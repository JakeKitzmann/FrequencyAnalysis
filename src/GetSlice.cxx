#include "GetSlice.h"
#include <itkExtractImageFilter.h>

GetSlice::GetSlice(ImageType::Pointer imageIn, int sliceIn)
{
    image = imageIn;
    slice = sliceIn;
}

SliceType::Pointer  GetSlice::execute() const
{
    using Extract2D = itk::ExtractImageFilter<ImageType, SliceType>;
    auto extract = Extract2D::New();

    const auto region = image->GetLargestPossibleRegion();
    const auto size0  = region.GetSize();
    const auto index0 = region.GetIndex();

    const long z0 = static_cast<long>(index0[2]);
    const long nz = static_cast<long>(size0[2]);
    const long z1 = z0 + nz - 1;

    ImageType::IndexType idx = index0;
    ImageType::SizeType  sz  = size0;

    idx[2] = static_cast<ImageType::IndexType::IndexValueType>(slice);
    sz[2]  = 0; // extract a 2D slice

    ImageType::RegionType desired;
    desired.SetIndex(idx);
    desired.SetSize(sz);

    extract->SetInput(image);
    extract->SetExtractionRegion(desired);
    extract->SetDirectionCollapseToSubmatrix();

    extract->Update();

    // graft disconnected out to new image so that it's not
    // deleted with rest of crap when filter is destructed
    auto out = SliceType::New();
    out->Graft(extract->GetOutput());
    out->DisconnectPipeline();    

    return out;
}