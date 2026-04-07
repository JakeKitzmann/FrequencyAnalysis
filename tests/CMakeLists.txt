include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

add_executable(test_HannWindow test_HannWindow.cxx)
add_executable(test_FFTShift test_FFTShift.cxx)
target_link_libraries(
    test_HannWindow
    PRIVATE
    FrequencyAnalysisLib
    GTest::gtest_main
)

target_link_libraries(test_FFTShift
    PRIVATE
    FrequencyAnalysisLib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_HannWindow)
gtest_discover_tests(test_FFTShift)
