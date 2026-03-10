cmake_minimum_required(VERSION 3.20)
project(FrequencyAnalysis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# sem
set(BUILD_TESTING OFF)

# dummy GTest exports bc argon is wierd
add_library(GTest::GTest INTERFACE IMPORTED)
add_library(GTest::Main INTERFACE IMPORTED)

find_package(SlicerExecutionModel REQUIRED GenerateCLP)
include(${GenerateCLP_USE_FILE})
include(${SlicerExecutionModel_USE_FILE})
set(FrequencyAnalysis_SOURCE FrequencyAnalysis.cxx)
GENERATECLP(FrequencyAnalysis_SOURCE FrequencyAnalysis.xml)

# itk
set(MINIMUM_ITK_VERSION 5.0)
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

# sem executable
SEMMacroBuildCLI(
        NAME FrequencyAnalysis

        SRCS
        ${FrequencyAnalysis_SOURCE}

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
        png
        z
)
