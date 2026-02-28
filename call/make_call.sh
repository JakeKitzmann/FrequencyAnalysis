#!/usr/bin/env bash
set -euo pipefail

CSV="${1:?Usage: $0 points.csv out_base_dir}"
OUT_BASE="${2:?Usage: $0 points.csv out_base_dir}"

BASE_DIR="$HOME/data/2-28"
RESOLUTIONS=("1024" "512")
EXT=".nii.gz"

CUTOFFS=(
  "0.1 0.2" "0.1 0.3" "0.1 0.4" "0.1 0.5" "0.1 0.6" "0.1 0.7" "0.1 0.8" "0.1 0.9"
  "0.2 0.3" "0.2 0.4" "0.2 0.5" "0.2 0.6" "0.2 0.7" "0.2 0.8" "0.2 0.9"
  "0.3 0.4" "0.3 0.5" "0.3 0.6" "0.3 0.7" "0.3 0.8" "0.3 0.9"
  "0.4 0.5" "0.4 0.6" "0.4 0.7" "0.4 0.8" "0.4 0.9"
  "0.5 0.6" "0.5 0.7" "0.5 0.8" "0.5 0.9"
  "0.6 0.7" "0.6 0.8" "0.6 0.9"
  "0.7 0.8" "0.7 0.9"
  "0.8 0.9"
)

: > call.txt

tail -n +2 "$CSV" | while IFS=, read -r \
  Name slice \
  aX aY bX bY \
  || [[ -n "${Name:-}" ]]
do
  Name="${Name%$'\r'}"
  Name="${Name//[[:space:]]/}"
  [[ -z "${Name:-}" ]] && continue

  NUM="${Name#r}"

  for RES in "${RESOLUTIONS[@]}"; do

    # ----- Image mapping -----
    if [[ "$Name" =~ ^r[0-9]+$ ]]; then
      INPUT_IMAGE="${BASE_DIR}/${RES}rs/${RES}_DFOV_${NUM}_resampled${EXT}"
    elif [[ "$Name" =~ ^[0-9]+$ ]]; then
      INPUT_IMAGE="${BASE_DIR}/${RES}/${RES}_DFOV_${NUM}${EXT}"
    else
      echo "ERROR: Unrecognized Name format: '$Name'" >&2
      exit 1
    fi

    [[ -f "$INPUT_IMAGE" ]] || {
      echo "ERROR: Missing image file: $INPUT_IMAGE" >&2
      exit 1
    }

    # ----- MTF mapping -----
    if [[ "$Name" =~ ^r[0-9]+$ ]]; then
      MTF_CSV="${BASE_DIR}/passResults/mtf/${RES}/${RES}_DFOV_${NUM}_resampled.x.mtf_curve.csv"
    else
      MTF_CSV="${BASE_DIR}/passResults/mtf/${RES}/${RES}_DFOV_${NUM}.x.mtf_curve.csv"
    fi

    [[ -f "$MTF_CSV" ]] || {
      echo "ERROR: Missing MTF file: $MTF_CSV" >&2
      exit 1
    }

    for pair in "${CUTOFFS[@]}"; do
      read -r MOD_LOW MOD_HIGH <<< "$pair"
      tag="m${MOD_LOW/./}_${MOD_HIGH/./}"

      OUTDIR="${OUT_BASE}/${RES}/${Name}/${tag}"
      mkdir -p "$OUTDIR"

      cmd=(
        FrequencyAnalysis
        "$INPUT_IMAGE"
        "$MTF_CSV"
        "$slice"
        "$aX" "$aY" "$bX" "$bY"
        "1.0"
        "$MOD_LOW"
        "$MOD_HIGH"
        "$OUTDIR"
      )

      printf '%q ' "${cmd[@]}" >> call.txt
      printf '\n' >> call.txt
    done

  done
done

echo "Wrote $(wc -l < call.txt) commands to call.txt"