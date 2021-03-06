#include "cpu_utilities.h"
#include "CmdParser.h"
#include "helper_funcs.h"
#include "gpu_utilities.h"

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
	if (myRC.gpuOn) {
#ifndef WIN32
		if (!deviceQuery())
			myRC.gpuOn = false;
#else
		myRC.gpuOn = false;
#endif
	}
	prepareBCpoints(myRC.npoints, myRC.idxRow, myRC.idxCol, myRC.nrows, myRC.ncols);

	if (myRC.testType == TestType::FULL || myRC.testType == TestType::MULT)
	{
		benchmark(myRC.nrows, myRC.ncols, myRC.dt, myRC.reps, myRC.idxRow, myRC.idxCol, myRC.npoints, true, MathUsed::USE_MULT, ProcType::CPU);
		if (myRC.gpuOn)
			benchmark(myRC.nrows, myRC.ncols, myRC.dt, myRC.reps, myRC.idxRow, myRC.idxCol, myRC.npoints, true, MathUsed::USE_MULT, ProcType::GPU);
	}
		
	if (myRC.testType == TestType::FULL || myRC.testType == TestType::SPECIAL)
	{
		benchmark(myRC.nrows, myRC.ncols, myRC.dt, myRC.reps, myRC.idxRow, myRC.idxCol, myRC.npoints, true, MathUsed::USE_POW, ProcType::CPU);
		if (myRC.gpuOn)
			benchmark(myRC.nrows, myRC.ncols, myRC.dt, myRC.reps, myRC.idxRow, myRC.idxCol, myRC.npoints, true, MathUsed::USE_POW, ProcType::GPU);
	}

	if (myRC.testType == TestType::FULL || myRC.testType == TestType::SPECIAL || myRC.testType == TestType::TRIGON)
	{
		benchmark(myRC.nrows, myRC.ncols, myRC.dt, myRC.reps, myRC.idxRow, myRC.idxCol, myRC.npoints, true, MathUsed::USE_TRIG, ProcType::CPU);
		if (myRC.gpuOn)
			benchmark(myRC.nrows, myRC.ncols, myRC.dt, myRC.reps, myRC.idxRow, myRC.idxCol, myRC.npoints, true, MathUsed::USE_TRIG, ProcType::GPU);
	}

	if (myRC.testType == TestType::FULL || myRC.testType == TestType::TANH)
	{
		benchmark(myRC.nrows, myRC.ncols, myRC.dt, myRC.reps, myRC.idxRow, myRC.idxCol, myRC.npoints, true, MathUsed::USE_TANH, ProcType::CPU);
		if (myRC.gpuOn)
			benchmark(myRC.nrows, myRC.ncols, myRC.dt, myRC.reps, myRC.idxRow, myRC.idxCol, myRC.npoints, true, MathUsed::USE_TANH, ProcType::GPU);
	}
		
	std::cout << " Goodbye!\n\n";
	return 0;
}