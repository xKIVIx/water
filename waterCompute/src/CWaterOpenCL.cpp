/* Copyright (c) 2018, Aleksandrov Maksim */

#include <iostream>
#include <fstream>
#include <algorithm>

#include "CL\cl.h"
#include "CSettingsManager.hpp"

#include "CWaterOpenCL.hpp"

#define SOURCE_PROGRAM_FILE "program.cl"

#define getSittingParametr(name, out) CSettingsManager::instance().getParametr("waterCompute", name, out)
#define setSittingParametr(name, out) CSettingsManager::instance().setParametr("waterCompute", name, out)

#define errorMessage(message, failCode) std::cout << message << ": " << failCode << std::endl

void chek( const cl_int  stat ) {
    if ( stat != CL_SUCCESS ) {
        errorMessage("Fail", stat);
        throw stat;
    }
}

void _stdcall pfn_notify(const char *errinfo,
                const void *private_info,
                size_t cb,
                void *user_data) {
    std::cout << errinfo;
}

CWaterOpenCL::CWaterOpenCL() {
    std::cout << "-------Init OpenCl-------\n";
    std::cout << "Init platform\n";
    cl_uint countPlatforms = 0;
    chek(clGetPlatformIDs(0,
                          nullptr,
                          &countPlatforms));
    cl_platform_id * platfroms = new cl_platform_id[countPlatforms];
    chek(clGetPlatformIDs(countPlatforms,
                          platfroms,
                          &countPlatforms));
    int i = 0;
    if(!getSittingParametr("OpenCL_platform_id", i)) {
        std::cout << "Select default platform\n";
        setSittingParametr("OpenCL_platform_id", 0);
        i = 0;
    }
    if(cl_uint(i) >= countPlatforms) {
        std::cout << "Uncurrect platform\nSelect default platform\n";
        setSittingParametr("OpenCL_platform_id", 0);
        i = 0;
    }
    // view name of selected platform
    cl_platform_id idSelectedPlatform = platfroms[i];
    char name[256];
    cl_uint nameSize = 0;
    chek(clGetPlatformInfo(idSelectedPlatform,
                           CL_PLATFORM_NAME,
                           256,
                           name,
                           &nameSize));
    std::cout << "Platform: " << name << std::endl;

    // init device
    std::cout << "Init device"<< std::endl;
    cl_uint countDevices = 0;
    chek(clGetDeviceIDs(idSelectedPlatform,
                        CL_DEVICE_TYPE_GPU,
                        0,
                        nullptr,
                        &countDevices));
    cl_device_id * devices = new cl_device_id[countDevices];
    chek(clGetDeviceIDs(idSelectedPlatform,
                        CL_DEVICE_TYPE_GPU,
                        countDevices,
                        devices,
                        &countDevices));
    if(!getSittingParametr("OpenCL_device_id", i)) {
        std::cout << "Select default device\n";
        setSittingParametr("OpenCL_device_id", 0);
        i = 0;
    }
    if(cl_uint(i) >= countDevices) {
        std::cout << "Uncurrect device\nSelect default device\n";
        setSittingParametr("OpenCL_device_id", 0);
        i = 0;
    }
    cl_device_id idSelectedDevice = devices[i];
    chek(clGetDeviceInfo(idSelectedDevice,
                         CL_DEVICE_NAME,
                         256,
                         name,
                         &nameSize));
    std::cout << "Device: " << name << std::endl;

    // create context
    std::cout << "Create context\n";
    cl_int err = 0;
    context_ = clCreateContext(NULL,
                               1,
                               &idSelectedDevice,
                               pfn_notify,
                               nullptr,
                               &err);
    chek(err);
    // create command queue
    std::cout << "Create command queue\n";
    commandQueue_ = clCreateCommandQueue((cl_context)context_,
                                         idSelectedDevice,
                                         0,
                                         &err);
    chek(err);


    // read .cl file
    std::cout << "Read .cl file\n";
    std::ifstream file(SOURCE_PROGRAM_FILE);
    if(!file.is_open()) {
        std::cout << "Can`t open OpenCl.cl\n";
        return;
    }
    std::string script;
    while(!file.eof()) {
        char tmp[256];
        file.getline(tmp, 256);
        script += tmp;
        script += '\n';
    }

    // compile prgram
    std::cout << "Compile program\n";
    const char * tmp = script.c_str();
    size_t sizeScript =  script.size();
    program_ = clCreateProgramWithSource((cl_context)context_,
                                         1,
                                         &tmp,
                                         &sizeScript,
                                         &err);
    chek(err);
    err = clBuildProgram((cl_program)program_,
                         1,
                         &idSelectedDevice,
                         NULL,
                         NULL,
                         NULL);
    if(err != CL_SUCCESS) {
        errorMessage("Fail build program", err);
        size_t endLog = 0;
        clGetProgramBuildInfo((cl_program)program_,
                              idSelectedDevice,
                              CL_PROGRAM_BUILD_LOG,
                              1000,
                              nullptr,
                              &endLog);
        char * log = new char[endLog];
        clGetProgramBuildInfo((cl_program)program_,
                              idSelectedDevice,
                              CL_PROGRAM_BUILD_LOG,
                              endLog,
                              log,
                              &endLog);
        log[endLog - 1] = '\0';
        std::cout << log << std::endl;
        delete[] log;
        throw err;
    }

    // init kernels
    std::cout << "Init kernels\n";
    err = initKernels();
    if(err != CL_SUCCESS) {
        errorMessage("Fail init kernels", err);
        throw err;
    }
    std::cout << "----------OpenCl is success init---------\n";
}

int CWaterOpenCL::initKernels() {
    int err;
    err = kernelFindEdges.setFunction(program_,
                                      "findEdges");
    if(err != CL_SUCCESS) {
        errorMessage("Fail init findEdges", err);
        return err;
    }

    err = kernelFindInnerEdges.setFunction(program_,
                                           "findInnerEdges");
    if(err != CL_SUCCESS) {
        errorMessage("Fail init findInnerEdges", err);
        return err;
    }

    err = kernelFindBorderEdges.setFunction(program_,
                                            "findBorderEdges");
    if(err != CL_SUCCESS) {
        errorMessage("Fail init findBorderEdges", err);
        return err;
    }

    return CL_SUCCESS;
}

CWaterOpenCL::~CWaterOpenCL() {
    //
    clReleaseProgram((cl_program)program_);
    clReleaseCommandQueue((cl_command_queue)commandQueue_);
    clReleaseContext((cl_context)context_);
}

int CWaterOpenCL::loadData(const std::vector<float>& vertex, 
                           const std::vector<uint32_t>& face) {
    int err;
    err = bufferVertex_.setFlag(context_,
                                CL_MEM_READ_ONLY);
    err = bufferVertex_.loadData(context_,
                                 commandQueue_,
                                 (char*)vertex.data(),
                                 vertex.size() * sizeof(float));
    if(err != CL_SUCCESS) {
        errorMessage("Fail load vertex", err);
        return err;
    }
    err = bufferFaces_.setFlag(context_,
                               CL_MEM_READ_ONLY);
    err = bufferFaces_.loadData(context_,
                                commandQueue_,
                                (char*)face.data(),
                                face.size() * sizeof(uint32_t));
    
    if(err != CL_SUCCESS) {
        errorMessage("Fail load faces", err);
        return err;
    }
    return computeData();
}

int CWaterOpenCL::getInnerFaces(std::vector<uint32_t>& face) const
{
    std::vector<uint32_t> result;
    int err = bufferInnerEdgeFaces_.getData(commandQueue_, result);
    result.resize(countInnerEdges_ * 2);
    if(err != CL_SUCCESS) {
        return err;
    }
    std::sort(result.begin(), result.end());
    face.reserve(countInnerEdges_ * 2);
    char counter = 0;
    uint32_t last = UINT32_MAX;
    for(auto iter = result.begin(); iter != result.end(); ++iter) {
        if(*iter == last) {
            counter++;
            if(counter == 2) {
                face.push_back(*iter);
                counter = 0;
            }
        } else {
            counter = 0;
        }
        last = *iter;
    }
    return CL_SUCCESS;
}

int CWaterOpenCL::getBorderFaces(std::vector<uint32_t>& face) const {
    return bufferBorderEdgeFaces_.getData(commandQueue_, face);
}

int CWaterOpenCL::computeData() {
    int err;
    std::cout << "Find edges\n";
    err = kernelFindEdges.bindParametr(bufferFaces_, 0);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind faces", err);
        return err;
    }
    CMemObject bufferEdges(context_,
                           bufferFaces_.getSize() * 2,
                           CL_MEM_READ_WRITE);
    err = kernelFindEdges.bindParametr(bufferEdges, 1);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind result edges buffer", err);
        return err;
    }

    uint32_t countFace = bufferFaces_.getSize() / sizeof(uint32_t);
    err = kernelFindEdges.complite(commandQueue_,
                                   &countFace,
                                   1);
    if(err != CL_SUCCESS) {
        errorMessage("Fail complite findEdges kernel", err);
        return err;
    }
    
    CMemObject bufferMarkNoneBorder((cl_context)context_,
                                    bufferEdges.getSize() / (2 * sizeof(uint32_t)),
                                    CL_MEM_READ_WRITE);
    err = computeInnerEdges(bufferEdges,
                            bufferMarkNoneBorder);
    if(err != CL_SUCCESS) {
        errorMessage("Fail compute inner edges", err);
        return err;
    }

    err = computeBorderEdges(bufferEdges,
                             bufferMarkNoneBorder);
    if(err != CL_SUCCESS) {
        errorMessage("Fail compute border edges", err);
        return err;
    }

    return CL_SUCCESS;
}

int CWaterOpenCL::computeInnerEdges(const CMemObject &bufferEdges,
                                    const CMemObject &bufferMarkNoneBorder) {
    std::cout << "Find inner edges\n";
    int err;
    err = kernelFindInnerEdges.bindParametr(bufferEdges, 0);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind faces", err);
        return err;
    }

    err = bufferInnerEdges_.resize((cl_context)context_, bufferEdges.getSize());
    if(err != CL_SUCCESS) {
        errorMessage("Fail set memmory for inner edges", err);
        return err;
    }
    err = kernelFindInnerEdges.bindParametr(bufferInnerEdges_, 1);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind inner edges buffer", err);
        return err;
    }

   
    err = kernelFindInnerEdges.bindParametr(bufferMarkNoneBorder, 2);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind mark none border buffer", err);
        return err;
    }

    CMemObject bufferCountInnerEdges((cl_context)context_,
                                     sizeof(uint32_t),
                                     CL_MEM_READ_WRITE);
    err = kernelFindInnerEdges.bindParametr(bufferCountInnerEdges, 3);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind count inner edges buffer", err);
        return err;
    }

    bufferInnerEdgeFaces_.resize((cl_context)context_, bufferEdges.getSize());
    if(err != CL_SUCCESS) {
        errorMessage("Fail set memmory for inner edges", err);
        return err;
    }
    err = kernelFindInnerEdges.bindParametr(bufferInnerEdgeFaces_, 4);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind inner edges buffer", err);
        return err;
    }
    uint32_t workSizes[] = {bufferEdges.getSize() / (2 * sizeof(uint32_t)),
                            bufferEdges.getSize() / (2 * sizeof(uint32_t))};
    err = kernelFindInnerEdges.complite(commandQueue_,
                                        workSizes,
                                        2);
    if(err != CL_SUCCESS) {
        errorMessage("Fail complite findInnerEdges kernel", err);
        return err;
    }

    err = bufferCountInnerEdges.getData(commandQueue_, countInnerEdges_);
    if(err != CL_SUCCESS) {
        errorMessage("Fail get count inner edges", err);
        return err;
    }
    err = clFinish((cl_command_queue)commandQueue_);
    if(err != CL_SUCCESS) {
        errorMessage("Fail wait finish", err);
        return err;
    }
    return CL_SUCCESS;
}

int CWaterOpenCL::computeBorderEdges(const CMemObject &bufferEdges,
                                     const CMemObject &bufferMarkNoneBorder) {
    std::cout << "Find border edges\n";
    int err;
    err = kernelFindBorderEdges.bindParametr(bufferEdges, 0);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind faces", err);
        return err;
    }
    err = kernelFindBorderEdges.bindParametr(bufferMarkNoneBorder, 1);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind mark none border buffer", err);
        return err;
    }
    
    countBorderEdges_ = bufferEdges.getSize() / (2 * sizeof(uint32_t)) - countInnerEdges_ * 2;
    
    err = bufferBorderEdges_.resize(context_, 
                                    countBorderEdges_ * 2 * sizeof(uint32_t));
    if(err != CL_SUCCESS) {
        errorMessage("Fail init border edges buffer", err);
        return err;
    }
    err = kernelFindBorderEdges.bindParametr(bufferBorderEdges_, 2);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind border edges buffer", err);
        return err;
    }

    err = bufferBorderEdgeFaces_.resize(context_, 
                                        countBorderEdges_ * sizeof(uint32_t));
    if(err != CL_SUCCESS) {
        errorMessage("Fail init border face buffer", err);
        return err;
    }
    err = kernelFindBorderEdges.bindParametr(bufferBorderEdgeFaces_, 3);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind border face buffer", err);
        return err;
    }

    CMemObject bufferCountBorderEdges((cl_context)context_,
                                      sizeof(uint32_t),
                                      CL_MEM_READ_WRITE);
    err = kernelFindBorderEdges.bindParametr(bufferCountBorderEdges, 4);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind count inner edges buffer", err);
        return err;
    }

    uint32_t workSize = bufferEdges.getSize() / (2 * sizeof(uint32_t));
    err = kernelFindBorderEdges.complite(commandQueue_, &workSize, 1);
    return CL_SUCCESS;
}
