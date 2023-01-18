/**
Alignrd new, taken from DXTK 
https://raw.githubusercontent.com/Microsoft/DirectXTK/master/Src/AlignedNew.h
*/

#pragma once

#include <malloc.h>
#include <exception>

/**
// Derive from this to customize operator new and delete for
// types that have special heap alignment requirements.
//
// Example usage:
//
//      __declspec(align(16)) struct MyAlignedType : public AlignedNew<MyAlignedType>
*/

template<typename TDerived> struct AlignedNew
{
	// Allocate aligned memory.
	static void* operator new (size_t size)
	{
		const size_t alignment = __alignof(TDerived);

		static_assert(alignment > 8, "AlignedNew is only useful for types with > 8 byte alignment. Did you forget a __declspec(align) on TDerived?");

		void* ptr = _aligned_malloc(size, alignment);

		if (!ptr)
			throw std::bad_alloc();

		return ptr;
	}


	// Free aligned memory.
	static void operator delete (void* ptr)
	{
		_aligned_free(ptr);
	}


	// Array overloads.
	static void* operator new[] (size_t size)
	{
		return operator new(size);
	}


	static void operator delete[] (void* ptr)
	{
		operator delete(ptr);
	}
};

