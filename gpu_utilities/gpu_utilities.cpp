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