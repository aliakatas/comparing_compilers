#include "CmdParser.h"

#include <iostream>
#include <string>

// Define the command line options here: 
std::string optionsExplained[] = { "number of row and columns (square domain)", "number of rows", "number of columns", "number of repetitions", 
	"timestep", "tolerance", "pct of points with BC", 
	"test type (0: Multiplication only, 1: Power and Trigonometric, 2: Trigonometric only, 3: Hyperbolic Tangent, Any other value: Full test" };
std::string options[] = { "-n", "-r", "-c", "-rep", "-dt", "-tol", "-bc", "-type" };

/**/
bool parseArguments(int argc, char** argv, RunConfiguration& rc)
{
	const std::string helpOpt = "-h";
	int iarg = 0;
	while (iarg < argc)
	{
		for (auto i = 0; i < size(options); ++i)
		{
			if (helpOpt.compare(argv[iarg]) == 0)
			{
				std::cout << " Help on the available options: \n";
				for (auto jj = 0; jj < size(options); ++jj)
				{
					std::cout << "        " << options[jj] << " :: " << optionsExplained[jj] << "\n";
				}
				std::cout << " Terminating now... \n";
				return false;
			}

			if (options[i].compare(argv[iarg]) == 0)
				setRunConfigurationParameter(options[i], argv[++iarg], rc);
		}
		++iarg;
	}
	return true;
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
			rc.testType = TestType::MULT;
			break;
		}
		case (1): {
			rc.testType = TestType::SPECIAL;
			break;
		}
		case (2): {
			rc.testType = TestType::TRIGON;
			break;
		}
		case (3): {
			rc.testType = TestType::TANH;
			break;
		}
		default: {
			rc.testType = TestType::FULL;
			break;
		}
		}
	}
}

/**/
void confirmRunConfiguration(RunConfiguration& rc)
{
	std::cout << "  Running Test with --> \n";
	std::cout << "            # Rows :: " << rc.nrows << "\n";
	std::cout << "         # Columns :: " << rc.ncols << "\n";
	std::cout << "          # Tsteps :: " << rc.reps << "\n";
	std::cout << "     Time step val :: " << rc.dt << "\n";
	std::cout << "         Tolerance :: " << rc.tolerance << "\n";
	rc.npoints = rc.pctBC * rc.nrows * rc.ncols; 
	std::cout << "  # Points with BC :: " << rc.npoints << "\n";
	std::cout << "         Test type :: ";
	switch (rc.testType)
	{
	case (TestType::MULT): {
		std::cout << "Multiplication \n";
		break;
	}
	case (TestType::SPECIAL): {
		std::cout << "Special \n";
		break;
	}
	case (TestType::FULL): {
		std::cout << "Full \n";
		break;
	}
	case (TestType::TRIGON): {
		std::cout << "Trigonometric \n";
		break;
	}
	case (TestType::TANH): {
		std::cout << "Hyp Tangent \n";
		break;
	}
	default:
	{
		std::cout << "** not recognised ** \n";
		break;
	}
	}
}

