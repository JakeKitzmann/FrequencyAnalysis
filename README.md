# FrequencyAnalysis

A tool for extracting spatial frequency information from lung tissue, airways, and other small objects in 
medical image modalities.

## Args:
FrequencyAnalysis --modHigh <float> --modLow <float> --roiMult <float> --slice <int> --bA <float>
  --bR <float> --aA <float> --aR <float> --inputCSVName <std::string> --outputDirectory <std::string>
  --inputImageName <std::string>


## Requirements

CMake >= 3.17
ITK >= 5.0
SlicerExecutionModel
C++ 17 Compiler

## Setup 

git clone https://github.com/username/ResampleFromImage
cd ResampleFromImage \n
mkdir build
cd build
cmake ..
make

### Note on CMakeLists.txt

Due to use in UIowa's Argon HPC cluster, lines for ignoring GTest, and some includes are included in
CMakeLists.txt

To run you may need to comment out:
add_library(GTest::GTest INTERFACE IMPORTED)
add_library(GTest::Main INTERFACE IMPORTED)
png
z

## Executable

Executable located at build/bin/FrequencyAnalysis

## Credits

This tool is completely free to use and publish with. If this aided your publication, please cite

Jacob J. Kitzmann, BSE, The University of Iowa College of Engineering
(add in paper citation when we get published)
