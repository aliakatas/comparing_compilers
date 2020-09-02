#include "MessageLogger.h"
#include "gpu_utilities.h"

#include <cuda_runtime.h>
#include <cuda.h>
#include <iostream>

extern "C" GPULIBRARY_API bool deviceQuery()
{
	int deviceCount = 0;
	cudaError_t error_id = cudaGetDeviceCount(&deviceCount);

	if (error_id != cudaSuccess) {
		std::cout << "  cudaGetDeviceCount returned " << static_cast<int>(error_id) << "\n  -> " << cudaGetErrorString(error_id) << "\n";
		std::cout << "  Result = FAIL\n";
		return false;
	}

	// This function call returns 0 if there are no CUDA capable devices.
	if (deviceCount == 0)
		std::cout << "  There are no available device(s) that support CUDA\n";
	else
		std::cout << "  Detected " << deviceCount << " CUDA Capable device(s)\n";

	int driverVersion = 0, runtimeVersion = 0;

	for (auto dev = 0; dev < deviceCount; ++dev)
	{
		cudaSetDevice(dev);
		cudaDeviceProp deviceProp;
		cudaGetDeviceProperties(&deviceProp, dev);

		std::cout << "\n  Device " << dev << ": \" " << deviceProp.name  << "\" \n";

		// Console log
		cudaDriverGetVersion(&driverVersion);
		cudaRuntimeGetVersion(&runtimeVersion);
		std::cout << "    CUDA Driver Version / Runtime Version          " << driverVersion / 1000 
			<< "." << (driverVersion % 100) / 10 << " / " << runtimeVersion / 1000 << "." << (runtimeVersion % 100) / 10 << "\n";
		std::cout << "    CUDA Capability Major/Minor version number:    " << deviceProp.major << "." << deviceProp.minor << "\n";
	}
	return true;
}

extern "C" GPULIBRARY_API void calculateOnGPU_f(float* xpy, const float* x, const float* y, const size_t rows, const size_t cols, const float dt, const size_t rep,
	const size_t* idxRow, const size_t* idxCol, const size_t nBC)
{
	float* d_temp = nullptr; 
	float* d_xpy = nullptr;
	float* d_x = nullptr;
	float* d_y = nullptr;
	size_t* d_idxRow = nullptr;
	size_t* d_idxCol = nullptr;

	cudaError_t allocError = cudaMalloc((void**)&d_temp, rows * cols * sizeof(float));
	if (allocError != cudaSuccess)
	{
		std::cout << "  cudaMalloc (temp) returned " << static_cast<int>(allocError) << "\n  -> " << cudaGetErrorString(allocError) << "\n";
		return;
	}

	allocError = cudaMalloc((void**)&d_xpy, rows * cols * sizeof(float));
	if (allocError != cudaSuccess)
	{
		std::cout << "  cudaMalloc (xpy) returned " << static_cast<int>(allocError) << "\n  -> " << cudaGetErrorString(allocError) << "\n";
		return;
	}

	allocError = cudaMalloc((void**)&d_x, rows * cols * sizeof(float));
	if (allocError != cudaSuccess)
	{
		std::cout << "  cudaMalloc (x) returned " << static_cast<int>(allocError) << "\n  -> " << cudaGetErrorString(allocError) << "\n";
		return;
	}

	allocError = cudaMalloc((void**)&d_y, rows * cols * sizeof(float));
	if (allocError != cudaSuccess)
	{
		std::cout << "  cudaMalloc (y) returned " << static_cast<int>(allocError) << "\n  -> " << cudaGetErrorString(allocError) << "\n";
		return;
	}

	allocError = cudaMalloc((void**)&d_idxRow, rows * cols * sizeof(size_t));
	if (allocError != cudaSuccess)
	{
		std::cout << "  cudaMalloc (idxRow) returned " << static_cast<int>(allocError) << "\n  -> " << cudaGetErrorString(allocError) << "\n";
		return;
	}

	allocError = cudaMalloc((void**)&d_idxCol, rows * cols * sizeof(size_t));
	if (allocError != cudaSuccess)
	{
		std::cout << "  cudaMalloc (idxCol) returned " << static_cast<int>(allocError) << "\n  -> " << cudaGetErrorString(allocError) << "\n";
		return;
	}
	
	cudaMemset(d_temp, 0, rows * cols * sizeof(float));
	cudaMemset(d_xpy, 0, rows * cols * sizeof(float));
	cudaMemcpy(d_x, x, rows * cols * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_y, y, rows * cols * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_idxRow, idxRow, nBC * sizeof(size_t), cudaMemcpyHostToDevice);
	cudaMemcpy(d_idxCol, idxCol, nBC * sizeof(size_t), cudaMemcpyHostToDevice);

	float time = 0.0f;
	for (auto i = 0; i < rep; ++i)
	{
		/*addWithCPU <real>(temp, x, y, rows, cols, dt);
		modifyWithCPU <real>(xpy, x, y, temp, rows, cols, dt, mathused);
		if (nBC > 0)
			applyBC <real>(xpy, rows, cols, time, idxRow, idxCol, nBC);
		memcpy(temp, xpy, rows * cols * sizeof(real));*/
		time += dt;
	}
	// This is not doing anything fancy. Just keeps the GPU busy for some time.

	cudaFree(d_temp);
	cudaFree(d_xpy);
	cudaFree(d_x);
	cudaFree(d_y);
}

extern "C" GPULIBRARY_API void calculateOnGPU_d(double* xpy, const double* x, const double* y, const size_t rows, const size_t cols, const double dt, const size_t rep,
	const size_t * idxRow, const size_t * idxCol, const size_t nBC)
{
	double* d_temp = nullptr;
	double* d_xpy = nullptr;
	double* d_x = nullptr;
	double* d_y = nullptr;
	size_t* d_idxRow = nullptr;
	size_t* d_idxCol = nullptr;

	cudaError_t allocError = cudaMalloc((void**)&d_temp, rows * cols * sizeof(double));
	if (allocError != cudaSuccess)
	{
		std::cout << "  cudaMalloc (temp) returned " << static_cast<int>(allocError) << "\n  -> " << cudaGetErrorString(allocError) << "\n";
		return;
	}

	allocError = cudaMalloc((void**)&d_xpy, rows * cols * sizeof(double));
	if (allocError != cudaSuccess)
	{
		std::cout << "  cudaMalloc (xpy) returned " << static_cast<int>(allocError) << "\n  -> " << cudaGetErrorString(allocError) << "\n";
		return;
	}

	allocError = cudaMalloc((void**)&d_x, rows * cols * sizeof(double));
	if (allocError != cudaSuccess)
	{
		std::cout << "  cudaMalloc (x) returned " << static_cast<int>(allocError) << "\n  -> " << cudaGetErrorString(allocError) << "\n";
		return;
	}

	allocError = cudaMalloc((void**)&d_y, rows * cols * sizeof(double));
	if (allocError != cudaSuccess)
	{
		std::cout << "  cudaMalloc (y) returned " << static_cast<int>(allocError) << "\n  -> " << cudaGetErrorString(allocError) << "\n";
		return;
	}

	allocError = cudaMalloc((void**)&d_idxRow, rows * cols * sizeof(size_t));
	if (allocError != cudaSuccess)
	{
		std::cout << "  cudaMalloc (idxRow) returned " << static_cast<int>(allocError) << "\n  -> " << cudaGetErrorString(allocError) << "\n";
		return;
	}

	allocError = cudaMalloc((void**)&d_idxCol, rows * cols * sizeof(size_t));
	if (allocError != cudaSuccess)
	{
		std::cout << "  cudaMalloc (idxCol) returned " << static_cast<int>(allocError) << "\n  -> " << cudaGetErrorString(allocError) << "\n";
		return;
	}

	cudaMemset(d_temp, 0, rows * cols * sizeof(double));
	cudaMemset(d_xpy, 0, rows * cols * sizeof(double));
	cudaMemcpy(d_x, x, rows * cols * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(d_y, y, rows * cols * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(d_idxRow, idxRow, nBC * sizeof(size_t), cudaMemcpyHostToDevice);
	cudaMemcpy(d_idxCol, idxCol, nBC * sizeof(size_t), cudaMemcpyHostToDevice);

	float time = 0.0f;
	for (auto i = 0; i < rep; ++i)
	{
		/*addWithCPU <real>(temp, x, y, rows, cols, dt);
		modifyWithCPU <real>(xpy, x, y, temp, rows, cols, dt, mathused);
		if (nBC > 0)
			applyBC <real>(xpy, rows, cols, time, idxRow, idxCol, nBC);
		memcpy(temp, xpy, rows * cols * sizeof(real));*/
		time += dt;
	}
	// This is not doing anything fancy. Just keeps the GPU busy for some time.

	cudaFree(d_temp);
	cudaFree(d_xpy);
	cudaFree(d_x);
	cudaFree(d_y);
}