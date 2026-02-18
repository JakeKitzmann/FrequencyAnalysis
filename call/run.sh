#!/bin/bash
#$ -N FOVAnalysis
#$ -cwd
#$ -o logs/output.log
#$ -pe smp 12


module purge
module load stack/2021.1
module load cmake/3.20.0_gcc-9.3.0
module load itk/5.1.2_gcc-9.3.0
module load fftw/3.3.9_gcc-9.3.0


while read -r cmd; do
    echo "Running: $cmd"
    $cmd
done < call.txt
