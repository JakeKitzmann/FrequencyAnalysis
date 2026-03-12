#!/bin/bash

# -------- SETTINGS --------
INPUT_DIR="/home/jkitzmann/data/FOVData/3-4/1024IR3"
OUTPUT_DIR="/home/jkitzmann/data/FOVData/3-4/1024IR3rs"
FIXED_IMAGE="/home/jkitzmann/data/FOVData/3-4/1024IR3/1024_DFOV_500_IR3.nii.gz"

# --------------------------

mkdir -p "$OUTPUT_DIR"

echo "Fixed image: $FIXED_IMAGE"
echo "Input dir:   $INPUT_DIR"
echo "Output dir:  $OUTPUT_DIR"
echo "-------------------------------------"

ls "$INPUT_DIR"/*.nii.gz | grep -v "$FIXED_IMAGE" | \
xargs -P 16 -I {} bash -c '
name=$(basename {} .nii.gz)
ResampleFromImage "'"$FIXED_IMAGE"'" {} "'"$OUTPUT_DIR"'/${name}_resampled.nii.gz" linear
'

echo "All resampling complete."
