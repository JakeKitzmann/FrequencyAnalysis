cmake_minimum_required(VERSION 3.20)
project(FrequencyAnalysis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SlicerExecutionModel REQUIRED GenerateCLP)
include(${GenerateCLP_USE_FILE})
include(${SlicerExecutionModel_USE_FILE})

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

# The macro creates FrequencyAnalysisLib from ADDITIONAL_SRCS internally,
# then wraps it in the FrequencyAnalysis executable for Slicer.
SEMMacroBuildCLI(
    NAME FrequencyAnalysis
    ADDITIONAL_SRCS
        src/RecistROI.cxx
        src/FFTCompatable.cxx
        src/HannWindow.cxx
        src/FFTShift.cxx
        src/BandPassFilter.cxx
        src/LowPassFilter.cxx
        src/ImageEnergy.cxx
        src/ParseMTF.cxx
    TARGET_LIBRARIES
        ${ITK_LIBRARIES}
)

# Expose headers to tests via the macro-created lib target
target_include_directories(FrequencyAnalysisLib PUBLIC ${CMAKE_SOURCE_DIR})

option(BUILD_TESTING "Build tests" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
