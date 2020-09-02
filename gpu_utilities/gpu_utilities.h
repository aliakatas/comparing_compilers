#pragma once
#ifdef GPULIBRARY_EXPORTS
#define GPULIBRARY_API __declspec(dllexport)
#else
#define GPULIBRARY_API __declspec(dllimport)
#endif

// Perform some basic checks that a GPU exists
extern "C" GPULIBRARY_API bool deviceQuery();

// Workload for the GPU
extern "C" GPULIBRARY_API void calculateOnGPU_f(float* xpy, const float* x, const float* y, const size_t rows, const size_t cols, const float dt, const size_t rep,
	const size_t * idxRow, const size_t * idxCol, const size_t nBC);
extern "C" GPULIBRARY_API void calculateOnGPU_d(double* xpy, const double* x, const double* y, const size_t rows, const size_t cols, const double dt, const size_t rep,
	const size_t * idxRow, const size_t * idxCol, const size_t nBC);
