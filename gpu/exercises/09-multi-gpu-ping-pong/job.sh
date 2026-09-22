#!/bin/bash
#SBATCH --job-name=test
#SBATCH --account=project_462001452
#SBATCH --reservation=SummerSchoolGPU
#SBATCH --partition=small-g
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=2
#SBATCH --cpus-per-task=1
#SBATCH --gpus-per-node=2
#SBATCH --mem-per-cpu=1G
#SBATCH --time=00:05:00

# Set the number of threads based on cpus-per-task
export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK:-1}
module load LUMI/25.03 partition/G rocm/6.3.4
export MPICH_GPU_SUPPORT_ENABLED=1
# Run the program
srun ./ping-pong
