#!/bin/bash
#SBATCH --job-name=python-multiproc
#SBATCH --output=slurm-%j.out          # standard output goes here
#SBATCH --error=slurm-%j.err           # errors go here
#SBATCH --time=08:00:00                # 8 hours
#SBATCH --partition=courses            # adjust if needed
#SBATCH --nodes=1                      # keep it on one node
#SBATCH --ntasks=1                     # one task (Python process)
#SBATCH --cpus-per-task=8              # number of CPUs for multiprocessing
#SBATCH --mem=32G                      # total memory
#SBATCH --account=bassigem            # replace with your account if required

# Run your Python script
python param_scan_2.2.py 8
