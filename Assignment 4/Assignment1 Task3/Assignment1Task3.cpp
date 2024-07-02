#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions

// C++ standard library and STL headers
#include <iostream>
#include <vector>
#include <fstream>


// OpenCL header, depending on OS
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

#include "common.h"


#define LENGTH 513

int main(void) {
	cl::Platform platform;			// device's platform
	cl::Device device;				// device used
	cl::Context context;			// context for the device
	cl::Program program;			// OpenCL program object
	cl::Kernel kernel;				// a single kernel object
	cl::CommandQueue queue;			// commandqueue for a context and device
	unsigned int i;
	int inputMultiplier; //store user's input

	// declare data and memory objects
	std::vector<cl_uint> intVectorA(LENGTH); //host side data obj

	//Create Buffer
	cl::Buffer bufferOne, resultBuffer;	//device side data obj

	//Get user's input of multiplier.
	std::cout << std::endl;
	std::cout << "Assignment 1 Task 3: " << std::endl;
	std::cout << "Please enter a multipler between (2 and 99): " << std::endl;
	std::cin >> inputMultiplier;
	//Check input
	while (2) {
		if (std::cin.fail() || inputMultiplier >= 99) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');

			std::cout << "Please enter a valid integer between 2 and 99! " << std::endl;
			std::cin >> inputMultiplier;
		}
		else {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			break;
		}
	}
	std::cout << "You have input Multiplier of: " << inputMultiplier << std::endl;


	try {
		//select OpenCL device
		if (!select_one_device(&platform, &device))
		{
			quit_program("Device not selcted");
		}

		context = cl::Context(device);

		if (!build_program(&program, &context, "task3.cl"))
		{
			quit_program("OpenCL program build error");
		}

		//create kernel
		kernel = cl::Kernel(program, "fillArray");
		//create command queue
		queue = cl::CommandQueue(context, device);
		//create buffers
		bufferOne = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(cl_uint) * LENGTH, &intVectorA[0]);
		//set kernel arguments
		kernel.setArg(0, inputMultiplier);
		kernel.setArg(1, bufferOne);

		queue.enqueueNDRangeKernel(kernel, cl::NDRange(0), cl::NDRange(LENGTH));

		std::cout << "Kernel enqueued" << std::endl;
		std::cout << "------------------" << std::endl;

		queue.enqueueReadBuffer(bufferOne, CL_TRUE, 0, sizeof(cl_uint) * LENGTH, &intVectorA[0]);

		//output
		std::cout << "\nContents of bufferOne: " << std::endl;
		for (int i = 1; i < LENGTH; i++) {
			std::cout << intVectorA[i] << " ";
		}
	}
	catch (cl::Error err) {
		handle_error(err);
	}
}