# Control de Semáforo para Tráfico Vehicular y Peatonal

Este código en C++ simula un sistema de control de semáforo para el tráfico vehicular y peatonal. Utiliza hilos y semáforos para coordinar la simulación del cambio de luz del semáforo, el comportamiento de los autos y peatones, y la impresión de mensajes en la consola.

## Dependencias

El programa utiliza las siguientes bibliotecas estándar de C++:

- `<iostream>`: Para la entrada y salida estándar.
- `<thread>`: Para la gestión de hilos.
- `<mutex>`: Para la sincronización mediante mutex (no utilizado en la versión final).
- `<chrono>`: Para el manejo de intervalos de tiempo.
- `<vector>`: Para la gestión de colecciones de hilos.
- `<semaphore.h>`: Para la implementación de semáforos.

## Estructura del Programa

- **Mutex y Variables Compartidas**: Se utiliza un mutex y una variable compartida `green_light` para sincronizar el cambio de luz del semáforo.

- **Descriptores de Tubería**: Se crean descriptores de tubería para la comunicación entre carros y peatones.

- **Semáforos**: Se utilizan semáforos para sincronizar el cambio de luz del semáforo (`semaphore_light`) y la impresión de mensajes en la consola (`semaphore_print`).

- **Función `traffic_light`**: Simula el cambio de luz del semáforo cada 5 segundos.

- **Funciones `car` y `pedestrian`**: Simulan el comportamiento de autos y peatones, respectivamente.

- **Función `main`**: Solicita al usuario el número de carros y peatones, inicializa semáforos, crea tuberías, y lanza hilos para el cambio de luz y comportamiento de carros y peatones.

## Compilación y Ejecución

Para compilar el programa, se puede utilizar un compilador de C++ estándar. Por ejemplo:

```bash
g++ -o traffic_light_control traffic_light_control.cpp -lpthread
```

Para ejecutar el programa:

```bash
./traffic_light_control
```

## Entrada de Usuario

El usuario debe ingresar el número de carros y peatones cuando se le solicite.

---

**Nota:** Este código está diseñado con fines educativos y de simulación. La implementación puede variar según el entorno y la plataforma.