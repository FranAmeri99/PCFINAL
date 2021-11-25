echo "--------------- MPI 1 ---------------"

mpicc Final_MPI1.c -o mpiv1
mpirun --oversubscribe -np 4 ./mpiv1

echo "--------------- MPI 2 ---------------"

mpicc Final_MPI2.c -o mpiv2
mpirun --oversubscribe -np 16 ./mpiv2
echo "--------------- OMP 1 ---------------"

export OMP_NUM_THREADS=4
gcc -o omp1 -fopenmp Final_OMP1.c
./omp1
echo "--------------- OMP 2 ---------------"

export OMP_NUM_THREADS=16
gcc -o omp2 -fopenmp Final_OMP2.c
./omp2
