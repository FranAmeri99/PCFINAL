#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <sys/time.h>
#include <unistd.h>
#include "funciones.h"
//double func(double x);
//double metodoRectangulo(double a, int n, double deltaX);//, int tid);
//double metodoPMedio(double a, int n, double deltaX);//, int tid);
//double metodoTrapecio(double a, double b, int n, double deltaX);//, int tid);
//double metodoSimpson(double a, double b, int n, double deltaX);//, int tid);

int main() {
    clock_t start, end;
    double start1 , end1;
    double tiempo_total;

    start1 = omp_get_wtime();

    const double pi = 3.14159265359;

    /*Rango de integración */
    double a = 3, b = 30;

    /*Cantidad de intervalos */
    int n = 1000000;

    double deltaX = (b-a)/n;
    int tid;
    double resultado1 = 0.0, resultado2 = 0.0, resultado3 = 0.0, resultado4 = 0.0;

    printf("\nVersion 1 OMP\n");

    printf("\nMetodos de integracion numerica\n");

    printf("Funcion: x*x");
    printf("\nRango [%.2f, %.2f] con %d intervalos\n\n", a, b, n);

    start = clock();

    /*Ejecución de los 4 métodos en paralelo, cada uno en un hilo*/
    #pragma omp parallel shared(a,b,n,deltaX) private(tid)
    {
        tid = omp_get_thread_num();
        #pragma omp sections nowait //Cada seccion se ejecuta por un thread
        {
            #pragma omp section
                resultado1 = metodoRectangulo(a,n,deltaX);//,tid);
                
            #pragma omp section
                resultado2 = metodoPMedio(a,n,deltaX);//,tid);,tid);
                
            #pragma omp section
                resultado3 = metodoTrapecio(a,b,n,deltaX);//,tid);,tid);

             #pragma omp section
                resultado4 = metodoSimpson(a,b,n,deltaX);//,tid);,tid);
  
        }
    }

    printf("-Metodo de rectangulo: %f\n", resultado1);
    printf("-Metodo punto medio: %f\n", resultado2);
    printf("-Metodo de trapecio: %f\n", resultado3);
    printf("-Metodo de Simpson: %f\n\n", resultado4);

    end = clock();

    tiempo_total = (end-start)/(double)CLOCKS_PER_SEC;
    end1 = omp_get_wtime(); 

    printf("Tiempo de uso de CPU : %fs\n", tiempo_total);
    printf("Tiempo de ejecucion  total: %fs\n", end1 - start1);
    
    return 0;
}
//
///*Función que calcula la f(x) a integrar valuada en la x pasada como parámetro*/
//double func(double x) {
//     //return sin(x);
//     return x+x;
//}
//
///* Regla del rectángulo */
//double metodoRectangulo(double a, int n, double deltaX){//, int tid) {
//    double x = 0.0, resultado = 0.0;
//    int i;
//
//    for(i = 0; i < n; i++) {
//        x = deltaX * i + a;
//        resultado += func(x);
//    }
//
//    resultado *= deltaX;
//
//    return resultado;
//}
//
///* Regla del Punto Medio */
//double metodoPMedio(double a, int n, double deltaX){//, int tid) {
//    double x = 0.0, resultado = 0.0;
//    int i;
//
//    for(i = 1; i <= n; i++) {
//        x = (deltaX*(i-1) + deltaX*i + 2*a) / 2;
//        resultado += func(x);
//    }
//
//    resultado *= deltaX;
//
//    return resultado;
//}
//
///* Regla del Trapecio */
//double metodoTrapecio(double a, double b, int n, double deltaX){//, int tid) {
//    double x = 0.0, resultado = 0.0;
//
//    int i;
//
//    for(i = 1; i <= n-1; i++) {
//        x = deltaX * i + a;
//        resultado += 2*func(x);
//    }
//
//    resultado += func(a) + func(b);
//    resultado *= deltaX/2;
//
//    return resultado;
//}
//
///* Simpson 1/3 */
//double metodoSimpson(double a, double b, int n, double deltaX){
//    double x = 0.0, resultado = 0.0;
//    int i;
//
//    for(i = 1; i < n; i++) {
//        x = a + deltaX * i;
//
//        if(i % 2 != 0) {
//            resultado += 4*func(x);
//        } else {
//            resultado += 2*func(x);
//        }
//    }
//
//    resultado += func(a) + func(b);
//    resultado *= deltaX/3;
//
//    return resultado;
//}