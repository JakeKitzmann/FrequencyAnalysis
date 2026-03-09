#!/usr/bin/env python3

import csv
import sys
from pathlib import Path
from itertools import combinations

if len(sys.argv) != 8:
    print(f'Usage: {sys.argv[0]} input_files.csv slice aR aA bR bA out_dir')
    sys.exit(1)

csv_file = sys.argv[1]
slice_idx = sys.argv[2]
aR = sys.argv[3]
aA = sys.argv[4]
bR = sys.argv[5]
bA = sys.argv[6]
base_out_dir = Path(sys.argv[7])

# Generate modulation pairs (modLow < modHigh)
mod_values = [round(x * 0.1, 1) for x in range(1, 10)]
mod_pairs = list(combinations(mod_values, 2))

call_file = Path("call.txt")
with open(call_file, "w") as out:
    with open(csv_file) as f:
        reader = csv.DictReader(f)
        for row in reader:
            image_path = Path(row["Name"])
            # Properly remove .nii.gz
            base_name = image_path.name
            if base_name.endswith(".nii.gz"):
                base_name = base_name[:-7]

            # Full path CSV: directory + base name + .x.mtf_curve.csv
            inputCSVName = str(image_path.parent / f"{base_name}.x.mtf_curve.csv")

            for modLow, modHigh in mod_pairs:
                # Nested output folder: base_out_dir/modLow_modHigh/scan_name
                output_dir = base_out_dir / f"{modLow}_{modHigh}" / base_name
                output_dir.mkdir(parents=True, exist_ok=True)

                cmd = (
                    f"FrequencyAnalysis "
                    f"--inputImageName {image_path} "
                    f"--outputDirectory {output_dir} "
                    f"--inputCSVName {inputCSVName} "
                    f"--slice {slice_idx} "
                    f"--aR {aR} --aA {aA} "
                    f"--bR {bR} --bA {bA} "
                    f"--modLow {modLow} --modHigh {modHigh} "
                    f"--roiMult 1.0"
                )
                out.write(cmd + "\n")

print(f"Wrote all commands to {call_file}")
