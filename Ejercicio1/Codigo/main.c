#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>

//Constantes Globales
#define CANTIDAD_AUTOPISTAS 4
#define CANTIDAD_CATEGORIAS 7
#define CANTIDAD_VEHICULOS 1
#define RECAUDACION 2

//Variables Globales
bool es_primero = true;

//Declaración de funciones
void carga_ticket(int matriz_autopistas[][3], int vector_categorias[], int* recaudacion_total, int d, int m, int a, int hora, int* primera_hora, int* primera_autopista);
void autopista_mas_recorrida(int matriz_autopistas[][3], int filas);
void autopista_mas_rentable(int matriz_autopistas[][3], int filas);
void mostrar_porcentaje(int vector_categorias[], int recaudacion);
bool validar_hora(int minutos);
bool validar_fecha(int dia, int mes, int anio);
bool bisiesto(int anio);

int main(void) {
    setlocale(LC_ALL, "spanish");

    //Declaracion de variables
    int info_autopistas[CANTIDAD_AUTOPISTAS][3] = {0};
    int info_categorias[CANTIDAD_CATEGORIAS] = {0};
    int recaudacion_total = 0;
    int dia = 0, mes = 0, anio = 0;
    int hora = 0;
    int primera_hora = 0;
    int primera_autopista = 0;

    //Llamada a funciones
    carga_ticket(info_autopistas, info_categorias, &recaudacion_total, dia, mes, anio, hora, &primera_hora, &primera_autopista);
    autopista_mas_recorrida(info_autopistas, CANTIDAD_AUTOPISTAS);
    autopista_mas_rentable(info_autopistas, CANTIDAD_AUTOPISTAS);
    mostrar_porcentaje(info_categorias, recaudacion_total);

    //Salida por pantalla
    printf("La recaudación total de la empresa fue: %d\n", recaudacion_total);
    printf("El primer ticket del día se realizó a las %d horas en la autopista nro. %d\n", primera_hora, primera_autopista);

    return 0;
}

void carga_ticket(int matriz_autopistas[][3], int vector_categorias[], int* recaudacion_total, int d, int m, int a, int hora, int* primera_hora, int* primera_autopista) {
    printf("Bienvenido/a a la carga de ticket!\n\n");
    //Declaración de variables locales a la función 'carga_ticket'
    int nro_ticket = 0;
    int valor_ticket = 0;
    int nro_autopista = 0;
    int categoria_vehiculo = 0;

    do {
        bool chequeo_autopista = false;
        bool chequeo_categoria = false;
        bool chequeo_fecha = false;
        bool chequeo_hora = false;

        printf("Ingrese el número de ticket (0 para finalizar): ");
        scanf("%d", &nro_ticket);

        if (nro_ticket == 0) {
            break;
        }

        printf("Ingrese el monto del ticket: ");
        scanf("%d", &valor_ticket);

        if (valor_ticket == 0) {
            break;
        }

        while (chequeo_autopista == false) {
            printf("Ingrese el número de autopista (1-4): ");
            scanf("%d", &nro_autopista);
            if (nro_autopista >= 1 && nro_autopista <= 4) {
                matriz_autopistas[nro_autopista - 1][CANTIDAD_VEHICULOS] += 1; //Suma +1 para cantidad de vehiculos
                matriz_autopistas[nro_autopista - 1][RECAUDACION] += valor_ticket; //Establece la recaudacion de cada autopista
                chequeo_autopista = true; // -> Sale del bucle WHILE interno
            } else {
                printf("Número de autopista inválido.\n"); //Vuelve al bucle a solicitar el numero de autopista
            }
        }

        while (chequeo_categoria == false) {
            printf("Ingrese la categoría del vehículo (1-7): ");
            scanf("%d", &categoria_vehiculo);

            if (categoria_vehiculo >= 1 && categoria_vehiculo <= 7) {
                vector_categorias[categoria_vehiculo - 1] += valor_ticket; //Recaudación de cada categoria para porcentaje
                chequeo_categoria = true; // -> Sale del bucle WHILE interno
            } else {
                printf("Categoría inválida.\n"); //Vuelve al bucle a solicitar el número de cateogoria
            }
        }

        while (chequeo_fecha == false) {
            printf("Ingrese la fecha como d/m/aaaa: ");
            scanf("%d %d %d", &d, &m, &a);
            if (validar_fecha(d, m, a) == true) {
                chequeo_fecha = true; // -> Sale del bucle WHILE interno
            } else {
                printf("La fecha es incorrecta.\n"); //Vuelve al bucle a solicitar el número de categoria
            }
        }

        while (chequeo_hora == false) {
            printf("Ingrese la hora (en minutos desde las 0:00): ");
            scanf("%d", &hora);
            if (validar_hora(hora) == true) {
                chequeo_hora = true; // -> Sale del bucle WHILE interno
            } else {
                printf("La hora es incorrecta.\n"); //Vuelve al bucle a solicitar una hora
            }
        }

        //Acumula los valores del ticket
        *recaudacion_total += valor_ticket;

        if (es_primero == true) {
            *primera_autopista = nro_autopista;
            *primera_hora = hora / 60;
            es_primero = false;
        }

    } while (nro_ticket != 0);
}

//Busca máximo de autopista recorrida en base a la cantidad de vehiculos
void autopista_mas_recorrida(int matriz_autopistas[][3], int filas) {
    int max_recorrida = 0;
    int autopista = 0;

    for (int i = 0; i < filas; i++) {
        if (matriz_autopistas[i][CANTIDAD_VEHICULOS] > max_recorrida) {
            max_recorrida = matriz_autopistas[i][CANTIDAD_VEHICULOS]; //Guarda el valor máximo
            autopista = i + 1; //Para mostrar el número de autopista real
        }
    }

    printf("La autopista más recorrida fue la Nro. %d\n", autopista);
}

//Busca máximo de autospita con mayor recaudación
void autopista_mas_rentable(int matriz_autopistas[][3], int filas) {
    int max_rentabilidad = 0;
    int autopista = 0;

    for (int i = 0; i < filas; i++) {
        if (matriz_autopistas[i][RECAUDACION] > max_rentabilidad) {
            max_rentabilidad = matriz_autopistas[i][RECAUDACION]; //Guarda el valor máximo
            autopista = i + 1; //Para mostrar el número de autopista real
        }
    }

    printf("La autopista más rentable fue la Nro. %d\n", autopista);
}

//Calcula el porcentaje recaudado por cada categoria, segun lo recaudado por cada una
void mostrar_porcentaje(int vector_categorias[], int recaudacion) {
    for (int i = 0; i < CANTIDAD_CATEGORIAS; i++) {
        if (recaudacion > 0) {
            float porcentaje = (float)vector_categorias[i] / recaudacion * 100;
            printf("Categoría %d: %.2f%%\n", i + 1, porcentaje);
        }
    }
}

//Valida la hora ingresada: TRUE -> OK o FALSE -> HORA INCORRECTA
bool validar_hora(int minutos) {
    int hora = minutos / 60; //Paso minutos a horas
    int minutos_totales = minutos % 60; //Tomo el resto como minutos totales de la hora inicial

    if (hora < 0 || hora > 23) {
        return false;
    }
    if (minutos_totales < 0 || minutos_totales > 59) {
        return false;
    }

    return true;
}

//Chequeo si el año es bisiesto, devolviendo: TRUE -> es bisiesto o FALSE -> no es bisiesto
bool bisiesto(int anio) {
    if ((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0)) {
        return true;
    } 
    return false;
}

//Valido la fecha, devolviendo TRUE -> si día, mes o año cumplen con los parametros establecidos o FALSE -> si no cumplen
bool validar_fecha(int dia, int mes, int anio) {
    if (dia < 1 || dia > 31) {
        return false;
    }
    if (mes < 1 || mes > 12) {
        return false;
    }

    if (bisiesto(anio) == true) {
        if (mes == 2 && dia > 29) {
            return false;
        }
    } else {
        if (mes == 2 && dia > 28) {
            return false;
        } else if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) {
            return false;
        } else if ((mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12) && dia > 31) {
            return false;
        }
    }

    return true;
}
