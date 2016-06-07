#include <thread>
#include "functions.h"

using std::thread;

/**
 * Применяет алгоритм линеаризации со скользящим окном к каждому из данных файлов в отдельном потоке
 */
int main() {
	thread work1(linearizeFile, "data1.csv", "data1.csv.out");
	thread work2(linearizeFile, "data2.csv", "data2.csv.out");
	work1.join();
	work2.join();

	return 0;
}