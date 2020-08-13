#include "cpu_utilities.h"
#include "CmdParser.h"

#include <iostream>

int main(int argc, char** argv)
{
	std::cout << " Hello from  -- test_components.exe -- \n";

	RunConfiguration myRC;

	parseArguments(argc, argv, myRC);
	confirmRunConfiguration(myRC);
	prepareBCpoints(myRC.npoints, myRC.idxRow, myRC.idxCol, myRC.nrows, myRC.ncols);

	/*if (myRC.testType == TestType::FULL || myRC.testType == TestType::MULT)
		cpu_benchmark(myRC.nrows, myRC.ncols, myRC.dt, myRC.reps, myRC.idxRow, myRC.idxCol, myRC.npoints, true, MathUsed::USE_MULT);
	
	if (myRC.testType == TestType::FULL || myRC.testType == TestType::SPECIAL)
		cpu_benchmark(myRC.nrows, myRC.ncols, myRC.dt, myRC.reps, myRC.idxRow, myRC.idxCol, myRC.npoints, true, MathUsed::USE_POW);

	if (myRC.testType == TestType::FULL || myRC.testType == TestType::SPECIAL || myRC.testType == TestType::TRIGON)
		cpu_benchmark(myRC.nrows, myRC.ncols, myRC.dt, myRC.reps, myRC.idxRow, myRC.idxCol, myRC.npoints, true, MathUsed::USE_TRIG);

	if (myRC.testType == TestType::FULL || myRC.testType == TestType::TANH)
		cpu_benchmark(myRC.nrows, myRC.ncols, myRC.dt, myRC.reps, myRC.idxRow, myRC.idxCol, myRC.npoints, true, MathUsed::USE_TANH);*/

	std::cout << " Goodbye!\n\n";
	return 0;
}