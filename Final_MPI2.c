#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>
#include <unistd.h>
#include "funciones.h"

int main (int argc, char** argv) {
    /* Variables para calcular el tiempo total de ejecucion */
    clock_t start, end;

    double tiempo_total;
    /* tomo el tiempo de cpu */
    double time1 = MPI_Wtime();

    const double pi = 3.14159265359;

    /*Rango de integración */
   
    double a = 3 , b = 30;

    /*Cantidad de intervalos */
    int n = 1000000;

    /* calculo dalta */
    double deltaX = (b-a)/n;

    double second_a, second_b, second_n;
    double resultado = 0.0, resultado1 = 0.0, resultado2 = 0.0, resultado3 = 0.0, resultado4 = 0.0;
    int rank, cant_total, color, second_rank, second_cant_total;

    MPI_Status status;
    
    /*Inicialización del entorno de ejecución MPI*/
    MPI_Init(&argc,&argv);
    /* Determina el Rango del proceso de llamada en el comunicador */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    /* Determina el tamanano del grupo asociado para un comunicador */
    MPI_Comm_size(MPI_COMM_WORLD, &cant_total);

    /*Si la cantidad de procesos ingresada en la linea de comando no es 16 se cierra*/
    if(cant_total < 16) {
        if (rank == 0) {
            printf("La cantidad de procesos debe ser igual a 16\n");
            MPI_Finalize();
            exit(0);
        }
    } else {
        /* defino el color para cada hilo */
        color = rank/4;
        /* van a ser cuatro hilos por cada metodo */
        /*Se divide el comunicador en 4 nuevos comunicadores*/
        MPI_Comm second_comm;
        MPI_Comm_split(MPI_COMM_WORLD, color, rank, &second_comm);

        MPI_Comm_rank(second_comm, &second_rank);
        MPI_Comm_size(second_comm, &second_cant_total);
        printf("hilo: %d  proceso %d \n", rank, second_rank);

        /*Variables locales para cada proceso según su rank en cada comunicador*/
        second_n = n/second_cant_total;
        second_a = a + second_rank*second_n*deltaX;
        second_b = second_a + second_n*deltaX;

        /*El proceso root en el comunicador WORLD imprime los mensajes*/
        if(rank == 0) {
            printf("\nVersion 2 MPI\n");
            printf("\nMetodos de integracion numerica\n");
            printf("Funcion: x*x");
            printf("\nRango [%.2f, %.2f] con %d intervalos\n\n", a, b, n);
            start = clock();

        }

        /*Primer comunicador*/
        if(color == 0) {

            /*Todos los procesos que pertenecen a este comunicador realizan la porción de integración que les toca*/
            resultado = metodoRectangulo(second_a, second_n, deltaX);

            /*Se obtiene el resultado total del método*/
            /* va sumando cada resultado de la porcion correspondiente del comunicador para tener el resutado */
            MPI_Reduce(&resultado, &resultado1, 1, MPI_DOUBLE, MPI_SUM, 0, second_comm);

            /*Si es el proceso root del comunicador imprime el resultado*/
            if(second_rank == 0) {
                printf("-Metodo de rectangulo %f\n", resultado1);
            }
        }

        /*Segundo comunicador*/
        if(color == 1) {
            
            resultado = metodoPMedio(second_a,second_n,deltaX);

            MPI_Reduce(&resultado, &resultado2, 1, MPI_DOUBLE, MPI_SUM, 0, second_comm);

            if(second_rank == 0) {
                printf("-Metodo punto medio: %f\n", resultado2);
            }
        }

        /*Tercer comunicador*/
        if(color == 2) {
            
            resultado = metodoTrapecio(second_a,second_b,second_n,deltaX);

            MPI_Reduce(&resultado, &resultado3, 1, MPI_DOUBLE, MPI_SUM, 0, second_comm);

            if(second_rank == 0) {
                printf("-Metodo de trapecio: %f\n", resultado3);
            }
        }

        /*Cuarto comunicador*/
        if(color == 3) {
            resultado = metodoSimpson(second_a,second_b,second_n,deltaX);

            MPI_Reduce(&resultado, &resultado4, 1, MPI_DOUBLE, MPI_SUM, 0, second_comm);

            if(second_rank == 0) {
                printf("-Metodo de Simpson : %f\n", resultado4);
            }
        }
        
      // printf("\n Hola soy proceso --> %d con id hijo --> %d \n:",rank,second_rank);

       /*El barrier permite que todos los procesos se sincronicen para que el root termine la ejecucion*/
        MPI_Barrier(MPI_COMM_WORLD);

        if(rank == 0) {
            end = clock();

            tiempo_total = (end-start)/(double)CLOCKS_PER_SEC;

            double elapsed = MPI_Wtime() - time1;

            printf("\nTiempo de uso de CPU: %fs\n", tiempo_total);
            printf("Tiempo de ejecucion  total: %fs\n", elapsed);
        }

        //sleep(50);
        MPI_Comm_free(&second_comm);
        MPI_Finalize();
    }
}
