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
)

add_executable(FrequencyAnalysis
  main.cxx
  GetSlice.cxx
  RecistROI.cxx
  FFTCompatable.cxx
  HannWindow.cxx
  FFTShift.cxx
  BandPassFilter.cxx
  LowPassFilter.cxx
  ImageEnergy.cxx
  SignalNoise.cxx
  ParseMTF.cxx
)

target_compile_features(FrequencyAnalysis PRIVATE cxx_std_17)

# Modern ITK: prefer targets
target_link_libraries(FrequencyAnalysis PRIVATE ${ITK_LIBRARIES})


