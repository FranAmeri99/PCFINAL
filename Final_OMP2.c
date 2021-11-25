#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <unistd.h>

double func(double x);
double metodoRectangulo(double a, int n, double deltaX, int tid, int divArreglo, int cantHilos);
double metodoPMedio(double a, int n, double deltaX, int tid, int divArreglo, int cantHilos);
double metodoTrapecio(double a, double b, int n, double deltaX, int tid, int divArreglo, int cantHilos);
double metodoSimpson(double a, double b, int n, double deltaX, int tid, int divArreglo, int cantHilos);

int main() {
    clock_t start, end;
    double start1, end1;
    double tiempo_total;

    start1 = omp_get_wtime(); 

    /*Paralelismo anidado*/
    omp_set_nested(1);

    const double pi = 3.14159265359;

    /*Rango de integración */
    double a = 3, b = 30;
    
    /*Cantidad de intervalos */
    int n = 1000000;
    
    double deltaX = (b-a)/n;
    double resultado1 = 0.0, resultado2 = 0.0, resultado3 = 0.0, resultado4 = 0.0;
    int tid, divArreglo = 4, cantHilos = 4;

    printf("\nVersion 2 OMP\n");
    
    printf("\nMetodos de integracion numerica\n");
    
    printf("Funcion: x*x");
    printf("\nRango [%.2f, %.2f] con %d intervalos\n\n", a, b, n);
    
    start = clock();

    /*Ejecución de los 4 métodos en paralelo, cada uno en un hilo*/
    #pragma omp parallel shared(a, b, n, deltaX, divArreglo, cantHilos) private(tid)
    {
        tid = omp_get_thread_num();
        /* printf("Hello from thread %d/%d\n", omp_get_thread_num(), omp_get_num_threads()); */
        /*Cada hilo ingresa en una section*/
        #pragma omp sections nowait
        {
            #pragma omp section
            {
                resultado1 = metodoRectangulo(a,n,deltaX,tid,divArreglo,cantHilos);
            }
            
            #pragma omp section
            {
                resultado2 = metodoPMedio(a,n,deltaX,tid,divArreglo,cantHilos);
            }
                
            #pragma omp section
            {
                resultado3 = metodoTrapecio(a,b,n,deltaX,tid,divArreglo,cantHilos);
            }

            #pragma omp section
            {
                resultado4 = metodoSimpson(a,b,n,deltaX,tid,divArreglo,cantHilos);
            }
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

    //sleep(500);
    return 0;
}

/*Función que calcula la f(x) a integrar valuada en la x pasada como parámetro*/
double func(double x) {
    return x*x;
}

/* Regla del rectángulo */
double metodoRectangulo(double a, int n, double deltaX, int tid, int divArreglo, int cantHilos) {
    double x = 0.0, resultado = 0.0;
    int i, id;

    /*Seteo cantidad de hilos que van a trabajar*/
    omp_set_num_threads(cantHilos);
    
    /*Ejecución*/
    /* Se hace el default para que sean shared todas las variables menos las que estan en private*/
    #pragma omp parallel default(shared) private(i,x,id)
    {
        id = omp_get_thread_num();

    /* Se hace un for en donde se va a dividir el arreglo en 4 partes iguales*/
        #pragma omp for schedule(static,divArreglo) reduction(+:resultado)
        for(i = 0; i < n; i++) {
            x = deltaX * i + a; //xi respectivas
            resultado += func(x);
        }
    }

    resultado *= deltaX;

    return resultado;
}

/* Regla del Punto Medio */
double metodoPMedio(double a, int n, double deltaX, int tid, int divArreglo, int cantHilos) {
    double x = 0.0, resultado = 0.0;
    int i, id;

    omp_set_num_threads(cantHilos);
    
    #pragma omp parallel default(shared) private(i,x,id)
    {
        id = omp_get_thread_num();
        
        #pragma omp for schedule(static,divArreglo) reduction(+:resultado)
        for(i = 1; i <= n; i++) {
            x = (deltaX*(i-1) + deltaX*i + 2*a) / 2;
            resultado += func(x);
        }
       // sleep(50);
    }

    resultado *= deltaX;
     
    return resultado;
}

/* Regla del Trapecio */
double metodoTrapecio(double a, double b, int n, double deltaX, int tid, int divArreglo, int cantHilos) {
    double x = 0.0, resultado = 0.0;
    int i, id;

    omp_set_num_threads(cantHilos);
    
    #pragma omp parallel default(shared) private(i,x,id)
    {
        id = omp_get_thread_num();

        #pragma omp for schedule(static,divArreglo) reduction(+:resultado)
        for(i = 1; i <= n-1; i++) {
            x = deltaX * i + a;
            resultado += 2*func(x);
        }
       // sleep(50);
    }

    resultado += func(a) + func(b);
    resultado *= deltaX/2; 

    return resultado;
}

/* Simpson 1/3 */
double metodoSimpson(double a, double b, int n, double deltaX, int tid, int divArreglo, int cantHilos) {
    double x = 0.0, resultado = 0.0;
    int i, id;

    omp_set_num_threads(cantHilos);
    
    #pragma omp parallel default(shared) private(i,x,id)
    {
        id = omp_get_thread_num();

        #pragma omp for schedule(static,divArreglo) reduction(+:resultado)
        for(i = 1; i < n; i++) {
            x = a + deltaX * i;
        
            if(i % 2 != 0) {
                resultado += 4*func(x);
            } else {
                resultado += 2*func(x);
            }
        }
    }

    resultado += func(a) + func(b);
    resultado *= deltaX/3;
    
    return resultado;
}