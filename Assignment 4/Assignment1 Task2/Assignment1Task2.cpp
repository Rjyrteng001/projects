#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions

// C++ standard library and STL headers
#include <iostream>
#include <vector>
#include <fstream>
#include <numeric>

// OpenCL header, depending on OS
#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.hpp>
#endif

#include "common.h"
#define CHARLENGTH 52
#define INTLENGTH 1023

// template to concatenate 2 vectors.
template <typename T>
std::vector<T> operator+(const std::vector<T>& A, const std::vector<T>& B) {
	std::vector<T> AB;
	AB.reserve(A.size() + B.size());                // preallocate memory
	AB.insert(AB.end(), A.begin(), A.end());        // add A;
	AB.insert(AB.end(), B.begin(), B.end());        // add B;
	return AB;
}

int main(void) {
	cl::Platform platform;			// device's platform
	cl::Device device;				// device used
	cl::Context context;			// context for the device
	cl::Program program;			// OpenCL program object
	cl::Kernel kernel;				// a single kernel object
	cl::CommandQueue queue;			// commandqueue for a context and device
	unsigned int i, j;
	unsigned int fillInt = 1; //starting integer to fill intVector

	//Create 2 vector, 1 with unsigned char of 52, another with unsigned int of 512.
	std::vector<cl_uchar> alphabets;
	std::vector<cl_uchar> alphabetsSmall(CHARLENGTH / 2);
	std::vector<cl_uchar> alphabetsLarge(CHARLENGTH / 2);
	std::vector<cl_uint> intVector;

	std::vector<cl_uchar> charOutput(CHARLENGTH);	// Char output data
	std::vector<cl_uint> intOutput(INTLENGTH);   // Int output data

	//Fill vector with small alphabets ztoa
	std::iota(alphabetsSmall.rbegin(), alphabetsSmall.rend(), 'a');
	//Fill vector with large alphabets ZtoA
	std::iota(alphabetsLarge.rbegin(), alphabetsLarge.rend(), 'A');
	//Merge the 2 vector into vector alphabets
	alphabets = alphabetsSmall + alphabetsLarge;

	//Fill vector 
	for (i = 0; i < INTLENGTH; i++) {
		intVector.push_back(fillInt);
		fillInt++;
	}

	//create three openCL memory objects
	cl::Buffer firstBuffer;
	cl::Buffer secondBuffer;
	cl::Buffer thirdBuffer;

	std::cout << "Assignment 1 Task 2: " << std::endl;
	try {
		if (!select_one_device(&platform, &device))
		{
			quit_program("Device not selected");
		}

		context = cl::Context(device);

		//read only buffer
		firstBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(cl_uchar) * alphabets.size(), &alphabets[0]);
		//write only buffer
		secondBuffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_uchar) * CHARLENGTH);
		//read and write buffer
		thirdBuffer = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_uint) * INTLENGTH);

		//create command queue
		queue = cl::CommandQueue(context, device);
		queue.enqueueCopyBuffer(firstBuffer, secondBuffer, 0, 0, sizeof(cl_uchar) * CHARLENGTH);
		queue.enqueueWriteBuffer(thirdBuffer, CL_TRUE, 0, sizeof(cl_uint) * intVector.size(), &intVector[0]);

		if (!build_program(&program, &context, "task2.cl"))
		{
			quit_program("OpenCL program build error");
		}

		//create kernel
		kernel = cl::Kernel(program, "task2");

		float a = 12.45;
		kernel.setArg(0, a);
		kernel.setArg(1, secondBuffer);
		kernel.setArg(2, thirdBuffer);

		//enqueue kernel for execution
		queue.enqueueTask(kernel);
		std::cout << "Kernel enqueued" << std::endl;
		std::cout << "--------------------" << std::endl;

		// enqueue command to read from device to host memory
		queue.enqueueReadBuffer(firstBuffer, CL_TRUE, 0, sizeof(cl_uchar) * CHARLENGTH, &charOutput[0]);

		// output contents
		std::cout << "\nContents of firstBuffer: " << std::endl;
		for (int i = 0; i < CHARLENGTH; i++) {
			std::cout << charOutput[i] << " ";
		}
		// enqueue command to read from device to host memory
		queue.enqueueReadBuffer(secondBuffer, CL_TRUE, 0, sizeof(cl_uchar) * CHARLENGTH, &charOutput[0]);

		// output contents
		std::cout << "\nContents of secondBuffer: " << std::endl;
		for (int i = 0; i < CHARLENGTH; i++) {
			std::cout << charOutput[i] << " ";
		}

		// enqueue command to read from device to host memory
		queue.enqueueReadBuffer(thirdBuffer, CL_TRUE, 0, sizeof(cl_uint) * INTLENGTH, &intOutput[0]);

		// output contents
		std::cout << "\nContents of thirdBuffer: " << std::endl;
		for (int i = 0; i < INTLENGTH; i++) {
			std::cout << intOutput[i] << " ";
		}
	}

	// catch any OpenCL function errors
	catch (cl::Error e) {
		// call function to handle errors
		handle_error(e);
	}
	return 0;
}
