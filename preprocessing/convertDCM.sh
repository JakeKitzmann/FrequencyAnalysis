#!/bin/bash

# ---------------- SETTINGS ----------------
INPUT_DIR="/home/jkitzmann/data/FOVData/3-4/in/FOVData"   # top-level folder with DICOM subfolders
OUTPUT_DIR="/home/jkitzmann/data/FOVData/3-4/in/FOVData"  # output folder for NIfTIs
ITK_THREADS=12                    # threads per DicomConvert process
# -----------------------------------------

# Set ITK threads
export ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS=$ITK_THREADS

# Make sure output folder exists
mkdir -p "$OUTPUT_DIR"

echo "ITK threads per job: $ITK_THREADS"
echo "Input: $INPUT_DIR"
echo "Output: $OUTPUT_DIR"
echo "-------------------------------------"

# Process subfolders sequentially
for folder in "$INPUT_DIR"/*/; do
    [ -d "$folder" ] || continue  # skip if not a directory
    name=$(basename "$folder")
    echo "Processing $name"
    echo "Writing to: $OUTPUT_DIR/$name.nii.gz"
    DicomConvert "$folder" "$OUTPUT_DIR/$name.nii.gz"
done

echo "All scans processed."
