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
        std::cout << "Can`t open " << SOURCE_PROGRAM_FILE << std::endl;
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
    err = kernelFindEdges_.setFunction(program_,
                                      "findEdges");
    if(err != CL_SUCCESS) {
        errorMessage("Fail init findEdges", err);
        return err;
    }

    err = kernelFindInnerEdges_.setFunction(program_,
                                           "findInnerEdges");
    if(err != CL_SUCCESS) {
        errorMessage("Fail init findInnerEdges", err);
        return err;
    }

    err = kernelFindBorderEdges_.setFunction(program_,
                                            "findBorderEdges");
    if(err != CL_SUCCESS) {
        errorMessage("Fail init findBorderEdges", err);
        return err;
    }

    err = kernelFindFractureEdges_.setFunction(program_,
                                            "findFractureEdges");
    if(err != CL_SUCCESS) {
        errorMessage("Fail init findFractureEdges", err);
        return err;
    }

    err = kernelDeleteDoubleVert_.setFunction(program_,
                                              "deleteDoubleVert");
    if(err != CL_SUCCESS) {
        errorMessage("Fail init deleteDoubleVert", err);
        return err;
    }

    return CL_SUCCESS;
}

CWaterOpenCL::~CWaterOpenCL() {
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
    err = bufferFaces_.loadData(context_,
                                commandQueue_,
                                (char*)face.data(),
                                face.size() * sizeof(uint32_t));
    
    if(err != CL_SUCCESS) {
        errorMessage("Fail load faces", err);
        return err;
    }

    err = kernelDeleteDoubleVert_.bindParametr(bufferVertex_, 0);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind buffer vertex to kernelDeleteDoubleVert_", err);
        return err;
    }

    err = kernelDeleteDoubleVert_.bindParametr(bufferFaces_, 1);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind buffer faces to kernelDeleteDoubleVert", err);
        return err;
    }

    uint32_t workSize = face.size();
    err = kernelDeleteDoubleVert_.complite(commandQueue_,
                                           &workSize,
                                           1);
    if(err != CL_SUCCESS) {
        errorMessage("Fail complite kernelDeleteDoubleVert", err);
        return err;
    }
    return computeData();
}

int CWaterOpenCL::getInnerEdgesFaces(std::vector<uint32_t> &face) const {
    return bufferInnerEdgeFaces_.getData(commandQueue_, face);
}

int CWaterOpenCL::getBorderFaces(std::vector<uint32_t> &face) const {
    return  bufferBorderEdgeFaces_.getData(commandQueue_, face);
}

int CWaterOpenCL::getFractureEdges(std::vector <uint32_t> &edges) const {
    std::vector <uint32_t> innerEdges,
                           idsFractureEdges;
    int err = CL_SUCCESS;
    err = bufferInnerEdges_.getData(commandQueue_, innerEdges);
    if(err != CL_SUCCESS) {
        return err;
    }
    err = bufferIdsFractureEdges_.getData(commandQueue_, idsFractureEdges);
    edges.reserve(idsFractureEdges.size() * 2);
    if(err != CL_SUCCESS) {
        return err;
    }
    for(auto iter = idsFractureEdges.begin(); iter != idsFractureEdges.end(); ++iter) {
        edges.push_back(innerEdges[*iter * 2]);
        edges.push_back(innerEdges[*iter * 2 + 1]);
    }
    return err;
}

int CWaterOpenCL::getFractureFaces(std::vector<uint32_t>& faces) const {
    std::vector <uint32_t> fractureEdges,
                           innerFaces;
    int err = bufferIdsFractureEdges_.getData(commandQueue_, fractureEdges);
    if(err != CL_SUCCESS) {
        return err;
    }
    err = bufferInnerEdgeFaces_.getData(commandQueue_, innerFaces);
    if(err != CL_SUCCESS) {
        return err;
    }

    faces.reserve(faces.capacity() + fractureEdges.size() * 2);
    for(auto iter = fractureEdges.begin(); iter != fractureEdges.end(); ++iter) {
        faces.push_back(innerFaces[*iter * 2]);
        faces.push_back(innerFaces[*iter * 2 + 1]);
    }
    return CL_SUCCESS;
}

int CWaterOpenCL::getBorderEdges(std::vector<uint32_t>& edges) const {
    return bufferBorderEdges_.getData(commandQueue_, edges);
}

int CWaterOpenCL::getInnerEdges(std::vector<uint32_t>& edges) const {
    return bufferInnerEdges_.getData(commandQueue_, edges);
}

int CWaterOpenCL::computeData() {
    int err;
    std::cout << "Find edges\n";
    err = kernelFindEdges_.bindParametr(bufferFaces_, 0);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind faces", err);
        return err;
    }
    CMemObject bufferEdges(context_,
                           bufferFaces_.getSize() * 2,
                           CL_MEM_READ_WRITE);
    err = kernelFindEdges_.bindParametr(bufferEdges, 1);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind result edges buffer", err);
        return err;
    }

    uint32_t countFace = bufferFaces_.getSize() / sizeof(uint32_t);
    err = kernelFindEdges_.complite(commandQueue_,
                                   &countFace,
                                   1);
    if(err != CL_SUCCESS) {
        errorMessage("Fail complite findEdges kernel", err);
        return err;
    }
    CMemObject bufferMarkNoneBorder(context_,
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

    err = computeFractureEdges();
    if(err != CL_SUCCESS) {
        errorMessage("Fail compute fracture edges", err);
        return err;
    }
    return err;
}

int CWaterOpenCL::computeInnerEdges(const CMemObject &bufferEdges,
                                    const CMemObject &bufferMarkNoneBorder) {
    std::cout << "Find inner edges\n";
    int err;
    err = kernelFindInnerEdges_.bindParametr(bufferEdges, 0);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind faces", err);
        return err;
    }

    err = bufferInnerEdges_.resize(context_,
                                   bufferEdges.getSize());
    if(err != CL_SUCCESS) {
        errorMessage("Fail set memmory for inner edges", err);
        return err;
    }
    err = kernelFindInnerEdges_.bindParametr(bufferInnerEdges_, 1);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind inner edges buffer", err);
        return err;
    }

   
    err = kernelFindInnerEdges_.bindParametr(bufferMarkNoneBorder, 2);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind mark none border buffer", err);
        return err;
    }

    CMemObject bufferCountInnerEdges(context_,
                                     sizeof(uint32_t),
                                     CL_MEM_READ_WRITE);
    err = kernelFindInnerEdges_.bindParametr(bufferCountInnerEdges, 3);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind count inner edges buffer", err);
        return err;
    }

    bufferInnerEdgeFaces_.resize(context_,
                                 bufferEdges.getSize());
    if(err != CL_SUCCESS) {
        errorMessage("Fail set memmory for inner edges", err);
        return err;
    }
    err = kernelFindInnerEdges_.bindParametr(bufferInnerEdgeFaces_, 4);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind inner edges buffer", err);
        return err;
    }
    uint32_t workSizes[] = {bufferEdges.getSize() / (2 * sizeof(uint32_t)),
                            bufferEdges.getSize() / (2 * sizeof(uint32_t))};
    err = kernelFindInnerEdges_.complite(commandQueue_,
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

    err = bufferInnerEdges_.resize(context_,
                                   commandQueue_,
                                   countInnerEdges_ * sizeof(uint32_t) * 2);
    if(err != CL_SUCCESS) {
        errorMessage("Fail resize inner edges buffer", err);
        return err;
    }

    err = bufferInnerEdgeFaces_.resize(context_,
                                       commandQueue_,
                                       countInnerEdges_ * sizeof(uint32_t) * 2);
    if(err != CL_SUCCESS) {
        errorMessage("Fail resize inner edge faces buffer", err);
        return err;
    }

    return err;
}

int CWaterOpenCL::computeBorderEdges(const CMemObject &bufferEdges,
                                     const CMemObject &bufferMarkNoneBorder) {
    std::cout << "Find border edges\n";
    int err;
    err = kernelFindBorderEdges_.bindParametr(bufferEdges, 0);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind faces", err);
        return err;
    }
    err = kernelFindBorderEdges_.bindParametr(bufferMarkNoneBorder, 1);
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
    err = kernelFindBorderEdges_.bindParametr(bufferBorderEdges_, 2);
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
    err = kernelFindBorderEdges_.bindParametr(bufferBorderEdgeFaces_, 3);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind border face buffer", err);
        return err;
    }

    CMemObject bufferCountBorderEdges(context_,
                                      sizeof(uint32_t),
                                      CL_MEM_READ_WRITE);
    err = kernelFindBorderEdges_.bindParametr(bufferCountBorderEdges, 4);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind count border edges buffer", err);
        return err;
    }

    uint32_t workSize = bufferEdges.getSize() / (2 * sizeof(uint32_t));
    err = kernelFindBorderEdges_.complite(commandQueue_, &workSize, 1);
    if(err != CL_SUCCESS) {
        errorMessage("Fail complite find border edges", err);
        return err;
    }
    
    return err;
}

int CWaterOpenCL::computeFractureEdges() {
    std::cout << "Find fracture edges\n";
    int err = kernelFindFractureEdges_.bindParametr(bufferVertex_, 0);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind vertex buffer", err);
        return err;
    }

    err = kernelFindFractureEdges_.bindParametr(bufferFaces_, 1);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind faces buffer", err);
        return err;
    }

    err = kernelFindFractureEdges_.bindParametr(bufferInnerEdges_, 2);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind inner edges buffer", err);
        return err;
    }

    err = kernelFindFractureEdges_.bindParametr(bufferInnerEdgeFaces_, 3);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind inner edge faces buffer", err);
        return err;
    }

    err = bufferIdsFractureEdges_.resize(context_,
                                      countInnerEdges_ * sizeof(uint32_t));
    if(err != CL_SUCCESS) {
        errorMessage("Fail set size bufferFractureEdges", err);
        return err;
    }
    err = kernelFindFractureEdges_.bindParametr(bufferIdsFractureEdges_, 4);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferFractureEdges", err);
        return err;
    }

    CMemObject bufferCountFractureEdges(context_,
                                        sizeof(uint32_t), 
                                        CL_MEM_READ_WRITE);
    err = kernelFindFractureEdges_.bindParametr(bufferCountFractureEdges, 5);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferFractureEdges", err);
        return err;
    }

    err = kernelFindFractureEdges_.complite(commandQueue_, &countInnerEdges_, 1);
    if(err != CL_SUCCESS) {
        errorMessage("Fail complite findFractureEdges", err);
        return err;
    }

    err = bufferCountFractureEdges.getData(commandQueue_, countFractureEdges_);
    if(err != CL_SUCCESS) {
        errorMessage("Fail get count fracture edges", err);
        return err;
    }

    err = bufferIdsFractureEdges_.resize(context_,
                                         commandQueue_,
                                         countFractureEdges_ * sizeof(uint32_t));
    if(err != CL_SUCCESS) {
        errorMessage("Fail resize bufferIdsFractureEdges", err);
        return err;
    }
    return err;
}
