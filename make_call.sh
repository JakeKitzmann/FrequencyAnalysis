#!/usr/bin/env bash
set -euo pipefail

CSV="${1:?Usage: $0 points.csv out_base_dir}"
OUT_BASE="${2:?Usage: $0 points.csv out_base_dir}"

RESAMPLED_DIR="/nfsscratch/jkitzmann/resampled"
NIFTIS_DIR="/nfsscratch/jkitzmann/niftis"
MTF_DIR="/nfsscratch/jkitzmann/mtf"

EXT=".nii.gz"

# Modulation cutoff pairs (low high)
CUTOFFS=(
  "0.2 0.5"
)

: > call.txt

# NOTE: the "|| [[ -n ... ]]" makes sure we don't drop the last row if the file
# doesn't end with a newline.
tail -n +2 "$CSV" | while IFS=, read -r \
  Name slice \
  aX aY bX bY \
  mtfSlice mtfX mtfY \
  airSlice airX airY \
  waterSlice waterX waterY \
  || [[ -n "${Name:-}" ]]
do
  # Clean up possible Windows CRLF and stray whitespace
  Name="${Name%$'\r'}"
  Name="${Name//[[:space:]]/}"
  waterY="${waterY%$'\r'}"

  # Skip empty lines
  [[ -z "${Name:-}" ]] && continue

  # Map Name -> image path
  if [[ "$Name" =~ ^r[0-9]+$ ]]; then
    INPUT_IMAGE="${RESAMPLED_DIR}/${Name}${EXT}"
  elif [[ "$Name" =~ ^[0-9]+$ ]]; then
    INPUT_IMAGE="${NIFTIS_DIR}/${Name}${EXT}"
  else
    echo "ERROR: Unrecognized Name format: '$Name' (expected r### or ###)" >&2
    exit 1
  fi

  [[ -f "$INPUT_IMAGE" ]] || { echo "ERROR: Missing image file: $INPUT_IMAGE" >&2; exit 1; }

  # Per-scan MTF CSV
  MTF_CSV="${MTF_DIR}/${Name}.csv"
  [[ -f "$MTF_CSV" ]] || { echo "ERROR: Missing MTF file: $MTF_CSV" >&2; exit 1; }

  # For each cutoff pair, create a unique outdir and command
  for pair in "${CUTOFFS[@]}"; do
    read -r MOD_LOW MOD_HIGH <<< "$pair"

    # Make a filesystem-safe tag like "m01_06" for 0.1-0.6
    tag="m${MOD_LOW/./}_${MOD_HIGH/./}"

    OUTDIR="${OUT_BASE}/${Name}/${tag}"
    mkdir -p "$OUTDIR"

    # Build the command as an array so nothing gets split/wrapped
    cmd=(
      "../../03_tools/FrequencyAnalysis/build/FrequencyAnalysis"
      "$INPUT_IMAGE"
      "$MTF_CSV"
      "$slice"
      "$aX" "$aY" "$bX" "$bY"
      "$mtfSlice" "$mtfX" "$mtfY"
      "$airSlice" "$airX" "$airY"
      "$waterSlice" "$waterX" "$waterY"
      "1.0"
      "$MOD_LOW"
      "$MOD_HIGH"
      "$OUTDIR"
    )

    # Write exactly ONE runnable line
    printf '%q ' "${cmd[@]}" >> call.txt
    printf '\n' >> call.txt
  done
done

echo "Wrote $(wc -l < call.txt) commands to call.txt"
