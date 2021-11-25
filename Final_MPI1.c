#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>
#include <unistd.h>
#include "funciones.h"

int main (int argc, char** argv) {
    clock_t start, end;
    double tiempo_total;

    double time1 = MPI_Wtime();

    const double pi = 3.14159265359;

    /*Rango de integración */

    double a = 3, b = 30;

    /*Cantidad de intervalos */
    int n = 1000000;

    double deltaX = (b-a)/n;
    double arreglo[4];
    double resultado = 0.0;
    int rank, cant, source, dest = 0, tag = 0;
    MPI_Status status;

    /*Inicialización del entorno MPI*/
    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &cant);

    /* Solo permite que sean 4 procesos que es lo que pide el enunciado*/
    if(cant != 4) {
        if (rank == 0) {
            printf("La cantidad de procesos debe ser igual a 4\n");
            MPI_Finalize();
            exit(0);
        }
    }

    /*Proceso root*/
    if(rank == 0) {
        start = clock();

        printf("\nVersion 1 MPI\n");
        printf("\nMetodos de integracion numerica\n");
        printf("Funcion: x*x");
        printf("\nRango [%.2f, %.2f] con %d intervalos\n\n", a, b, n);

        arreglo[0] = metodoRectangulo(a,n,deltaX);

          for(int i = 1; i < cant; i++) {
            MPI_Recv(&resultado, 1, MPI_DOUBLE, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
            arreglo[status.MPI_SOURCE] = resultado;
            }

        printf("-Metodo de rectangulo: %f\n", arreglo[0]);
        printf("-Metodo punto medio: %f\n", arreglo[1]);
        printf("-Metodo de trapecio: %f\n", arreglo[2]);
        printf("-Metodo de Simpson: %f\n\n", arreglo[3]);

        end = clock();

        tiempo_total = (end-start)/(double)CLOCKS_PER_SEC; //solo se mide tiempo de cpu

        double elapsed = MPI_Wtime() - time1;

        printf("Tiempo de uso de CPU : %fs\n", tiempo_total);
        printf("Tiempo de ejecucion  total: %fs\n", elapsed);
    }

    /*Cada proceso realiza un método de integración distinto y luego envía el resultado al proceso root*/
    if(rank == 1) {
        resultado = metodoPMedio(a,n,deltaX);
        MPI_Send(&resultado, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
    }

    if(rank == 2) {
        resultado = metodoTrapecio(a,b,n,deltaX);
        MPI_Send(&resultado, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
    }

    if(rank == 3) {
        resultado = metodoSimpson(a,b,n,deltaX);
        MPI_Send(&resultado, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}
