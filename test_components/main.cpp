#include "cpu_utilities.h"
#include <iostream>

void get_random_ints(const size_t N, size_t* idx, const size_t minVal, const size_t maxVal)
{
	for (auto i = 0; i < N; ++i)
		idx[i] = minVal + (rand() % static_cast<size_t>(maxVal - minVal + 1));
}

int main(int argc, char** argv)
{

	std::cout << " Hello from  -- test_components.exe -- \n";
#ifdef _DEBUG
	const size_t nrows = 500;
	const size_t ncols = 840;
	const double dt = 0.01;
	const size_t reps = 23;
	const size_t np = 60;
	size_t idxRows[np]{};
	size_t idxCols[np]{};
#else
	const size_t nrows = 1500;
	const size_t ncols = 3000;
	const double dt = 0.01;
	const size_t reps = 120;
	const size_t np = 600;
	size_t idxRows[np]{};
	size_t idxCols[np]{};
#endif
	get_random_ints(np, idxRows, 0, nrows - 1);
	get_random_ints(np, idxCols, 0, ncols - 1);

	cpu_benchmark(nrows, ncols, dt, reps, idxRows, idxCols, np, true);
	cpu_benchmark(nrows, ncols, dt, reps, idxRows, idxCols, np, true, MathUsed::USE_POW);
	cpu_benchmark(nrows, ncols, dt, reps, idxRows, idxCols, np, true, MathUsed::USE_TANH);

	std::cout << " Goodbye!\n\n";
	return 0;
}