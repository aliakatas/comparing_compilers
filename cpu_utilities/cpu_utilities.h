#pragma once
#include "Timer.h"

#include <math.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>

enum class ProcType {
    CPU,
    GPU
};

enum class MathUsed
{
    USE_TANH,
    USE_POW,
    USE_TRIG,
    USE_MULT
};

void benchmark(const size_t nrows, const size_t ncols,
    const double dt, const size_t reps, const size_t* idxRow, const size_t* idxCol, const size_t npoints, const bool logfile = false, 
    const MathUsed mathused = MathUsed::USE_MULT, const ProcType procType = ProcType::CPU);

void get_random_ints(const size_t N, size_t* idx, const size_t minVal, const size_t maxVal);

void prepareBCpoints(const size_t N, size_t*& idxRows, size_t*& idxCols, const size_t nrows, const size_t ncols);

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

// Initialise an array with random numbers.
template <class real>
void initialise(real* x, const size_t nrows, const size_t ncols, const int seed = 1)
{
    srand(seed);
    for (auto i = 0; i < nrows; ++i)
        for (auto j = 0; j < ncols; ++j)
            x[i * ncols + j] = static_cast <real>(static_cast <real> (rand()) / static_cast <real> (RAND_MAX));
}

// Perform the addition on the CPU to get baseline performance.
template <class real>
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
template <class real>
void modifyWithCPU(real* xpy, const real* x, const real* y, const real* w, const size_t rows, const size_t cols, const real dt, const MathUsed mathused = MathUsed::USE_MULT)
{
    size_t idx = 0;
    for (auto i = 0; i < rows; ++i)
    {
        for (auto j = 0; j < cols; ++j)
        {
            idx = i * cols + j;
            switch (mathused)
            {
            case (MathUsed::USE_MULT): {
                xpy[idx] = w[idx] * w[idx] + dt * (x[idx] * y[idx]) / real(2.0);
                break;
            }
            case (MathUsed::USE_POW): {
                xpy[idx] = w[idx] * w[idx] + dt * (pow(x[idx], 2) + pow(y[idx], 2)) / real(2.0);
                break;
            }
            case (MathUsed::USE_TANH): {
                xpy[idx] = w[idx] * w[idx] + dt * (tanh(x[idx]) + tanh(y[idx])) / real(2.0);
                break;
            }
            case (MathUsed::USE_TRIG): {
                xpy[idx] = w[idx] * w[idx] + dt * (sin(x[idx]) * cos(y[idx])) / real(2.0);
                break;
            }
            default:
            {
                xpy[idx] = w[idx] * w[idx] + dt * (x[idx] * y[idx]) / real(2.0);
                break;
            }
            }
        }
    }
}

// Simulate the application of boundary conditions
template <class real>
void applyBC(real* xpy, const size_t rows, const size_t cols, const real val, const size_t* idxRow, const size_t* idxCol, const size_t num)
{
    size_t idx = 0;
    for (auto i = 0; i < num; ++i)
    {
        idx = idxRow[i] * cols + idxCol[i];
        xpy[idx] = val;
    }
}

// Perform a full cycle of computations
template <class real>
void calculateOnCPU(real* xpy, const real* x, const real* y, const size_t rows, const size_t cols, const real dt, const size_t rep,
	const size_t* idxRow, const size_t* idxCol, const size_t nBC, const MathUsed mathused = MathUsed::USE_MULT)
{
    real* temp = (real*)malloc(rows * cols * sizeof(real));
    memset(temp, 0, rows * cols * sizeof(real));
    real time = real(0.0);
    for (auto i = 0; i < rep; ++i)
    {
        addWithCPU <real> (temp, x, y, rows, cols, dt);
        modifyWithCPU <real> (xpy, x, y, temp, rows, cols, dt, mathused);
        if (nBC > 0)
            applyBC <real> (xpy, rows, cols, time, idxRow, idxCol, nBC);
        memcpy(temp, xpy, rows * cols * sizeof(real));
        time += dt;
    }
    // This is not doing anything fancy. Just keeps the CPU busy for some time.

    if (temp)
        free(temp);
}

// Compare results.
template <class real>
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
template <class real>
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
            csvstream << to_string_with_precision(myArray[idx]) + ", ";
        }
        csvstream << "\n";
    }
    csvstream.close();
}

// Get one iteration of the workload
template <class real>
void run_cpu_workload_iteration (float& timing, const int iter, const int maxIter, const size_t nrows, const size_t ncols,
    const real dt, const size_t reps, const size_t* idxRow, const size_t* idxCol, const size_t npoints, const MathUsed mathused = MathUsed::USE_MULT)
{
    // Create a timer...
    Timer timer;

    // Declare the arrays
    real* a_h = nullptr;
    real* b_h = nullptr;
    real* c_h = nullptr;

    std::string tempMessage;
    if (sizeof(real) == sizeof(float))
        tempMessage = "CPU workload (single precision) ";
    else
        tempMessage = "CPU workload (double precision) ";
    tempMessage += std::to_string(iter);
    tempMessage += " / ";
    tempMessage += std::to_string(maxIter);

    timer.start(tempMessage);
    a_h = (real*)malloc(nrows * ncols * sizeof(real));
    b_h = (real*)malloc(nrows * ncols * sizeof(real));
    c_h = (real*)malloc(nrows * ncols * sizeof(real));

    // Initialise the input matrices
    initialise <real> (a_h, nrows, ncols);
    initialise <real> (b_h, nrows, ncols, 2);

    // Run work on CPU
    calculateOnCPU <real> (c_h, a_h, b_h, nrows, ncols, dt, reps, idxRow, idxCol, npoints, mathused);

    std::string csvName;
    if (sizeof(real) == sizeof(float))
        csvName = "single";
    else
        csvName = "double";
    
    csvName += "_prec_results_";
    switch (mathused)
    {
    case (MathUsed::USE_MULT): {
        csvName += "mult_";
        break;
    }
    case (MathUsed::USE_POW): {
        csvName += "pow_";
        break;
    }
    case (MathUsed::USE_TANH): {
        csvName += "tanh_";
        break;
    }
    default:
    {
        csvName += "mult_";
        break;
    }
    }

#ifdef WIN32
    csvName += "32bit";
#else
    csvName += "64bit";
#endif

#ifdef __INTEL_COMPILER
    csvName += "_ICC";
#else
    csvName += "_MSVC";
#endif
    csvName += ".csv";

    dump_to_csv(csvName, c_h, nrows, ncols);

    if (a_h)
        free(a_h);
    if (b_h)
        free(b_h);
    if (c_h)
        free(c_h);
    timing = timer.stop();
    timer.print();
}

// Repeat to get average results
template <class real>
double run_all_iterations(const size_t iters, const size_t nrows, const size_t ncols,
    const real dt, const size_t reps, const size_t* idxRow, const size_t* idxCol, const size_t npoints, const MathUsed mathused = MathUsed::USE_MULT)
{
    float temp_timing = 0.0f;
    double final_timing = 0.0;
    int iter = 0;
    while (iter < iters)
    {
        run_cpu_workload_iteration <real> (temp_timing, iter + 1, iters, nrows, ncols, dt, reps, idxRow, idxCol, npoints, mathused);
        final_timing += temp_timing;
        ++iter;
    }

    return final_timing / (double)iters;
}
