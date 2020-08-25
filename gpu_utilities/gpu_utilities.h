#pragma once
#ifdef GPULIBRARY_EXPORTS
#define GPULIBRARY_API __declspec(dllexport)
#else
#define GPULIBRARY_API __declspec(dllimport)
#endif

// Perform some basic checks that a GPU exists
extern "C" GPULIBRARY_API bool deviceQuery();