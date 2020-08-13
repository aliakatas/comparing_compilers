#pragma once

#include <string>

#define MAX_PCT_FOR_BC  0.75
#define MIN_PCT_FOR_BC  0.0

enum class TestType
{
	FULL,
	MULT,
	TRIGON,
	TANH, 
	SPECIAL
};

// Keeping the configuration tidy - default values
struct RunConfiguration
{
#ifdef _DEBUG
	size_t nrows = 500;
	size_t ncols = 840;
	size_t reps = 23;
#else
	size_t nrows = 1500;
	size_t ncols = 3000;
	size_t reps = 120;
#endif
	double dt = 0.01; 
	double tolerance = 0.0001;

	double pctBC = 0.0;
	size_t npoints = 0;
	size_t* idxRow = nullptr;
	size_t* idxCol = nullptr;

	TestType testType = TestType::MULT;
};

// Parse user inputs from the command-line and modify the configuration of the program.
bool parseArguments(int argc, char** argv, RunConfiguration& rc);

// Modify a specific configuration parameter according to user input.
void setRunConfigurationParameter(const std::string istr_, const std::string val, RunConfiguration& rc);

// Confirm (or maybe just inform) with the user the test configuration
void confirmRunConfiguration(RunConfiguration& rc);

