#include "functions.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>

using std::vector;
using std::ifstream;
using std::ofstream;
using std::sort;

/**
 * \brief Применяет алгоритм линеаризации со скользящим окном к заданному файлу.
 *                    Выводит линеаризованные данные в отдельный файл в столбец.
 * \param inputFilename Название файла, в котором заданы моменты PTS
 * \param outputFilename Название файла для вывода линеаризованных моментов PTS
 */
void linearizeFile(const char* inputFilename, const char* outputFilename) {
	ifstream inputStream(inputFilename);
	ofstream outputStream(outputFilename);

	vector<int> ptses;
	char line[256];
	int pts;
	inputStream.getline(line, 256);

	while (!inputStream.eof()) {
		pts = atoi(strstr(line, ",") + 1);
		if (pts < 0) {
			pts = 0;
		}
		ptses.push_back(pts);

		inputStream.getline(line, 256);
	}

	sort(ptses.begin(), ptses.end());

	int width = 20;
	vector<int> linearizedPTSes = linearizeDataEvenWidth(ptses, width);

	for (int currentPTS : linearizedPTSes) {
		outputStream << currentPTS << "\n";
	}
}