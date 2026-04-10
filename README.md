# FrequencyAnalysis

A 3D Slicer extension for extracting spatial frequency information from medical images. Given an airway RECIST measurement and a CT scan, it computes the energy fraction in a frequency band defined by an MTF curve — useful for characterizing lung tissue, airways, and other small structures. This is currently used for partial volume averaging quantification in CT.

## Requirements

- CMake >= 3.20
- ITK >= 5.0
- SlicerExecutionModel
- C++17 compiler

## Build

```bash
git clone https://github.com/JakeKitzmann/FrequencyAnalysis
cd FrequencyAnalysis
mkdir build && cd build
cmake ..
make
```

The executable will be at `build/bin/FrequencyAnalysis`.

## Usage

```
FrequencyAnalysis --inputImageName <path> --inputCSVName <path> --outPath <path> [options]
```

### Arguments

| Flag | Type | Description |
|------|------|-------------|
| `--inputImageName` | string | Path to the input CT volume (.nii.gz) |
| `--inputCSVName` | string | Path to the MTF curve CSV file |
| `--outPath` | string | Output path for results CSV |
| `--slice` | int | Axial slice index of the RECIST measurement |
| `--aR` | float | R (right) RAS coordinate of first RECIST point |
| `--aA` | float | A (anterior) RAS coordinate of first RECIST point |
| `--bR` | float | R (right) RAS coordinate of second RECIST point |
| `--bA` | float | A (anterior) RAS coordinate of second RECIST point |
| `--modLow` | float | Low modulation bound for frequency band (0–1) |
| `--modHigh` | float | High modulation bound for frequency band (0–1) |
| `--roiMult` | float | RECIST distance multiplier for ROI size (default: 1.0) |
| `--outputImages` | bool | Write intermediate images to disk (default: false) |

## Development Setup

After cloning, install the pre-commit hook to automatically run tests before each commit:

```bash
sh scripts/setup-hooks.sh
```

Tests are built by default and can be run with:

```bash
ctest --test-dir build --output-on-failure
```

To build without tests:

```bash
cmake .. -DBUILD_TESTING=OFF
```

## Credits

This tool is free to use and publish with. If it aided your research, please cite:

Jacob J. Kitzmann, BSE — The University of Iowa College of Engineering
*(publication citation pending)*

## To Do
- Figure out why RAS / LPS is inconsistent across platforms
- Windows compatibility
- Run without MTF curve (user-specified spatial frequencies)
- ~~Test driven development~~
