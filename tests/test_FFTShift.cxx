#include <gtest/gtest.h>
#include "src/FFTShift.h"
#include <itkImageRegionIterator.h>

static Float2D::Pointer MakeImage(int width, int height, float fill = 1.0f)
{
    Float2D::Pointer img = Float2D::New();
    Float2D::IndexType start;
    start.Fill(0);
    Float2D::SizeType size;
    size[0] = width;
    size[1] = height;
    Float2D::RegionType region(start, size);
    img->SetRegions(region);
    img->Allocate();
    img->FillBuffer(fill);
    return img;
}

// Even (x+y) pixels should be unchanged (multiplied by 1).
// Odd (x+y) pixels should be negated (multiplied by -1).
// Since the image is filled with 1.0f we know the expected values exactly.
TEST(FFTShiftTest, CheckMultiplication)
{
    auto img = MakeImage(64, 64, 1.0f);
    FFTShift fftShift(img);
    auto f = fftShift.execute();

    itk::ImageRegionIterator<Float2D> it(f, f->GetLargestPossibleRegion());
    for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
        const auto idx = it.GetIndex();
        if ((idx[0] + idx[1]) % 2 == 0)
            EXPECT_NEAR(it.Get(), 1.0f, 1e-6f);
        else
            EXPECT_NEAR(it.Get(), -1.0f, 1e-6f);
    }
}

// Applying the shift twice should restore the original values.
TEST(FFTShiftTest, DoubleShiftRestoresOriginal)
{
    auto img = MakeImage(8, 8, 3.0f);
    Float2D::IndexType idx = {3, 5};
    float original = img->GetPixel(idx);

    FFTShift shift1(img);
    shift1.execute();

    FFTShift shift2(img);
    shift2.execute();

    EXPECT_NEAR(img->GetPixel(idx), original, 1e-6f);
}

// Output size should be unchanged.
TEST(FFTShiftTest, OutputSizeUnchanged)
{
    auto img = MakeImage(16, 32, 1.0f);
    FFTShift shift(img);
    auto result = shift.execute();

    const auto size = result->GetLargestPossibleRegion().GetSize();
    EXPECT_EQ(size[0], 16u);
    EXPECT_EQ(size[1], 32u);
}
