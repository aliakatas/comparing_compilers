#include "CmdParser.h"

#include <string>

// Define the command line options here: { "number of row and columns", "number of rows", "number of columns", "number of repetitions",
//                                         "timestep", "tolerance", "pct of points with BC", "test type" }
std::string options[] = { "-n", "-r", "-c", "-rep", "-dt", "-tol", "-bc", "-type" };

/**/
void parseArguments(int argc, char** argv, RunConfiguration& rc)
{
	int iarg = 0;
	while (iarg < argc)
	{
		for (auto i = 0; i < size(options); ++i)
		{
			if (options[i].compare(argv[iarg]) == 0)
				setRunConfigurationParameter(options[i], argv[++iarg], rc);
		}
		++iarg;
	}
}

/**/
void setRunConfigurationParameter(const std::string istr_, const std::string val, RunConfiguration& rc)
{
	std::string istr = istr_;

	if (istr.compare("-n") == 0)
	{
		rc.ncols = std::stoi(val);
		rc.nrows = std::stoi(val);
	}
	else if (istr.compare("-r") == 0)
		rc.nrows = std::stoi(val);
	else if (istr.compare("-c") == 0)
		rc.ncols = std::stoi(val);
	else if (istr.compare("-rep") == 0)
		rc.reps = std::stoi(val);
	else if (istr.compare("-dt") == 0)
		rc.dt = std::stod(val);
	else if (istr.compare("-tol") == 0)
		rc.tolerance = std::stod(val);
	else if (istr.compare("-bc") == 0)
	{
		double temp = std::stod(val);
		rc.pctBC = temp >= MAX_PCT_FOR_BC ? MAX_PCT_FOR_BC : temp <= MIN_PCT_FOR_BC ? MIN_PCT_FOR_BC : temp;
	}
	else if (istr.compare("-type") == 0)
	{
		int tempType = std::stoi(val);
		switch (tempType)
		{
		case (0): {
			rc.testType = TestType::SIMPLE;
			break;
		}
		case (1): {
			rc.testType = TestType::SPECIAL;
			break;
		}
		default: {
			rc.testType = TestType::FULL;
			break;
		}
		}
	}
}












//
///**/
//void setBClocations(RunConfiguration& rc)
//{
//	size_t npointsX = 3;
//	if (rc.ncols > 1000)
//		npointsX = 90;
//	else if (rc.ncols > 100)
//		npointsX = 9;
//
//	size_t npointsY = 2;
//	if (rc.nrows > 1000)
//		npointsY = 30;
//	else if (rc.nrows > 100)
//		npointsY = 6;
//
//	rc.npoints = npointsX * npointsY;
//
//	rc.idxCol = new size_t[rc.npoints];
//	rc.idxRow = new size_t[rc.npoints];
//	size_t idx = 0;
//	for (auto i = 0; i < npointsY; ++i)
//	{
//		for (auto j = 0; j < npointsX; ++j)
//		{
//			rc.idxCol[idx] = rc.ncols * (float)j / (float)npointsX;
//			rc.idxRow[idx] = rc.nrows * (float)i / (float)npointsY;
//			++idx;
//		}
//	}
//}