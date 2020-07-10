#include "CmdParser.h"
#include "cpu_utilities.h"
#include "definitions.h"
#include "Timer.h"

#include <math.h>
#include <stdlib.h>
#include <string>
#include <fstream>

// Repeat to get reasonable results
void run_cpu_workload_iteration(const size_t bytes, const RunConfiguration myRC, float& timing)
{
    // Create a timer...
    Timer timer;

    // Declare the arrays
    real* a_h = nullptr;
    real* b_h = nullptr;
    real* c_h = nullptr;

    timer.start("CPU workload ");
    a_h = (real*)malloc(bytes);
    b_h = (real*)malloc(bytes);
    c_h = (real*)malloc(bytes);

    // Initialise the input matrices
    initialise(a_h, myRC.nrows, myRC.ncols);
    initialise(b_h, myRC.nrows, myRC.ncols);

    // Run work on CPU
    calculateOnCPU(c_h, a_h, b_h, myRC.nrows, myRC.ncols, myRC.dt, myRC.reps, myRC.idxRow, myRC.idxCol, myRC.npoints);

    if (a_h)
        free(a_h);
    if (b_h)
        free(b_h);
    if (c_h)
        free(c_h);
    timing = timer.stop();
    timer.print();
}

// Initialise an array with random numbers.
void initialise(real* x, const size_t nrows, const size_t ncols)
{
    for (auto i = 0; i < nrows; ++i)
        for (auto j = 0; j < ncols; ++j)
            x[i * ncols + j] = static_cast <real>(static_cast <real> (rand()) / static_cast <real> (RAND_MAX));
}

// Perform the addition on the CPU to get baseline performance.
void addWithCPU(real* xpy, const real* x, const real* y, const size_t rows, const size_t cols, const real dt)
{
    size_t idx = 0;
    for (auto i = 0; i < rows; ++i)
    {
        for (auto j = 0; j < cols; ++j)
        {
            idx = i * cols + j;
            xpy[idx] = xpy[idx] + dt * (x[idx] + y[idx]);
        }
    }
}

// Perform a modification on the CPU to get baseline performance.
void modifyWithCPU(real* xpy, const real* x, const real* y, const real* w, const size_t rows, const size_t cols, const real dt)
{
    size_t idx = 0;
    for (auto i = 0; i < rows; ++i)
    {
        for (auto j = 0; j < cols; ++j)
        {
            idx = i * cols + j;
            xpy[idx] = w[idx] * w[idx] + dt * (tanh(x[idx]) + tanh(y[idx])) / real(2.0);
        }
    }
}

// Perform a full cycle of computations
void calculateOnCPU(real* xpy, const real* x, const real* y, const size_t rows, const size_t cols, const real dt, const size_t rep,
    const size_t* idxRow, const size_t* idxCol, const size_t nBC)
{
    real* temp = (real*)malloc(rows * cols * sizeof(real));
    memset(temp, 0, rows * cols * sizeof(real));
    real time = real(0.0);
    for (auto i = 0; i < rep; ++i)
    {
        addWithCPU(temp, x, y, rows, cols, dt);
        modifyWithCPU(xpy, x, y, temp, rows, cols, dt);
        if (nBC > 0)
            applyBC(xpy, rows, cols, time, idxRow, idxCol, nBC);
        memcpy(temp, xpy, rows * cols * sizeof(real));
        time += dt;
    }
    // This is not doing anything fancy. Just keeps the CPU busy for some time.

    std::string fname = "outcome";
#ifdef _ICC
    fname += "_ICC";
#else
    fname += "_MSVC";
#endif
#ifdef DP
    fname += "_DP";
#endif // DP
    fname += ".csv";

    dump_to_csv(fname, xpy, rows, cols);
    printf(" \n");
}

// Simulate the application of boundary conditions
void applyBC(real* xpy, const size_t rows, const size_t cols, const real val, const size_t* idxRow, const size_t* idxCol, const size_t num)
{
    size_t idx = 0;
    for (auto i = 0; i < num; ++i)
    {
        idx = idxRow[i] * cols + idxCol[i];
        xpy[idx] = val;
    }
}

// Compare results.
size_t check_answer(const real* ref, const real* test, const size_t nrows, const size_t ncols, const real tol)
{
    size_t idx = 0;
    size_t count = 0;
    for (auto irow = 0; irow < nrows; irow++) {
        for (auto icol = 0; icol < ncols; icol++)
        {
            idx = icol + ncols * irow;
            if (fabs(ref[idx] - test[idx]) > tol)
                ++count;
        }
    }
    return count;
}

// Save an array in csv format
void dump_to_csv(const std::string fname, const real* myArray, const size_t nrows, const size_t ncols)
{
    std::ofstream csvstream;
    csvstream.open(fname);
    size_t idx = 0;
    for (auto irow = 0; irow < nrows; ++irow)
    {
        for (auto icol = 0; icol < ncols; ++icol)
        {
            idx = irow * ncols + icol;
            csvstream << std::to_string(myArray[idx]) + ", ";
        }
        csvstream << "\n";
    }
    csvstream.close();
}