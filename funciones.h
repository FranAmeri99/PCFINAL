//
// Created by francisco on 24/11/21.
//

#ifndef FINALPC2021_FUNCIONES_H
#define FINALPC2021_FUNCIONES_H

/*Función que calcula la f(x) a integrar valuada en la x pasada como parámetro*/
double func(double x) {
    /* return sin(x); */
    return x*x;
}

/* Regla del rectángulo */
double metodoRectangulo(double a, int n, double deltaX) {
    double x = 0.0, resultado = 0.0;
    int i;

    for(i = 0; i < n; i++) {
        x = deltaX * i + a;
        resultado += func(x);
    }

    resultado *= deltaX;

    return resultado;
}

/* Regla del Punto Medio */
double metodoPMedio(double a, int n, double deltaX) {
    double x = 0.0, resultado = 0.0;
    int i;

    for(i = 1; i <= n; i++) {
        x = (deltaX*(i-1) + deltaX*i + 2*a) / 2;
        resultado += func(x);
    }

    resultado *= deltaX;

    return resultado;
}

/* Regla del Trapecio */
double metodoTrapecio(double a, double b, int n, double deltaX) {
    double x = 0.0, resultado = 0.0;
    int i;

    for(i = 1; i <= n-1; i++) {
        x = deltaX * i + a; //Calculamos los xi respectivos
        resultado += 2*func(x);
    }

    resultado += func(a) + func(b);
    resultado *= deltaX/2;

    return resultado;
}

/* Simpson 1/3 */
double metodoSimpson(double a, double b, int n, double deltaX) {
    double x = 0.0, resultado = 0.0;
    int i;

    for(i = 1; i < n; i++) {
        x = deltaX * i + a; //Calculamos los xi respectivos

        if(i % 2 != 0) {
            resultado += 4*func(x); //xi impares
        } else {
            resultado += 2*func(x); //xi pares
        }
    }

    resultado += func(a) + func(b);
    resultado *= deltaX/3;

    return resultado;
}
#endif //FINALPC2021_FUNCIONES_H
