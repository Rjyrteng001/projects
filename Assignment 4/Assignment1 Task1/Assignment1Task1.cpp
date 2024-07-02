#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions

// C++ standard library and STL headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

// OpenCL header, depending on OS
#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.hpp>
#endif

// functions to handle errors
#include "error.h"

//Assignment 1 Task 1 fn prototype
cl_device_type selectDeviceType(std::string);

void createContext(std::vector<cl::Platform>&, std::vector<int>&, std::vector<cl::Device>&,
    std::vector<int>&, cl::Context&, std::vector<cl::Device>&, cl::CommandQueue&);

int main(void) {
    std::vector<cl::Platform> platforms;
    std::vector<cl::Device> devices;

    std::vector<std::vector<cl::Device>> platformDev;
    std::vector<std::pair<int, int>> options;
    //Store platform ID
    std::vector<int> platformID;
    //Store device ID
    std::vector<int> deviceID;
    //Context of device
    cl::Context contexts;
    //Device with context
    std::vector<cl::Device> deviceWContext;
    //Command queue for context and device
    cl::CommandQueue queue;
    //Program
    cl::Program program;
    //Kernels
    std::vector<cl::Kernel> allKernels;

    //Store output from getInfo
    std::string outputString;
    //Store user input
    std::string inputString;
    int outputInt;

    unsigned int a, b;
    bool validInput = false;
    //store the output of CL_DEVICE_GLOBAL_MEM_SIZE & CL_DEVICE_LOCAL_MEM_SIZE
    long globalMemorySize;
    long localMemorySize;
    //CL_DEVICE_MAX_WORK_ITEM_SIZES is in 3 dimension
    int workItem[3];

    //Device chosen by user
    cl_device_type chosenDevice;
    //Store device type
    cl_device_type deviceType;
    unsigned int optionCounter = 0;
    std::ifstream programFile("task1.cl");

    try {
        std::cout << std::endl;
        std::cout << "Assignment 1 Task 1: " << std::endl;
        bool validInput = false;

        while (!validInput) {
            std::cout << "Please enter the device you want to use (CPU/GPU): " << std::endl;
            std::getline(std::cin, inputString);
            std::transform(inputString.begin(), inputString.end(), inputString.begin(), ::toupper);
            if (inputString == "GPU" || inputString == "CPU") {
                validInput = true;
                chosenDevice = selectDeviceType(inputString);
            }
            else {
                std::cout << "Please enter a valid input (CPU/GPU)" << std::endl;
            }
        }
        //Display information
        std::cout << std::endl;
        cl::Platform::get(&platforms);
        //print all platforms on the system
        std::cout << "Number of OpenCL platforms detected: " << platforms.size() << std::endl;
        //Loop through vectors
        for (int i = 0; i < platforms.size(); i++) {
            platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &devices);
            //store available devices 
            platformDev.push_back(devices);
            //Loop through devices
            for (int j = 0; j < devices.size(); j++) {
                devices[j].getInfo(CL_DEVICE_TYPE, &deviceType);
                if (deviceType == chosenDevice) {
                    //Get platform name
                    platforms[i].getInfo(CL_PLATFORM_NAME, &outputString);
                    std::cout << "CL_PLATFORM_NAME: " << outputString << std::endl;
                    //Get Device Name
                    devices[j].getInfo(CL_DEVICE_NAME, &outputString);
                    std::cout << "CL_DEVICE_ID: " << j + 1 << std::endl;
                    std::cout << "CL_DEVICE_NAME: " << outputString << std::endl;
                    //Get Max compute units
                    devices[j].getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &outputInt);
                    std::cout << "CL_DEVICE_MAX_COMPUTE_UNITS: " << outputInt << std::endl;
                    //Get Max work group size
                    devices[j].getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &outputInt);
                    std::cout << "CL_DEVICE_MAX_WORK_GROUP_SIZE: " << outputInt << std::endl;
                    //Get Max work item dimensions
                    devices[j].getInfo(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, &outputInt);
                    std::cout << "CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS: " << outputInt << std::endl;
                    //Get Max work item sizes
                    devices[j].getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES, &workItem);
                    std::cout << "CL_DEVICE_MAX_WORK_ITEM_SIZES: " << workItem[0] << "*" << workItem[1] << "*" << workItem[2] << std::endl;
                    //Get dev global memory size
                    globalMemorySize = devices[j].getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
                    std::cout << "CL_DEVICE_GLOBAL_MEM_SIZE: " << globalMemorySize << std::endl;
                    //Get dev Local memory size
                    localMemorySize = devices[j].getInfo<CL_DEVICE_LOCAL_MEM_SIZE>();
                    std::cout << "CL_DEVICE_LOCAL_MEM_SIZE: " << localMemorySize << std::endl;
                    platformID.push_back(i);
                    deviceID.push_back(j);
                    options.push_back(std::make_pair(i, j));
                }
            }
        }
        std::cout << std::endl;
        //create context on device
        createContext(platforms, platformID, devices, deviceID, contexts, deviceWContext, queue);

        //read program source code and build the program
        if (!programFile.is_open()) {
            quit_program("File not found");
        }
        //loads and displays file contents
        std::string programString(std::istreambuf_iterator<char>(programFile),
            (std::istreambuf_iterator<char>()));
        std::cout << std::endl;
        std::cout << programString << std::endl;
        //create program source from one input string
        cl::Program::Sources source(1, std::make_pair(programString.c_str(), programString.length() + 1));
        //Create a context for the program obj
        cl::Program program(contexts, source);
        //Try to build the program. If not, display error.
        try {
            program.build(deviceWContext);
            //Output build log of context deviceS.
            for (int i = 0; i < deviceWContext.size(); i++) {
                //get the cpntext device name
                outputString = deviceWContext[i].getInfo<CL_DEVICE_NAME>();
                std::string buildLog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(deviceWContext[i]);
                //output device name
                std::cout << "Device " << outputString << " build log:" << std::endl;
                //output the build log of the program.
                std::cout << buildLog << std::endl;
            }
        }

        catch (cl::Error err) {
            if (err.err() == CL_BUILD_PROGRAM_FAILURE) {
                std::cout << err.what() << ": Failure to build program" << std::endl;
                for (int i = 0; i < deviceWContext.size(); i++) {
                    outputString = deviceWContext[i].getInfo<CL_DEVICE_NAME>();
                    std::string build_log = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(deviceWContext[i]);
                    //output device name
                    std::cout << "Device " << outputString << " build log: " << std::endl;
                    std::cout << build_log << std::endl;
                }
            }
        }

        //Display kernels in the program
        std::cout << std::endl;
        //create Kernels
        program.createKernels(&allKernels);
        //output number of Kernels
        std::cout << "Number of Kernels: " << allKernels.size() << std::endl;
        std::cout << "Kernels found: " << std::endl;
        for (int i = 0; i < allKernels.size(); i++) {
            outputString = allKernels[i].getInfo<CL_KERNEL_FUNCTION_NAME>();
            std::cout << "Kernel " << i << " : " << outputString << std::endl;
        }
    }
    catch (cl::Error err) {
        handle_error(err);
    }
}

//Selecting device
cl_device_type selectDeviceType(std::string inputString) {
    cl_device_type selectDeviceType;
    std::transform(inputString.begin(), inputString.end(), inputString.begin(), ::toupper);
    if (inputString == "GPU" || inputString == "CPU" || inputString == "ALL") {
        if (inputString == "CPU") {
            selectDeviceType = CL_DEVICE_TYPE_CPU;
        }
        else if (inputString == "GPU") {
            selectDeviceType = CL_DEVICE_TYPE_GPU;
        }
    }
    return selectDeviceType;
}

//Creating context
void createContext(std::vector<cl::Platform>& platforms, std::vector<int>& platformsID, std::vector<cl::Device>& devices,
    std::vector<int>& devicesID, cl::Context& context, std::vector<cl::Device>& contextDevice, cl::CommandQueue& queue) {
    bool validInput = false;
    std::string getInput;
    while (!validInput) {
        std::cout << "Enter one device ID to create context: " << std::endl;
        std::getline(std::cin, getInput);
        unsigned int num = std::atoi(getInput.c_str());
        if (num <= devicesID.size() && num != 0) {
            std::cout << "Creating Context for platform: " << platforms[platformsID[num - 1]].getInfo<CL_PLATFORM_NAME>() << std::endl;
            platforms[platformsID[num - 1]].getDevices(CL_DEVICE_TYPE_ALL, &devices);
            context = cl::Context(devices[devicesID[num - 1]]);
            contextDevice = context.getInfo<CL_CONTEXT_DEVICES>();
            for (unsigned int i = 0; i < contextDevice.size(); i++) {
                std::cout << "Context created for Device: " << contextDevice[i].getInfo<CL_DEVICE_NAME>() << std::endl;
            }
            queue = cl::CommandQueue(context, devices[devicesID[num - 1]]);
            std::cout << "Command Queue created for Device: " << std::endl;
            validInput = true;
        }
        else {
            std::cout << "Incorrect Input." << std::endl;
        }
    }
}