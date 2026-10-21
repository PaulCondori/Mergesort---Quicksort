#include <iostream>
using namespace std;

void quickSort(int arreglo[], int inicio, int fin) {
    int i = inicio;
    int j = fin;
    int pivote = arreglo[(inicio + fin) / 2];
    int auxiliar;

    while (i <= j) {
        while (arreglo[i] < pivote) {
            i++;
        }

        while (arreglo[j] > pivote) {
            j--;
        }

        if (i <= j) {
            auxiliar = arreglo[i];
            arreglo[i] = arreglo[j];
            arreglo[j] = auxiliar;

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

int main() {
    int arreglo[1000];
    int n;

    do {
        cout << "Cantidad de elementos (1-1000): ";
        cin >> n;

        if (n <= 0 || n > 1000) {
            cout << "Cantidad no valida. Intente nuevamente." << endl;
        }
    } while (n <= 0 || n > 1000);
    
    cout << endl;

    cout << "Ingrese los elementos:" << endl;

    for (int i = 0; i < n; i++) {
        cin >> arreglo[i];
    }

    quickSort(arreglo, 0, n - 1);

    cout << "Arreglo ordenado: ";

    for (int i = 0; i < n; i++) {
        cout << arreglo[i] << " ";
    }

    cout << endl;

    return 0;
}
