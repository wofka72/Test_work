#pragma once
#include <vector>

/**
 * \brief Применяет алгоритм линеаризации со скользящим окном к заданному файлу.
 *                    Выводит линеаризованные данные в отдельный файл в столбец.
 * \param inputFilename Название файла, в котором заданы моменты PTS
 * \param outputFilename Название файла для вывода линеаризованных моментов PTS
 */
void linearizeFile(const char* inputFilename, const char* outputFilename);

/**
 * \brief Применяет алгоритм линеаризации со скользящим окном к vector'у моментов Presentation Timestamp.
 *        Одномерная регрессия описана на сайте
 *        http://www.machinelearning.ru/wiki/index.php?title=Линейная_регрессия_(пример).
 *
 * \param source Отсортированное по возрастанию множество моментов PTS
 * \param width Ширина окна в линеаризации
 * \returns вектор линеаризиванных данных
 */
std::vector<int> linearizeDataEvenWidth(const std::vector<int>& source, int width);

/**
 * \brief Просчитывает матрицу (A' * A)^(-1) 
 * \param width Ширина окна в линеаризации
 * \returns матрицу (A' * A)^(-1)
 */
double** const getInverseSquareA(const int width);

/**
 * \brief Заполняет матрицу A для алгоритма одномерной линейной регрессии
 * \param width Ширина окна в линеаризации
 * \returns матрицу A
 */
int** const getMatrixA(const int width);