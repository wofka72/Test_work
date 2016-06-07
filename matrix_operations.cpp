#include "functions.h"


/**
 * \brief Заполняет матрицу A для алгоритма одномерной линейной регрессии
 * \param width Ширина окна в линеаризации
 * \returns матрицу A
 */
int** const getMatrixA(const int width) {
	int** A = new int*[2];
	A[0] = new int[width / 2 + 1];
	A[1] = new int[width / 2 + 1];

	for (int i = 0; i <= width / 2; ++i) {
		A[0][i] = 1;
		A[1][i] = i + 1;
	}
	return A;
}

/**
 * \brief Просчитывает матрицу (A' * A)^(-1) 
 * \param width Ширина окна в линеаризации
 * \returns матрицу (A' * A)^(-1)
 */
double** const getInverseSquareA(const int width) {
	int** const A = matrixA(width);
	int squareA[2][2] = { { 0, 0 },{ 0, 0 } };

	for (int i = 0; i < 2; ++i) {
		for (int k = 0; k < 2; ++k) {
			for (int j = 0; j <= width / 2; ++j) {
				squareA[i][k] += A[i][j] * A[k][j];
			}
		}
	}
	int determinant = squareA[0][0] * squareA[1][1] - squareA[0][1] * squareA[1][0];

	double** inverseASquare  = new double*[2];
	inverseASquare[0] = new double[2];
	inverseASquare[1] = new double[2];

	inverseASquare[0][0] = squareA[1][1];
	inverseASquare[0][1] = -squareA[0][1];
	inverseASquare[1][0] = squareA[0][0];
	inverseASquare[1][1] = -squareA[1][0];

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			inverseASquare[i][j] /= determinant;
		}
	}

	delete[] A[0];
	delete[] A[1];
	delete[] A;

	return inverseASquare;
}