cmake_minimum_required(VERSION 3.20)
project(FrequencyAnalysis LANGUAGES CXX)

set(MINIMUM_ITK_VERSION 5.0)

# Pick only what you actually use. Start a bit broad, then trim once it builds.
find_package(ITK ${MINIMUM_ITK_VERSION} REQUIRED COMPONENTS
  ITKCommon
  ITKIOImageBase
  ITKIONIFTI
  ITKImageFilterBase
  ITKImageIntensity
  ITKFFT
  ITKTransform
  ITKStatistics
  ITKImageGrid
)

include(${ITK_USE_FILE})

add_executable(FrequencyAnalysis
  src/main.cxx
  src/GetSlice.cxx
  src/RecistROI.cxx
  src/FFTCompatable.cxx
  src/HannWindow.cxx
  src/FFTShift.cxx
  src/BandPassFilter.cxx
  src/LowPassFilter.cxx
  src/ImageEnergy.cxx
  src/SignalNoise.cxx
  src/ParseMTF.cxx
)


target_link_libraries(FrequencyAnalysis ${ITK_LIBRARIES})


