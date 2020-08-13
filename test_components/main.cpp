#include "cpu_utilities.h"
#include "CmdParser.h"
#include "helper_funcs.h"

#include <iostream>

int main(int argc, char** argv)
{
	std::string thisExeName = argv[0];
	std::cout << " Hello from  -- " << get_filename(thisExeName) << " -- \n";
	std::cout << " Run " << get_filename(thisExeName) << " -h for various options and change test configuration... \n";

	RunConfiguration myRC;

	if (!parseArguments(argc, argv, myRC))
		return 0;

	confirmRunConfiguration(myRC);
	prepareBCpoints(myRC.npoints, myRC.idxRow, myRC.idxCol, myRC.nrows, myRC.ncols);

	if (myRC.testType == TestType::FULL || myRC.testType == TestType::MULT)
		cpu_benchmark(myRC.nrows, myRC.ncols, myRC.dt, myRC.reps, myRC.idxRow, myRC.idxCol, myRC.npoints, true, MathUsed::USE_MULT);
	
	if (myRC.testType == TestType::FULL || myRC.testType == TestType::SPECIAL)
		cpu_benchmark(myRC.nrows, myRC.ncols, myRC.dt, myRC.reps, myRC.idxRow, myRC.idxCol, myRC.npoints, true, MathUsed::USE_POW);

	if (myRC.testType == TestType::FULL || myRC.testType == TestType::SPECIAL || myRC.testType == TestType::TRIGON)
		cpu_benchmark(myRC.nrows, myRC.ncols, myRC.dt, myRC.reps, myRC.idxRow, myRC.idxCol, myRC.npoints, true, MathUsed::USE_TRIG);

	if (myRC.testType == TestType::FULL || myRC.testType == TestType::TANH)
		cpu_benchmark(myRC.nrows, myRC.ncols, myRC.dt, myRC.reps, myRC.idxRow, myRC.idxCol, myRC.npoints, true, MathUsed::USE_TANH);

	std::cout << " Goodbye!\n\n";
	return 0;
}