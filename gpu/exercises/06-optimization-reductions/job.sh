#!/bin/bash
#SBATCH --job-name=test
#SBATCH --account=project_462001452
#SBATCH --reservation=SummerSchoolGPU
#SBATCH --partition=small-g
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --gpus-per-node=8
#SBATCH --mem-per-cpu=1G
#SBATCH --time=00:05:00

# Set the number of threads based on cpus-per-task
export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK:-1}

# Run the program
srun amd-smi metric 
srun ./hip_reductions --mode naive --repeat 5 
srun ./hip_reductions --mode blockatomic --repeat 5
srun ./hip_reductions --mode shared --repeat 5
srun ./hip_reductions --mode hipcub --repeat 5
