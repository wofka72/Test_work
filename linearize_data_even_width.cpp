#include "functions.h"
#include <vector>

using std::vector;

/**
 * \brief Применяет алгоритм линеаризации со скользящим окном к vector'у моментов Presentation Timestamp.
 *        Одномерная регрессия описана на сайте
 *        http://www.machinelearning.ru/wiki/index.php?title=Линейная_регрессия_(пример).
 *
 * \param source Отсортированное по возрастанию множество моментов PTS
 * \param width Ширина окна в линеаризации
 * \returns вектор линеаризиванных данных
 */
vector<int> linearizeDataEvenWidth(const vector<int>& source, const int width) {
	if (width % 2 != 0) {
		return vector<int>(source);
	}

	int** const A = getMatrixA(width);
	double** const inverseASquare = getInverseSquareA(width);
	const int half = width / 2;

	vector<int> result(source.size());
	result[0] = source[0];

	int first, last;

	for (int i = 1; i < source.size(); ++i) {
		/// 
		first = -1;
		/// Текущий элемент слишком близок к левому краю -- уменьшим размер окна
		if (i - half + 1 < 0) {
			first = 0;
			last = i * 2;
		}
		/// Текущий элемент даёт лишь один вариант для окна шириной 20
		else if (i - half + 1 == 0) {
			first = 0;
			last = i * 2 + 1;
		}
		/// Текущий элемент даёт лишь один вариант для окна шириной 20
		else if (i + half == source.size()) {
			first = i - half;
			last = source.size() - 1;
		}
		/// Текущий элемент слишком близок к правому краю -- также уменьшим размер окна
		else if (i + half > source.size()) {
			first = i - (source.size() - i - 1);
			last = source.size() - 1;
		}

		if (first != -1) {
			result[i] = 0;
			for (int j = first; j <= last; ++j) {
				result[i] += source[j];
			}
			result[i] /= (last - first + 1);
		}
		else {
			/// С помощью линейной регрессии и поиском погрешности методом наименьших квадратов
			/// узнаем, какой из двух вариантов (слева 9, справа 10 либо слева 10, справа 9)
			/// более приближён к линейному 
			int resultVariant1 = 0;
			int resultVariant2 = 0;
			for (int j = -half; j < half; ++j) {
				resultVariant1 += source[i + j];
			}
			for (int j = -half + 1; j <= half; ++j) {
				resultVariant2 += source[i + j];
			}
			resultVariant1 /= width;
			resultVariant2 /= width;

			int* res1 = new int[width];
			int* res2 = new int[width];

			for (int j = 0; j < half; ++j) {
				res1[j] = res2[j] = result[i - half + j];
			}
			res1[half] = resultVariant1;
			res2[half] = resultVariant2;

			double AMultipleResult1[2] = { 0, 0 };
			double AMultipleResult2[2] = { 0, 0 };

			for (int j = 0; j < half; ++j) {
				AMultipleResult1[0] += A[0][j] * res1[j];
				AMultipleResult1[1] += A[1][j] * res1[j];

				AMultipleResult2[0] += A[0][j] * res2[j];
				AMultipleResult2[1] += A[1][j] * res2[j];
			}
			/// w[0], w[1] -- параметры из модели зависимости y[i] = w[0] + w[1] * x[i] + epsilon[i]
			double w1[2];
			double w2[2];
			for (int j = 0; j < 2; ++j) {
				w1[j] = inverseASquare[j][0] * AMultipleResult1[0] + inverseASquare[j][1] * AMultipleResult1[1];
				w2[j] = inverseASquare[j][0] * AMultipleResult2[0] + inverseASquare[j][1] * AMultipleResult2[1];
			}
			/// resApprox -- это yApprox
			double* res1Approx = new double[half + 1];
			double* res2Approx = new double[half + 1];

			for (int j = 0; j <= half; ++j) {
				res1Approx[j] = 0.0;
				res2Approx[j] = 0.0;
				for (int k = 0; k < 2; ++k) {
					res1Approx[j] += w1[k] * A[k][j];
					res2Approx[j] += w2[k] * A[k][j];
				}
			}
			/// погрешность метода наименьших квадратов
			double squareSum1 = 0.0, squareSum2 = 0.0;

			for (int j = 0; j <= half; ++j) {
				squareSum1 += (res1[j] - res1Approx[j]) * (res1[j] - res1Approx[j]);
				squareSum2 += (res2[j] - res2Approx[j]) * (res2[j] - res2Approx[j]);
			}

			if (squareSum1 < squareSum2) {
				result[i] = resultVariant1;
			}
			else {
				result[i] = resultVariant2;
			}

			delete res1, res2, res1Approx, res2Approx;
		}
	}

	delete A[0], A[1], inverseASquare[0], inverseASquare[1];
	delete A, inverseASquare;
	return result;
}