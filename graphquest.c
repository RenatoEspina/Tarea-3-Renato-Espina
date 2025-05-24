#include "tdas/extra.h"
#include "tdas/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

// Estructuras
typedef struct {
    int id;
    bool final;
    char nombre[18];
    char descripcion[140];
    int arrVecinos[4]; // indices: 0=arriba, 1=abajo, 2=izquierda, 3=derecha
    List* objetos;
} piso;

typedef struct {
    char nombre[14];
    int puntos;
    int peso;
    bool recogido; //para reiniciar inventario
    int pisoRecogido; //para reiniciar piso
    int pisoActual; //para reiniciar piso
} objeto;

typedef struct {
    char nombre[20];
    int actual; 
    List* inventario;
    int pesoTotal;
    int puntajeTotal;
    int tiempo;
} jugador;

bool Basecargada = false; // booleano global usado para saber si se cargo la base

void titulo() {
    printf("========================================\n");
    printf("              GraphQuest               \n");
    printf("========================================\n");
}

// Menu
void mostrarMenu() {
    titulo();
    printf("1) Cargar Juego\n");
    printf("2) Jugar!!!\n");
    printf("3) Salir\n");
}

void mostrarMenuJuego() {
    printf("1) Recoger Ítem(s)\n");
    printf("2) Descartar Ítem(s)\n");
    printf("3) Avanzar en una Dirección\n");
    printf("4) Reiniciar Partida\n");
    printf("5) Salir del Juego\n");
}

// Funciones para inicializar estructuras
void inicializarJugador(jugador* jugador) {
    jugador->actual = 0;
    jugador->pesoTotal = 0;
    jugador->puntajeTotal = 0;
    jugador->tiempo = 30;
    jugador->inventario = list_create();
}

void inicializarPisos(piso* pisos) {
    for (int i = 0; i < 16; i++) {
        pisos[i].id = 0;
        pisos[i].final = false;
        for(size_t j=0;j<4;j++) pisos[i].arrVecinos[j] = -1;
        pisos[i].nombre[0]= '\0';
        pisos[i].descripcion[0] = '\0';
        pisos[i].objetos = list_create();
    }
}

// Vuelve todo a su estado inicial
void reiniciar(jugador* jugador, List* objetos, piso* pisos) {
    jugador->actual = 0;
    jugador->pesoTotal = 0;
    jugador->puntajeTotal = 0;
    jugador->tiempo = 15;
    list_clean(jugador->inventario);
    objeto* obj;
    while ((obj = list_popFront(objetos)) != NULL) {
        if(obj->recogido) {
            obj->recogido = false;
            list_pushBack(pisos[obj->pisoRecogido].objetos, obj); 
        }
        if(obj->pisoActual != obj->pisoRecogido) {
            list_pushBack(pisos[obj->pisoRecogido].objetos, obj);
            list_remove(pisos[obj->pisoActual].objetos, obj);
            obj->pisoActual = obj->pisoRecogido;
        }
    }

}

// Calculo del tiempo
int calcularTiempo(int pesoTotal) {
    int tiempo = (pesoTotal + 1) / 10;
    return tiempo;
}

// Funcion de lectura
void leer_pisos(piso* pisos, List* objetos) {
    limpiarPantalla();
    titulo();
    printf("Cargando base de datos...\n");
    FILE *archivo = fopen("DataSet/graphquest.csv", "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    char **campos;
    campos = leer_linea_csv(archivo, ','); // Leer encabezados

    int i = 0;
    while (i < 16 && (campos = leer_linea_csv(archivo, ',')) != NULL) {

        //informacion del piso
        pisos[i].id = atoi(campos[0]);
        strcpy(pisos[i].nombre, campos[1]);
        strcpy(pisos[i].descripcion, campos[2]);

        //objetos de piso
        List* items = split_string(campos[3], ";");
        for (char *item = list_first(items); item != NULL; item = list_next(items)) {
            List* values = split_string(item, ",");
            char* item_name = list_first(values);
            int item_value = atoi(list_next(values));
            int item_weight = atoi(list_next(values));
            objeto* obj = malloc(sizeof(objeto));
            strcpy(obj->nombre, item_name);
            obj->puntos = item_value;
            obj->peso = item_weight;
            obj->pisoRecogido = i;
            obj->pisoActual = i;
            obj->recogido = false;
            list_pushBack(pisos[i].objetos, obj);
            list_pushBack(objetos, obj);
            list_clean(values);
            free(values);
        }
        list_clean(items);
        free(items);

//vecinos
        int arriba = atoi(campos[4]);
        int abajo = atoi(campos[5]);
        int izquierda = atoi(campos[6]);
        int derecha = atoi(campos[7]);
        if (arriba != -1) {
            pisos[i].arrVecinos[0] = arriba-1;
        }
        if (abajo != -1) {
            pisos[i].arrVecinos[1] = abajo-1;
        }
        if (izquierda != -1) {
            pisos[i].arrVecinos[2] = izquierda-1;
        }
        if (derecha != -1) {
            pisos[i].arrVecinos[3] = derecha-1;
        }

        if (strcmp(campos[8], "Si") == 0) pisos[i].final = true;
        i++;
    }

    Basecargada = true;
    printf("Base de datos cargada con exito\n");
    fclose(archivo);
}

// Funcion para imprimir
void mostrarEstado(jugador* j, piso* pisos) {
    piso* actual = &pisos[j->actual];

    printf("========================================\n");
    printf("Escenario actual: %s\n", actual->nombre);
    printf("Descripción: %s\n", actual->descripcion);
    printf("\n");

    if(actual->objetos->head==NULL) printf("No hay objetos en este escenario.\n");
    else{
        printf("Objetos disponibles:\n");
        for (objeto* obj = list_first(actual->objetos); obj != NULL; obj = list_next(actual->objetos)) printf("  - %s (%d pts, %d kg)\n", obj->nombre, obj->puntos, obj->peso);
    }

    printf("\nTiempo restante: %d\n", j->tiempo);

    printf("\nInventario:\n");
    for (objeto* obj = list_first(j->inventario); obj != NULL; obj = list_next(j->inventario)) printf("  - %s (%d pts, %d kg)\n", obj->nombre, obj->puntos, obj->peso);

    printf("Peso total: %d kg\n", j->pesoTotal);
    printf("Puntaje total: %d pts\n", j->puntajeTotal);

    printf("\nDirecciones disponibles:\n");
    char* direcciones[] = {"Arriba", "Abajo", "Izquierda", "Derecha"};
    for (int i = 0; i < 4; i++) {
        if (actual->arrVecinos[i] != -1) printf("  %s -> %s\n", direcciones[i], pisos[actual->arrVecinos[i]].nombre);
    }
    printf("========================================\n");
}

// Funcion para jugar
void jugar(jugador* j, piso* pisos, List* objetos) {
    char opcion;
    while (true) {
        limpiarPantalla();
        mostrarEstado(j, pisos);
        mostrarMenuJuego();
        printf("Seleccione una opción: ");
        scanf(" %c", &opcion);
        limpiarLineas(6); //borra el menu para mas claridad

        if (j->tiempo <= 0) {
            printf("¡Se acabó el tiempo! Has perdido.\n");
            return;
        }

        switch (opcion) {
            case '1': { // Recoger ítems
                if (list_size(pisos[j->actual].objetos) == 0) {
                    printf("No hay ítems para recoger.\n");
                    break;
                }

                printf("Ítems disponibles:\n");
                int index = 0;
                for (objeto* obj = list_first(pisos[j->actual].objetos); obj != NULL; obj = list_next(pisos[j->actual].objetos)) {
                    index++;
                    printf("%d) %s (%d pts, %d kg)\n", index, obj->nombre, obj->puntos, obj->peso);
                }

                printf("Seleccione el índice del ítem a recoger (-1 para cancelar): ");
                int eleccion;
                scanf("%d", &eleccion);
                eleccion -= 1;
                if (eleccion >= 0 && eleccion < list_size(pisos[j->actual].objetos)) {
                    objeto* obj = list_get(pisos[j->actual].objetos, eleccion);
                    list_pushBack(j->inventario, obj); 
                    j->pesoTotal += obj->peso;
                    j->puntajeTotal += obj->puntos;
                    obj->recogido = true;
                    list_remove(pisos[j->actual].objetos, obj); 
                    j->tiempo -= 1;
                    printf("Ítem recogido!\n");
                }
                break;
            }

            case '2': { // Descartar ítems
                if (list_size(j->inventario) == 0) {
                    printf("Inventario vacío.\n");
                    break;
                }

                printf("Inventario actual:\n");
                int index = 0;
                for (objeto* obj = list_first(j->inventario); obj != NULL; obj = list_next(j->inventario)) {
                    index++;
                    printf("%d) %s (%d pts, %d kg)\n", index, obj->nombre, obj->puntos, obj->peso);
                }

                printf("Seleccione el índice del ítem a descartar (-1 para cancelar): ");
                int eleccion;
                scanf("%d", &eleccion);
                eleccion -= 1; 
                if (eleccion >= 0 && eleccion < list_size(j->inventario)) {
                    objeto* obj = list_get(j->inventario, eleccion);
                    j->pesoTotal -= obj->peso;
                    j->puntajeTotal -= obj->puntos;
                    obj->recogido = false;
                    obj->pisoActual = j->actual;
                    list_pushBack(pisos[j->actual].objetos, obj);
                    list_remove(j->inventario, obj);
                    j->tiempo -= 1;
                    printf("Ítem descartado en el escenario!\n");
                }
                break;
            }

            case '3': { // Moverse
                printf("Direcciones:\n");
                char* direcciones[] = {"Arriba", "Abajo", "Izquierda", "Derecha"};
                for (int i = 0; i < 4; i++) {
                    if (pisos[j->actual].arrVecinos[i] != -1)
                        printf("%d) %s -> %s\n", i + 1, direcciones[i], pisos[pisos[j->actual].arrVecinos[i]].nombre);
                }

                printf("Seleccione dirección (1-4): ");
                int dir;
                scanf("%d", &dir);
                dir -= 1;

                if (dir >= 0 && dir < 4 && pisos[j->actual].arrVecinos[dir] != -1) {
                    int nuevo = pisos[j->actual].arrVecinos[dir]; //esto funciona porque "pisos" esta ordenado por id y "arrVecinos" guarda id-1
                    j->actual = nuevo;
                    int costo = calcularTiempo(j->pesoTotal);
                    if (pisos[j->actual].final) {
                        limpiarPantalla();
                        printf("¡Has llegado al escenario final!\n");
                        printf("Inventario final:\n");
                        for (objeto* obj = list_first(j->inventario); obj != NULL; obj = list_next(j->inventario))
                            printf("  - %s (%d pts, %d kg)\n", obj->nombre, obj->puntos, obj->peso);
                        printf("Puntaje final: %d pts\n", j->puntajeTotal);
                        printf("Tiempo restante: %d\n", j->tiempo);
                        reiniciar(j, objetos, pisos);
                        return;
                    }
                    if(costo==0) costo = 1;
                    if (costo > j->tiempo) {
                        printf("No tienes suficiente tiempo para moverte.\n");
                        break;
                    }
                    j->tiempo -= costo;
                } else {
                    printf("Dirección inválida.\n");
                }
                break;
            }

            case '4': // Reiniciar
                reiniciar(j, objetos, pisos);
                printf("Juego reiniciado.\n");
                break;

            case '5': //salir
                reiniciar(j, objetos, pisos);
                return;

            default:
                printf("Opción inválida.\n");
        }

        presioneTeclaParaContinuar();
    }
}

void limpiar(jugador* jugador, List* objetos, piso* pisos) {
    list_clean(objetos);
    list_clean(jugador->inventario);
    free(objetos);
    free(jugador->inventario);
    for (int i = 0; i < 16; i++) free(pisos[i].objetos);
    free(pisos);
    free(jugador);
}

// Main
int main() {
    limpiarPantalla();
    SetConsoleOutputCP(CP_UTF8);

    jugador jugador1;
    inicializarJugador(&jugador1);
    piso pisos[16];
    inicializarPisos(pisos);
    List *objetos = list_create();
    char opcion;
    do {
        mostrarMenu();
        printf("Ingrese su opcion: ");
        scanf(" %c", &opcion);

        switch (opcion){
        case '1':
            if(!Basecargada) leer_pisos(pisos, objetos);
            else printf("Ya se ha cargado la base de datos\n");
            break;

        case '2':
            if (!Basecargada) printf("Primero debe cargar la base de datos.\n");
            else {
                jugar(&jugador1, pisos, objetos);
            }
            break;
        }
        presioneTeclaParaContinuar();
        limpiarPantalla();
    } while (opcion != '3');

    limpiar(&jugador1,objetos,pisos);
    printf("programa cerrado con exito!!");
    return 0;
}