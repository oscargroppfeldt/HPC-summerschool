#!/bin/bash
#SBATCH --job-name=test
#SBATCH --account=project_462001452
#SBATCH --reservation=SummerSchoolGPU
#SBATCH --partition=small-g
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=4
#SBATCH --gpus-per-node=1
#SBATCH --mem-per-cpu=1G
#SBATCH --time=00:05:00

# Set the number of threads based on cpus-per-task
export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK:-1}

export CRAY_ACC_DEBUG=2
# Run the program
srun ./poisson.x 1024 2000 2
