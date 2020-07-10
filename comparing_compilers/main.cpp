#include "CmdParser.h"
#include "cpu_utilities.h"
#include "Timer.h"

int main(int argc, char** argv)
{
	
	// Create the configuration
	RunConfiguration myRC;
	// Modify configuration as per user input
	parseArguments(argc, argv, myRC);

    // Create some general info...
    const size_t arraySize = myRC.ncols * myRC.nrows;
    const size_t bytesPerMatrix = arraySize * sizeof(real);

    // Show some info on the work to be done...
    printf("\n");
#ifdef DP
    printf("    Running with Precision :: Double -> %u bytes / real number.\n", sizeof(real));
#else
    printf("    Running with Precision :: Single -> %u bytes / real number.\n", sizeof(real));
#endif
    printf("            Rows x Columns :: %llu x %llu \n", myRC.nrows, myRC.ncols);
    printf(" Total Elements per matrix :: %llu \n", arraySize);
    printf("          Bytes per matrix :: %llu Bytes | %f KB | %f MB \n", bytesPerMatrix, real(bytesPerMatrix) / 1024.0, real(bytesPerMatrix) / 1024.0 / 1024.0);
    printf("               Repetitions :: %u \n", myRC.reps);
    printf("                 Tolerance :: %f \n", myRC.tolerance);

    float temp_timing = 0.0f;
#ifdef _DEBUG
    const int N = 3;
#else
    const int N = 10;
#endif // _DEBUG

    double final_timing = 0.0;
    int ij = 0;
    while (ij < N)
    {
        printf("*****************************************\n");
        printf("Performing calculations on CPU  %u/%u... \n", ij + 1, N);
        run_cpu_workload_iteration(bytesPerMatrix, myRC, temp_timing);
        printf("\n");
        final_timing += temp_timing;
        ++ij;
    }

#ifdef _DEBUG
    printf("** Debug ** \n");
#endif
#ifdef _ICC
    printf("  ICC compiler \n");
#else
    printf("  MSVC compiler \n");
#endif
#ifdef DP
    printf("  Double precision \n");
#else
    printf("  Single precision \n");
#endif
    printf("  After %u iterations, the average run time is %f (s) \n", N, final_timing / (double)N);
    
	return 0;
}