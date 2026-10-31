#include <iostream>
using namespace std;

void combinar(int arreglo[], int inicio, int medio, int fin) {
    int auxiliar[1000];
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
        int medio = (inicio + fin) / 2;

        mergeSort(arreglo, inicio, medio);
        mergeSort(arreglo, medio + 1, fin);

        combinar(arreglo, inicio, medio, fin);
    }
}

int main() {
    int arreglo[1000];
    int n;
	
	do {
    cout << "Cantidad de elementos (1-1000): ";
    cin >> n;

    if (n <= 0 || n > 1000) {
        cout << "Cantidad no valida. Intente nuevamente..." << endl;
    }
	
	} while (n <= 0 || n > 1000);
    cout << "Ingrese los elementos:" << endl;

    for (int i = 0; i < n; i++) {
        cin >> arreglo[i];
    }

    mergeSort(arreglo, 0, n - 1);
    
    cout << endl;

    cout << "Arreglo ordenado: ";

    for (int i = 0; i < n; i++) {
        cout << arreglo[i] << " ";
    }

    cout << endl;

    return 0;
}
