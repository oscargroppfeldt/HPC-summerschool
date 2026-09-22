#!/bin/bash
#SBATCH --job-name=test
#SBATCH --account=project_462001452
#SBATCH --reservation=SummerSchoolCPU
#SBATCH --partition=small
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=4
#SBATCH --cpus-per-task=8
#SBATCH --mem-per-cpu=1G
#SBATCH --time=00:05:00

export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK:-1}
export OMP_DISPLAY_AFFINITY=true
export OMP_AFFINITY_FORMAT="Process %P level %L thread %0.4n/%0.4N on node %H core %A"
export OMP_PLACES=cores
export OMP_PROC_BIND=spread
srun heat_hybrid
