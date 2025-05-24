# GRAPHQUEST

## Descripción

Graphquest es un juego que trata de la resolucion de un laberinto, la idea del juego es llegar a la salida maximizando la cantidad de puntos obtenidos sin que se te acabe el tiempo!!

## Cómo compilar y ejecutar

Este juego ha sido desarrollado en lenguaje C y puede ejecutarse fácilmente utilizando **Visual Studio Code** junto con una extensión para C/C++, como **C/C++ Extension Pack** de Microsoft. Para comenzar a jugar con el juego en tu equipo local, sigue estos pasos:

### Requisitos previos:

- Tener instalado [Visual Studio Code](https://code.visualstudio.com/).
- Instalar la extensión **C/C++** (Microsoft).
- Tener instalado un compilador de C (como **gcc**). Si estás en Windows, se recomienda instalar [MinGW](https://www.mingw-w64.org/) o utilizar el entorno [WSL](https://learn.microsoft.com/en-us/windows/wsl/).

### Pasos para compilar y ejecutar:

1. **Descarga y descomprime el** archivo `.zip` en una carpeta de tu elección.
2. **Abre el proyecto en Visual Studio Code**
    - Inicia Visual Studio Code.
    - Selecciona `Archivo > Abrir carpeta...` y elige la carpeta donde descomprimiste el proyecto.
3. **Compila el código**
    - Abre el archivo principal (por ejemplo, `graphquest.c`).
    - Abre la terminal integrada (`Terminal > Nueva terminal`).
    - En la terminal, compila el programa con el siguiente comando (ajusta el nombre si el archivo principal tiene otro nombre):
        
        ```bash
        gcc graphquest.c .\tdas\*.c -o ejecutable.exe
        ```
        
4. **Ejecuta el programa**
    - Una vez compilado, puedes ejecutar la aplicación con:
        
        ```
        ./ejecutable.exe
        ```
        

## Funcionalidades

### Funcionando correctamente:

- Todas las opciones del juego funcionan en su totalidad

### Problemas conocidos:

- si se usa el ejecutable desde visual, si achicas el tamaño de tu terminal vas a bugear los printeos en pantalla hasta que se actualize (comportamiento habitual de visual pero tener en cuenta por el tamaño del menu)

**Paso 0: Menu Principal**

Estas son las opciones del menu principal

```
========================================
              GraphQuest
========================================
1) Cargar Juego
2) Jugar!!!
3) Salir
Ingrese su opcion: 
```

**Paso 1: Cargar Juegos**

Se carga la base de datos 

```
========================================
              GraphQuest
========================================
Cargando base de datos...
Base de datos cargada con exito
Presione enter para continuar...
```

**Paso 2: Jugar!!!**

Se despliega el menu de juego el cual se compone de la informacion del escenario, la informacion del jugador y las direcciones disponibles, sin importar la opcion que se escoja la informacion no desaparecera.

```
========================================
Escenario actual: Entrada principal
Descripción: Una puerta rechinante abre paso a esta mansion olvidada por los dioses y los conserjes. El aire huele a humedad y a misterios sin resolver.

No hay objetos en este escenario.

Tiempo restante: 30

Inventario:
Peso total: 0 kg
Puntaje total: 0 pts

Direcciones disponibles:
  Abajo -> Cocina
========================================
1) Recoger Ítem(s)
2) Descartar Ítem(s)
3) Avanzar en una Dirección
4) Reiniciar Partida
5) Salir del Juego
Seleccione una opción:
```

**Recoger Item**

Se dezpliega un menu con la informacion de cada item y se da la posibilidad de seleccion

```
Ítems disponibles:
1) Cuchillo (3 pts, 1 kg)
2) Pan (2 pts, 1 kg)
Seleccione el índice del ítem a recoger (-1 para cancelar):
```

**Descartar Item**

Se dezpliega un menu con la informacion de los item del inventario y se da la posibilidad de descartar, el descarte deja el item seleccionado en el piso actual

```
Inventario actual:
1) Cuchillo (3 pts, 1 kg)
Seleccione el índice del ítem a descartar (-1 para cancelar):
```

**Avanzar en una Direccion**

Se despliega un menu con todas las direcciones disponibles para el usuario siendo 1.-arriba, 2.-abajo, 3.-izquierda y 4.-derecha
```
Direcciones:
1) Arriba -> Entrada principal
2) Abajo -> Jardin
4) Derecha -> Comedor
Seleccione dirección (1-4):
```

**Reiniciar Partida**

Se vuelve todo al punto inicial devolviendo los objetos a su piso correspondiente y reiniciando toda la informacion del jugador.

**Salir del Juego**

Se reinicia todo y te devuelve al menu principal.
