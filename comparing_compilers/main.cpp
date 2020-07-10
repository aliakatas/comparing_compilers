#include "CmdParser.h"
#include "cpu_utilities.h"
#include "Timer.h"

int main(int argc, char** argv)
{
	// Create a timer...
	Timer timer;
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


    // Declare the arrays
    real* a_h = nullptr;
    real* b_h = nullptr;
    real* c_h = nullptr;

    timer.start("CPU workload ");
    a_h = (real*)malloc(bytesPerMatrix);
    b_h = (real*)malloc(bytesPerMatrix);
    c_h = (real*)malloc(bytesPerMatrix);

    // Initialise the input matrices
    initialise(a_h, myRC.nrows, myRC.ncols);
    initialise(b_h, myRC.nrows, myRC.ncols);

    // Run work on CPU
    timer.start("CPU execution ");
    printf("Performing calculations on CPU... \n");
    calculateOnCPU(c_h, a_h, b_h, myRC.nrows, myRC.ncols, myRC.dt, myRC.reps, myRC.idxRow, myRC.idxCol, myRC.npoints);

    if (a_h)
        free(a_h);
    if (b_h)
        free(b_h);
    if (c_h)
        free(c_h);
    float total_time = timer.stop();



	return 0;
}