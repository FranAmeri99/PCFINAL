#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "funciones.h"
//
//double func(double x);
//double metodoRectangulo(double a, int n, double deltaX);
//double metodoPMedio(double a, int n, double deltaX);
//double metodoTrapecio(double a, double b, int n, double deltaX);
//double metodoSimpson(double a, double b, int n, double deltaX);

int main() {
    clock_t start, end;
    struct timeval start1, end1;
    /*Arranca a medir el tiempo de ejecucion total*/
    gettimeofday(&start1, 0);
    double tiempo_total;

    const double pi = 3.14159265359;

    /*Rango de integración */
    double a = 3, b = 30;

    /*Cantidad de intervalos */
    int n = 12800;
    /* Tamano de c/intervalo*/
    double deltaX = (b-a)/n;

    double resultado1 = 0.0, resultado2 = 0.0, resultado3 = 0.0, resultado4 = 0.0;

    printf("\nVersion Secuencial\n");

    printf("\nMetodos de integracion numerica\n");
   
    printf("Funcion: x*x");
    printf("\nRango [%.2f, %.2f] con %d intervalos\n\n", a, b, n);
    
    start = clock();

    resultado1 = metodoRectangulo(a,n,deltaX);
    resultado2 = metodoPMedio(a,n,deltaX);
    resultado3 = metodoTrapecio(a,b,n,deltaX);
    resultado4 = metodoSimpson(a,b,n,deltaX);

    end = clock();

    tiempo_total = (end-start)/(double)CLOCKS_PER_SEC;

    printf("-Metodo de rectangulo: %f\n", resultado1);
    printf("-Metodo punto medio: %f\n", resultado2);
    printf("-Metodo de trapecio: %f\n", resultado3);
    printf("-Metodo de Simpson: %f\n\n", resultado4);

    end = clock();
    tiempo_total = (end-start)/(double)CLOCKS_PER_SEC;

    gettimeofday(&end1, 0);
    long seconds = end1.tv_sec - start1.tv_sec;
    long microseconds = end1.tv_usec - start1.tv_usec;
    double elapsed = seconds + microseconds*1e-6;

    printf("Tiempo total de uso de CPU: %fs\n", tiempo_total);
    printf("Tiempo de ejecucion  total: %fs\n", elapsed);
    //sleep(200);
    return 0;
}
//
///*Función que calcula la f(x) a integrar valuada en la x pasada como parámetro*/
//double func(double x) {
//    /* return sin(x); */
//    return x*x;
//}
//
///* Regla del rectángulo */
//double metodoRectangulo(double a, int n, double deltaX) {
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
//double metodoPMedio(double a, int n, double deltaX) {
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
//double metodoTrapecio(double a, double b, int n, double deltaX) {
//    double x = 0.0, resultado = 0.0;
//    int i;
//
//    for(i = 1; i <= n-1; i++) {
//        x = deltaX * i + a; //Calculamos los xi respectivos
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
//double metodoSimpson(double a, double b, int n, double deltaX) {
//    double x = 0.0, resultado = 0.0;
//    int i;
//
//    for(i = 1; i < n; i++) {
//        x = deltaX * i + a; //Calculamos los xi respectivos
//
//        if(i % 2 != 0) {
//            resultado += 4*func(x); //xi impares
//        } else {
//            resultado += 2*func(x); //xi pares
//        }
//    }
//
//    resultado += func(a) + func(b);
//    resultado *= deltaX/3;
//
//    return resultado;
//}