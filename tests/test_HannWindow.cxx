#include <gtest/gtest.h>
#include "src/HannWindow.h"

// Helper: create a Float2D image of given size filled with a constant value
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

// For standard Hann (a=0.5, b=0.5): w(0) = 0.5 - 0.5*cos(0) = 0
// So any pixel on the border (x=0, y=0, x=N-1, y=M-1) should be zeroed.
TEST(HannWindowTest, CornersAreZero)
{
    auto img = MakeImage(64, 64, 1.0f);
    HannWindow window(img, 0.5, 0.5);
    auto result = window.execute();

    Float2D::IndexType idx;

    idx = {0, 0};
    EXPECT_NEAR(result->GetPixel(idx), 0.0f, 1e-6f);

    idx = {63, 0};
    EXPECT_NEAR(result->GetPixel(idx), 0.0f, 1e-6f);

    idx = {0, 63};
    EXPECT_NEAR(result->GetPixel(idx), 0.0f, 1e-6f);

    idx = {63, 63};
    EXPECT_NEAR(result->GetPixel(idx), 0.0f, 1e-6f);
}

// Output image should be the same size as input.
TEST(HannWindowTest, OutputSizeUnchanged)
{
    auto img = MakeImage(32, 48, 1.0f);
    HannWindow window(img, 0.5, 0.5);
    auto result = window.execute();

    const auto size = result->GetLargestPossibleRegion().GetSize();
    EXPECT_EQ(size[0], 32u);
    EXPECT_EQ(size[1], 48u);
}

// Windowing a zero image should produce a zero image.
TEST(HannWindowTest, ZeroImageRemainsZero)
{
    auto img = MakeImage(32, 32, 0.0f);
    HannWindow window(img, 0.5, 0.5);
    auto result = window.execute();

    Float2D::IndexType idx = {16, 16};
    EXPECT_NEAR(result->GetPixel(idx), 0.0f, 1e-6f);
}
