# FrequencyAnalysis

A tool for extracting spatial frequency information from lung tissue, airways, and other small objects in 
medical image modalities.

## Args:
FrequencyAnalysis

--modHigh <float>

--modLow <float> 

--roiMult <float> 

--slice <int> 

--bA <float>
 
--bR <float> 
  
--aA <float> 

--aR <float> 

--inputCSVName <std::string> 

--outputDirectory <std::string>
  
--inputImageName <std::string>


## Requirements

CMake >= 3.17

ITK >= 5.0

SlicerExecutionModel

C++ 17 Compiler

## Setup

```bash
git clone https://github.com/username/ResampleFromImage
cd ResampleFromImage
mkdir build
cd build
cmake ..
make
```

## Development Setup

After cloning, install the pre-commit hook to automatically run tests before each commit:

```bash
sh scripts/setup-hooks.sh
```

Tests are built by default. To skip them:

```bash
cmake .. -DBUILD_TESTING=OFF
```

## Executable

Executable located at build/bin/FrequencyAnalysis

## Credits

This tool is completely free to use and publish with. If this aided your publication, please cite

Jacob J. Kitzmann, BSE, The University of Iowa College of Engineering
(add in paper citation when we get published)

### To Do
- Windows compatability?
- Run w/o MTF curve (given spatial frequencies)
- ~~Test driven development -- incorporate unit tests to run after changes.~~
