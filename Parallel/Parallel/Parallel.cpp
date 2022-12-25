#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <fstream>

using namespace std;

int** mallocMatrix(int n, int m) {
    int** matrix = (int**)malloc(sizeof(int*) * n);

    for (int i = 0; i < n; i++) {
        matrix[i] = (int*)malloc(sizeof(int) * m);
    }

    return matrix;
}

int** randomMatrix(int n, int m) {
    int** matrix = mallocMatrix(n, m);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix[i][j] = rand() % 11;
        }
    }

    return matrix;
}

void matrixMulti(int** matrix1, int n1, int m1, int** matrix2, int n2, int m2, int threadsNum) {

    if (m1 != n2) {
        cout << "Ошибка размерностей матриц" << endl;
        exit(0);
    }

    int** result = mallocMatrix(n1, m2);

    omp_set_num_threads(threadsNum);

    int i, j, k;
    #pragma omp parallel for shared(matrix1, matrix2, result) private (i, j, k)
    for (i = 0; i < n1; i++) {
        for (j = 0; j < m2; j++) {
            result[i][j] = 0;
            for (k = 0; k < m1; k++) {
                result[i][j] += (matrix1[i][k] * matrix2[k][j]);
            }
        }
    }
}

int main(int argc, char** argv) {

    setlocale(LC_ALL, "Russian");

    const int n1 = 1500;
    const int m1 = 1500;

    const int n2 = 1500;
    const int m2 = 1500;

    srand(time(NULL));

    int** matrix1 = randomMatrix(n1, m1);
    int** matrix2 = randomMatrix(n2, m2);

    double start = omp_get_wtime();
    
    //Перебор потоков
    for (int i = 1; i < 9; i++)
    {
        //Расчет времени
        auto start = omp_get_wtime();
        matrixMulti(matrix1, n1, m1, matrix2, n2, m2, i);
        double end = omp_get_wtime();
        double time = end - start;

        cout << "Время работы (" << i << "): " << time << " секунд" << endl;
    }

    return 0;
}