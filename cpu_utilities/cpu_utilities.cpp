#include "cpu_utilities.h"
#include "MessageLogger.h"

void benchmark(const size_t nrows, const size_t ncols,
    const double dt, const size_t reps, const size_t* idxRow, const size_t* idxCol, const size_t npoints, const bool logfile, 
    const MathUsed mathused, const ProcType procType)
{
    MessageLogger messenger(true);
    if (logfile)
    {
        std::string logName;
        switch (procType)
        {
        case (ProcType::CPU): {
            logName += "cpu_";
            break;
        }
        case (ProcType::GPU): {
            logName += "gpu_";
            break;
        }
        default:
            break;
        }
        
        logName = "benchmark_";
        switch (mathused)
        {
        case (MathUsed::USE_MULT): {
            logName += "mult_";
            break;
        }
        case (MathUsed::USE_POW): {
            logName += "pow_";
            break;
        }
        case (MathUsed::USE_TANH): {
            logName += "tanh_";
            break;
        }
        case (MathUsed::USE_TRIG): {
            logName += "trig_";
            break;
        }
        default: {
            logName += "mult_";
            break;
        }
        }

#ifdef WIN32
        logName += "32bit_";
#else
        logName += "64bit_";
#endif
#ifdef __INTEL_COMPILER
        logName += "ICC";
#else
        logName += "MSVC";
#endif
        logName += ".log";
        messenger.addFile(logName);
    }

    const size_t bytesPerMatrixSingle = nrows * ncols * sizeof(float);
    const size_t bytesPerMatrixDouble = nrows * ncols * sizeof(double);

    // Show some info on the work to be done...
    messenger.writeOutput();
    std::string tempMessage = "                      ";
    switch (procType)
    {
    case (ProcType::CPU): {
        tempMessage += "CPU ";
        break;
    }
    case (ProcType::GPU): {
        tempMessage += "GPU ";
        break;
    }
    default:
        break;
    }
    tempMessage += "Benchmark ";
#ifdef WIN32
    tempMessage += "32bit ";
#else
    tempMessage += "64bit ";
#endif
    tempMessage += "              ";
    messenger.writeOutput(tempMessage);

    messenger.writeOutput(" ---------------------------------------------------------- ");
    std::string tempStr;
    tempStr = "            Rows x Columns :: " + std::to_string(nrows) + " x " + std::to_string(ncols);
    messenger.writeOutput(tempStr);
    tempStr = " Total Elements per matrix :: " + std::to_string(nrows * ncols);
    messenger.writeOutput(tempStr);
    tempStr = " Bytes per matrix (single) :: " + std::to_string(bytesPerMatrixSingle) + " Bytes | " +
        to_string_with_precision(double(bytesPerMatrixSingle) / 1024.0, 2) + " KB | " + to_string_with_precision(double(bytesPerMatrixSingle) / 1024.0 / 1024.0, 3) + " MB";
    messenger.writeOutput(tempStr);
    tempStr = " Bytes per matrix (double) :: " + std::to_string(bytesPerMatrixDouble) + " Bytes | " +
        to_string_with_precision(double(bytesPerMatrixDouble) / 1024.0, 2) + " KB | " + to_string_with_precision(double(bytesPerMatrixDouble) / 1024.0 / 1024.0, 3) + " MB";
    messenger.writeOutput(tempStr);
    tempStr = "               Repetitions :: " + std::to_string(reps);
    messenger.writeOutput(tempStr);
    switch (mathused)
    {
    case (MathUsed::USE_MULT): {
        messenger.writeOutput("                 Math Used :: Multiplication ");
        break;
    }
    case (MathUsed::USE_POW): {
        messenger.writeOutput("                 Math Used :: Power ");
        break;
    }
    case (MathUsed::USE_TANH): {
        messenger.writeOutput("                 Math Used :: Tanh ");
        break;
    }
    case (MathUsed::USE_TRIG): {
        messenger.writeOutput("                 Math Used :: Trigonometric ");
        break;
    }
    default: {
        messenger.writeOutput("                 Math Used :: Multiplication ");
        break;
    }
    }

#ifdef _DEBUG
    const int N = 3;
#else
    const int N = 10;
#endif // _DEBUG

    messenger.writeOutput("\n");
#ifdef _DEBUG
    messenger.writeOutput("****** Debug ****** ");
#endif
#ifdef __INTEL_COMPILER
    messenger.writeOutput("  ICC compiler results");
#else
    messenger.writeOutput("  MSVC compiler results");
#endif
    messenger.writeOutput();
    const double stime = run_all_iterations <float>(N, nrows, ncols, dt, reps, idxRow, idxCol, npoints, mathused);
    messenger.writeOutput("  After " + std::to_string(N) + " iterations, the average run time for SINGLE precision is " + to_string_with_precision(stime, 3) + " (s) ");

    const double dtime = run_all_iterations <double>(N, nrows, ncols, dt, reps, idxRow, idxCol, npoints, mathused);
    messenger.writeOutput("  After " + std::to_string(N) + " iterations, the average run time for DOUBLE precision is " + to_string_with_precision(dtime, 3) + " (s) ");
    messenger.writeOutput();
}

void get_random_ints(const size_t N, size_t* idx, const size_t minVal, const size_t maxVal)
{
    for (auto i = 0; i < N; ++i)
        idx[i] = minVal + (rand() % static_cast<size_t>(maxVal - minVal + 1));
}

void prepareBCpoints(const size_t N, size_t*& idxRows, size_t*& idxCols, const size_t nrows, const size_t ncols)
{
    if (N <= 0)
        return;

    idxRows = (size_t*)malloc(N * sizeof(size_t));
    idxCols = (size_t*)malloc(N * sizeof(size_t));

    get_random_ints(N, idxRows, 0, nrows - 1);
    get_random_ints(N, idxCols, 0, ncols - 1);
}