#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

// Mutex para sincronización (no se utiliza en la versión con semáforos)
mutex mtx;

// Variable compartida que representa el estado del semáforo
bool green_light = false;

// Variable global para contar peatones
int pedestrian_count = 0;

// Descriptores de tubería para la comunicación con carros y peatones
int car_pipe[2];
int pedestrian_pipe[2];

// Semáforo para sincronizar el cambio de luz del semáforo
sem_t semaphore_light;

// Semáforo para sincronizar la impresión de mensajes en la consola
sem_t semaphore_print;

// Función que simula el cambio de luz del semáforo
void traffic_light() {
    while (true) {
        // Simula un cambio de luz cada 5 segundos
        this_thread::sleep_for(chrono::seconds(5));

        // Espera el semáforo antes de cambiar la luz
        sem_wait(&semaphore_light);

        // Cambia la luz
        green_light = !green_light;
        
        // Obtiene y muestra el directorio de trabajo actual
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            cout << "Directorio actual: " << cwd << endl;
        } else {
            cerr << "Error al obtener el directorio actual" << endl;
        }

        // Libera el semáforo
        sem_post(&semaphore_light);

        // Imprime el estado actual de la luz
        sem_wait(&semaphore_print);
        cout << "Semáforo:\n";
        if (green_light) {
            cout << "[1]\n[0]\n";
        } else {
            cout << "[O]\n[1]\n";
        }
        sem_post(&semaphore_print);
    }
}

// Función que simula el comportamiento de un auto
void car(int id) {
    while (true) {
        // Espera la luz verde antes de avanzar
        sem_wait(&semaphore_light);
        if (green_light) {
            sem_post(&semaphore_light);  // Libera el semáforo antes de imprimir
            sem_wait(&semaphore_print);
            cout << "Auto " << id << ": Pasa por la luz verde\n";
            sem_post(&semaphore_print);
        } else {
            sem_post(&semaphore_light);  // Libera el semáforo sin imprimir
            sem_wait(&semaphore_print);
            cout << "Auto " << id << ": Esperando luz verde\n";
            sem_post(&semaphore_print);
        }

        // Simula un intervalo de 2 segundo entre acciones de los carros
        this_thread::sleep_for(chrono::seconds(2));
    }
}

// Función que simula el comportamiento de un peatón
void pedestrian() {
    int current_index;
    {
        lock_guard<mutex> guard(mtx);
        current_index = ++pedestrian_count;
    }

    while (true) {
        // Espera la luz roja antes de cruzar
        sem_wait(&semaphore_light);
        if (!green_light) {
            sem_post(&semaphore_light);  // Libera el semáforo antes de imprimir
            sem_wait(&semaphore_print);
            cout << "Peatón " << current_index << ": Cruza la calle\n";
            sem_post(&semaphore_print);
        } else {
            sem_post(&semaphore_light);  // Libera el semáforo sin imprimir
            sem_wait(&semaphore_print);
            cout << "Peatón " << current_index << ": Esperando luz roja para cruzar\n";
            sem_post(&semaphore_print);
        }

        // Simula un intervalo de 2 segundos entre acciones de los peatones
        this_thread::sleep_for(chrono::seconds(2));
    }
}

int main() {
    // Cambiar el directorio de trabajo a uno específico
    const char* nuevoDirectorio = "/Users/aldocovamartinez/Desktop/Semaforo/Semaforo";
    if (chdir(nuevoDirectorio) != 0) {
        cerr << "Error al cambiar el directorio de trabajo" << endl;
        return 1;
    }
    
    int num_cars, num_pedestrians;

    // Solicitar al usuario que ingrese el número de carros y peatones
    cout << "Ingrese el número de carros: ";
    cin >> num_cars;

    cout << "Ingrese el número de peatones: ";
    cin >> num_pedestrians;

    // Inicializar los semáforos
    sem_init(&semaphore_light, 0, 1);
    sem_init(&semaphore_print, 0, 1);

    // Crear tuberías para la comunicación con carros y peatones
    if (pipe(car_pipe) == -1 || pipe(pedestrian_pipe) == -1) {
        cerr << "Error al crear las tuberías\n";
        return 1;
    }

    // Crear un hilo para el cambio de luz del semáforo
    thread light_thread(traffic_light);

    // Crear hilos para los carros
    vector<thread> car_threads;
    for (int i = 0; i < num_cars; ++i) {
        car_threads.emplace_back(car, i + 1);
    }

    // Crear hilos para los peatones
    vector<thread> pedestrian_threads;
    for (int i = 0; i < num_pedestrians; ++i) {
        pedestrian_threads.emplace_back(pedestrian);
    }

    // Esperar a que el hilo de cambio de luz termine
    light_thread.join();

    // Unirse a los hilos de los carros
    for (auto &car_thread : car_threads) {
        car_thread.join();
    }

    // Unirse a los hilos de los peatones
    for (auto &pedestrian_thread : pedestrian_threads) {
        pedestrian_thread.join();
    }

    // Cerrar los descriptores de tubería
    close(car_pipe[0]);
    close(car_pipe[1]);
    close(pedestrian_pipe[0]);
    close(pedestrian_pipe[1]);

    // Destruir los semáforos
    sem_destroy(&semaphore_light);
    sem_destroy(&semaphore_print);

    return 0;
}
