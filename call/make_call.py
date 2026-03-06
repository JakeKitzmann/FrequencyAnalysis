#!/usr/bin/env python3

import csv
import sys
from pathlib import Path

if len(sys.argv) != 8:
    print(f"Usage: {sys.argv[0]} names.csv slice aR aA bR bA out_dir")
    sys.exit(1)

csv_file = sys.argv[1]
slice_idx = sys.argv[2]
aR = sys.argv[3]
aA = sys.argv[4]
bR = sys.argv[5]
bA = sys.argv[6]
out_dir = sys.argv[7]

with open(csv_file) as f, open("call.txt", "w") as out:
    reader = csv.DictReader(f)

    for row in reader:
        name = row["Name"]

        cmd = (
            f"FrequencyAnalysis "
            f"--name {name} "
            f"--slice {slice_idx} "
            f"--aR {aR} --aA {aA} "
            f"--bR {bR} --bA {bA} "
            f"--out {out_dir}/{name}.nii.gz"
        )

        out.write(cmd + "\n")