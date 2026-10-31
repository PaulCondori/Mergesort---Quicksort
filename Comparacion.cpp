#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace chrono;

const int MAX = 100000;
const int REPETICIONES = 30;
const int COPIAS = 10;

static int original[MAX];
static int copiasMerge[COPIAS][MAX];
static int copiasQuick[COPIAS][MAX];
static int auxiliar[MAX];

// ==================== MERGE SORT ====================

void combinar(int arreglo[], int inicio, int medio, int fin) {
    int i = inicio;
    int j = medio + 1;
    int k = inicio;

    while (i <= medio && j <= fin) {
        if (arreglo[i] <= arreglo[j]) {
            auxiliar[k] = arreglo[i];
            i++;
        } else {
            auxiliar[k] = arreglo[j];
            j++;
        }
        k++;
    }

    while (i <= medio) {
        auxiliar[k] = arreglo[i];
        i++;
        k++;
    }

    while (j <= fin) {
        auxiliar[k] = arreglo[j];
        j++;
        k++;
    }

    for (int i = inicio; i <= fin; i++) {
        arreglo[i] = auxiliar[i];
    }
}

void mergeSort(int arreglo[], int inicio, int fin) {
    if (inicio < fin) {
        int medio = inicio + (fin - inicio) / 2;

        mergeSort(arreglo, inicio, medio);
        mergeSort(arreglo, medio + 1, fin);

        combinar(arreglo, inicio, medio, fin);
    }
}

// ==================== QUICK SORT ====================

void quickSort(int arreglo[], int inicio, int fin) {
    int i = inicio;
    int j = fin;
    int pivote = arreglo[inicio + (fin - inicio) / 2];
    int temp;

    while (i <= j) {
        while (arreglo[i] < pivote) {
            i++;
        }

        while (arreglo[j] > pivote) {
            j--;
        }

        if (i <= j) {
            temp = arreglo[i];
            arreglo[i] = arreglo[j];
            arreglo[j] = temp;

            i++;
            j--;
        }
    }

    if (inicio < j) {
        quickSort(arreglo, inicio, j);
    }

    if (i < fin) {
        quickSort(arreglo, i, fin);
    }
}

// ==================== COPIAR ARREGLO ====================

void copiar(int origen[], int destino[], int n) {
    for (int i = 0; i < n; i++) {
        destino[i] = origen[i];
    }
}

// ==================== GENERAR DATOS ====================

// Tipo 1: aleatorios
// Tipo 2: ordenados
// Tipo 3: inversos

void generarDatos(int arreglo[], int n, int tipo) {
    for (int i = 0; i < n; i++) {
        if (tipo == 1) {
            arreglo[i] = rand() % 100000;
        } else if (tipo == 2) {
            arreglo[i] = i;
        } else {
            arreglo[i] = n - i;
        }
    }
}

// ==================== VERIFICAR ORDEN ====================

bool estaOrdenado(int arreglo[], int n) {
    for (int i = 1; i < n; i++) {
        if (arreglo[i] < arreglo[i - 1]) {
            return false;
        }
    }

    return true;
}

// ==================== MEDIR MERGE SORT ====================

double medirMerge(int n) {
    steady_clock::time_point inicio;
    steady_clock::time_point fin;

    inicio = steady_clock::now();

    for (int c = 0; c < COPIAS; c++) {
        mergeSort(copiasMerge[c], 0, n - 1);
    }

    fin = steady_clock::now();

    return duration<double, milli>(fin - inicio).count()
           / COPIAS;
}

// ==================== MEDIR QUICK SORT ====================

double medirQuick(int n) {
    steady_clock::time_point inicio;
    steady_clock::time_point fin;

    inicio = steady_clock::now();

    for (int c = 0; c < COPIAS; c++) {
        quickSort(copiasQuick[c], 0, n - 1);
    }

    fin = steady_clock::now();

    return duration<double, milli>(fin - inicio).count()
           / COPIAS;
}

// ==================== PROGRAMA PRINCIPAL ====================

int main() {
    int tamanos[6] = {
        1000, 5000, 10000, 20000, 50000, 100000
    };

    double tiempoMerge;
    double tiempoQuick;

    ofstream archivo("resultados_finales.csv");

    if (!archivo.is_open()) {
        cout << "No se pudo crear el archivo CSV." << endl;
        return 1;
    }

    archivo << fixed << setprecision(8);

    archivo << "Elementos,Tipo,Repeticion,"
            << "MergeSort_ms,QuickSort_ms,Verificacion"
            << endl;

    srand(1);

    for (int t = 0; t < 6; t++) {
        int n = tamanos[t];

        cout << "\n==================================" << endl;
        cout << "CANTIDAD DE ELEMENTOS: " << n << endl;
        cout << "==================================" << endl;

        for (int tipo = 1; tipo <= 3; tipo++) {
            double sumaMerge = 0;
            double sumaQuick = 0;

            if (tipo == 1) {
                cout << "\nDatos aleatorios" << endl;
            } else if (tipo == 2) {
                cout << "\nDatos ordenados" << endl;
            } else {
                cout << "\nDatos inversos" << endl;
            }

            for (int r = 1; r <= REPETICIONES; r++) {

                // Generar los datos de entrada.
                generarDatos(original, n, tipo);

                // Preparar diez copias independientes para cada algoritmo.
                for (int c = 0; c < COPIAS; c++) {
                    copiar(original, copiasMerge[c], n);
                    copiar(original, copiasQuick[c], n);
                }

                // Alternar el orden de medicion.
                if (r % 2 == 1) {
                    tiempoMerge = medirMerge(n);
                    tiempoQuick = medirQuick(n);
                } else {
                    tiempoQuick = medirQuick(n);
                    tiempoMerge = medirMerge(n);
                }

                // Comprobar las diez copias de ambos algoritmos.
                bool correcto = true;

                for (int c = 0; c < COPIAS; c++) {
                    if (!estaOrdenado(copiasMerge[c], n) ||
                        !estaOrdenado(copiasQuick[c], n)) {
                        correcto = false;
                    }
                }

                if (!correcto) {
                    cout << "ERROR: ordenamiento incorrecto." << endl;
                    archivo.close();
                    return 1;
                }

                sumaMerge += tiempoMerge;
                sumaQuick += tiempoQuick;

                archivo << n << "," << tipo << "," << r << ","
                        << tiempoMerge << "," << tiempoQuick
                        << ",SI" << endl;
            }

            cout << fixed << setprecision(8);

            cout << "Promedio MergeSort: "
                 << sumaMerge / REPETICIONES << " ms" << endl;

            cout << "Promedio QuickSort: "
                 << sumaQuick / REPETICIONES << " ms" << endl;
        }
    }

    archivo.close();

    cout << "\nEXPERIMENTO FINALIZADO." << endl;
    cout << "Archivo creado: resultados_finales.csv" << endl;

    return 0;
}
