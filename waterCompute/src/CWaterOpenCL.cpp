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

inline void roadMatOut(const std::vector<uint32_t> &edges) {
    for(uint32_t i = 0; i < edges.size(); i+=2) {
        for(uint32_t j = 0; j < edges.size(); j += 2) {
            if(i == j) {
                std::cout << "0, ";
                continue;
            }
            if((edges[i] == edges[j]) ||
                (edges[i] == edges[j + 1]) ||
               (edges[i + 1] == edges[j]) ||
               (edges[i + 1] == edges[j + 1])) {
                std::cout << "1, ";
            } else {
                std::cout << "0, ";
            }
        }
        std::cout << std::endl;
    }
}

inline void chek( const cl_int  stat ) {
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
        errorMessage("Fail init kernelFindEdges", err);
        return err;
    }

    err = kernelFindInnerEdges_.setFunction(program_,
                                           "findInnerEdges");
    if(err != CL_SUCCESS) {
        errorMessage("Fail init kernelFindInnerEdges", err);
        return err;
    }

    err = kernelFindBorderEdges_.setFunction(program_,
                                            "findBorderEdges");
    if(err != CL_SUCCESS) {
        errorMessage("Fail init kernelFindBorderEdges", err);
        return err;
    }

    err = kernelFindFractureEdges_.setFunction(program_,
                                            "findFractureEdges");
    if(err != CL_SUCCESS) {
        errorMessage("Fail init kernelFindFractureEdges", err);
        return err;
    }

    err = kernelDeleteDoubleVert_.setFunction(program_,
                                              "deleteDoubleVert");
    if(err != CL_SUCCESS) {
        errorMessage("Fail init kernelDeleteDoubleVert", err);
        return err;
    }

    err = kernelFindInnerVertex_.setFunction(program_,
                                            "findInnerVertex");
    if(err != CL_SUCCESS) {
        errorMessage("Fail init kernelFindInnerVertex", err);
        return err;
    }

    err = kernelFindInnerFaces_.setFunction(program_,
                                            "findInnerFaces");
    if(err != CL_SUCCESS) {
        errorMessage("Fail init kernelFindInnerFaces", err);
        return err;
    }

    err = kernelCountColise_.setFunction(program_,
                                         "countColise");
    if(err != CL_SUCCESS) {
        errorMessage("Fail init kernelCountColise", err);
        return err;
    }

    err = kernelRemoveCommunityAreas_.setFunction(program_,
                                                  "removeCommunityAreas");
    if(err != CL_SUCCESS) {
        errorMessage("Fail init removeCommunityAreas", err);
        return err;
    }

    err = kernelFindUnionVertex_.setFunction(program_,
                                                  "findUnionVertex");
    if(err != CL_SUCCESS) {
        errorMessage("Fail init findUnionVertex", err);
        return err;
    }

    err = kernelGetMarkData_.setFunction(program_,
                                         "getMarkData");
    if(err != CL_SUCCESS) {
        errorMessage("Fail init getMarkData", err);
        return err;
    }
    return CL_SUCCESS;
}

CWaterOpenCL::~CWaterOpenCL() {
    clearOpenCl();
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
    face.insert(face.end(),
                facesIn_.begin(),
                facesIn_.end());
    return CL_SUCCESS;
}

int CWaterOpenCL::getBorderFaces(std::vector<uint32_t> &face) const {
    std::vector <uint32_t> counts;
    counts.resize(bufferFaces_.getSize() / sizeof(uint32_t) / 3);
    for(auto iter = facesIn_.begin(); iter != facesIn_.end(); ++iter) {
        counts[*iter] ++;
    }
    for(uint32_t i = 0; i < counts.size(); i++) {
        if(counts[i] < 3) {
            face.push_back(i);
        }
    }
    return  CL_SUCCESS;
}

int CWaterOpenCL::getFractureEdges(std::vector <uint32_t> &edges) const {
    edges.reserve(countFractureEdges_ * 2);
    edges.insert(edges.end(),
                 edgesHoleBorders_.data(),
                 &edgesHoleBorders_.data()[countFractureEdges_  * 2]);
    return CL_SUCCESS;
}

int CWaterOpenCL::getBorderEdges(std::vector<uint32_t>& edges) const {
    edges.reserve(countBorderEdges_ * 2);
    edges.insert(edges.end(),
                 &edgesHoleBorders_.data()[countFractureEdges_ * 2],
                 &edgesHoleBorders_.data()[(countFractureEdges_ + countBorderEdges_) * 2]);
    return CL_SUCCESS;
}

int CWaterOpenCL::getVertex(std::vector<float>& result) const {
    return bufferVertex_.getData(commandQueue_, result);
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
    CMemObject bufferInnerFaces(context_,
                                bufferEdges.getSize(),
                                CL_MEM_READ_WRITE);
    CMemObject bufferInnerEdges(context_,
                                bufferEdges.getSize(),
                                CL_MEM_READ_WRITE);
    err = computeInnerEdges(bufferEdges,
                            bufferMarkNoneBorder,
                            bufferInnerEdges,
                            bufferInnerFaces);
    if(err != CL_SUCCESS) {
        errorMessage("Fail compute inner edges", err);
        return err;
    }

    err = computeFractureEdges(bufferInnerFaces,
                               bufferInnerEdges);
    if(err != CL_SUCCESS) {
        errorMessage("Fail compute fracture edges", err);
        return err;
    }
    edgesHoleBorders_.resize(countFractureEdges_ * 2);
    err = bufferInnerFaces.getData(commandQueue_, facesIn_);
    if(err != CL_SUCCESS) {
        errorMessage("Fail get data from bufferInnerFaces", err);
        return err;
    }
    facesIn_.resize(countInnerEdges_ * 2);
    err = computeBorderEdges(bufferEdges,
                             bufferMarkNoneBorder);
    if(err != CL_SUCCESS) {
        errorMessage("Fail compute border edges", err);
        return err;
    }
    edgesHoleBorders_.resize((countFractureEdges_ + countBorderEdges_) * 2);
    return err;
}

int CWaterOpenCL::computeInnerEdges(const CMemObject &bufferEdges,
                                    const CMemObject &bufferMarkNoneBorder,
                                    const CMemObject &bufferInnerEdges,
                                    const CMemObject &bufferInnerFaces) {
    std::cout << "Find inner edges\n";
    int err;
    err = kernelFindInnerEdges_.bindParametr(bufferEdges, 0);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind faces", err);
        return err;
    }

    err = kernelFindInnerEdges_.bindParametr(bufferInnerEdges, 1);
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
    countInnerEdges_ = 0;
    err = bufferCountInnerEdges.loadData(context_,
                                         commandQueue_,
                                         (char *)&countInnerEdges_,
                                         sizeof(uint32_t));
    if(err != CL_SUCCESS) {
        errorMessage("Fail zeroing bufferCountInnerEdges", err);
        return err;
    }
    err = kernelFindInnerEdges_.bindParametr(bufferCountInnerEdges, 3);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind count inner edges buffer", err);
        return err;
    }

    err = kernelFindInnerEdges_.bindParametr(bufferInnerFaces, 4);
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

    return err;
}

int CWaterOpenCL::computeInnerFaces(const std::list<std::vector<uint32_t>> &borders, 
                                    std::list<std::vector<uint32_t>> &faces) {
    uint32_t maxSize = 0;
    for(auto iter = borders.begin(); iter != borders.end(); ++iter) {
        maxSize = (iter->size() > maxSize) ? iter->size() : maxSize;
    }
    CMemObject bufferBorder(context_,
                            maxSize * sizeof(uint32_t),
                            CL_MEM_READ_ONLY);
    CMemObject buferInnerVertex(context_,
                                bufferVertex_.getSize() / 3,
                                CL_MEM_READ_WRITE);
    CMemObject bufferEdges(context_,
                           edgesHoleBorders_.size() * sizeof(uint32_t),
                           CL_MEM_READ_ONLY);
    CMemObject bufferInnerFaces(context_,
                                bufferFaces_.getSize(),
                                CL_MEM_READ_WRITE);
    CMemObject bufferCountInnerFaces(context_,
                                     sizeof(uint32_t),
                                     CL_MEM_READ_WRITE);
    int err = bufferEdges.loadData(context_,
                                   commandQueue_,
                                   (char *)edgesHoleBorders_.data(),
                                   bufferEdges.getSize());
    if(err != CL_SUCCESS) {
        errorMessage("Fail load data into bufferEdges", err);
        return err;
    }
    for(auto iter = borders.begin(); iter != borders.end(); ++iter) {
        err = bufferBorder.loadData(context_,
                                    commandQueue_,
                                    (char *)iter->data(),
                                    iter->size() * sizeof(uint32_t));
        if(err != CL_SUCCESS) {
            errorMessage("Fail load data into bufferBorder", err);
            return err;
        }
        uint32_t countVertex = 0;
        err = computeInnerVertex(bufferEdges, 
                                 bufferBorder,
                                 iter->size(),
                                 buferInnerVertex,
                                 countVertex);
        if(err != CL_SUCCESS) {
            errorMessage("Fail computeInnerVertex", err);
            return err;
        }

        err = kernelFindInnerFaces_.bindParametr(bufferFaces_, 0);
        if(err != CL_SUCCESS) {
            errorMessage("Fail bind bufferFaces", err);
            return err;
        }

        err = kernelFindInnerFaces_.bindParametr(bufferFaces_.getSize() / sizeof(uint32_t), 1);
        if(err != CL_SUCCESS) {
            errorMessage("Fail bind size faces", err);
            return err;
        }
        err = kernelFindInnerFaces_.bindParametr(buferInnerVertex, 2);
        if(err != CL_SUCCESS) {
            errorMessage("Fail bind bufferInnerVertex", err);
            return err;
        }

        err = kernelFindInnerFaces_.bindParametr(bufferInnerFaces, 3);
        if(err != CL_SUCCESS) {
            errorMessage("Fail bind bufferInnerFaces", err);
            return err;
        }
        uint32_t countInnerFaces = 0;
        err = bufferCountInnerFaces.loadData(context_,
                                             commandQueue_,
                                             (char *)&countInnerFaces,
                                             sizeof(uint32_t));
        err = kernelFindInnerFaces_.bindParametr(bufferCountInnerFaces, 4);
        if(err != CL_SUCCESS) {
            errorMessage("Fail bind bufferCountInnerFaces", err);
            return err;
        }
        uint32_t workSize[3] = {
            countVertex,
            countVertex,
            countVertex
        };

        err = kernelFindInnerFaces_.complite(commandQueue_,
                                             workSize,
                                             3);
        if(err != CL_SUCCESS) {
            errorMessage("Fail complite kernelFindInnerFaces", err);
            return err;
        }
        faces.emplace_back();
        err = bufferCountInnerFaces.getData(commandQueue_, countInnerFaces);
        if(err != CL_SUCCESS) {
            errorMessage("Fail get data bufferInnerFaces", err);
            return err;
        }
        err = bufferInnerFaces.getData(commandQueue_, faces.back());
        if(err != CL_SUCCESS) {
            errorMessage("Fail get data bufferInnerFaces", err);
            return err;
        }
        faces.back().resize(countInnerFaces);
    }
    return CL_SUCCESS;
}

class CClrear {
public:
    CClrear(uint32_t size) {
        clearMem = new char[size];
        for(uint32_t i = 0; i < size; i++) {
            clearMem[i] = 0;
        }
    }
    ~CClrear() {
        delete[] clearMem;
    }
    const char *get() const {
        return clearMem;
    }
private:
    char *clearMem = nullptr;
};

int CWaterOpenCL::removeCommunityAreas(std::list<std::vector<uint32_t>>& areas,
                                       std::list<std::vector<uint32_t>>& newAreas) {
    CClrear clearMem(bufferFaces_.getSize() / (3 * sizeof(uint32_t)));
    CMemObject bufferArea(context_,
                          bufferFaces_.getSize() / 3,
                          CL_MEM_READ_ONLY),
        bufferFirstCountersColise(context_,
                                  bufferFaces_.getSize() / (3 * sizeof(uint32_t)),
                                  CL_MEM_READ_WRITE),
        bufferSecondCountersColise(context_,
                                   bufferFaces_.getSize() / (3 * sizeof(uint32_t)),
                                   CL_MEM_READ_WRITE),
        bufferResultFirstArea(context_,
                              bufferFaces_.getSize() / 3,
                              CL_MEM_READ_WRITE),
        bufferResultSecondArea(context_,
                               bufferFaces_.getSize() / 3,
                               CL_MEM_READ_WRITE),
        bufferResultThirdArea(context_,
                              bufferFaces_.getSize() / 3,
                              CL_MEM_READ_WRITE),
        bufferResultSizes(context_,
                          sizeof(uint32_t) * 3,
                          CL_MEM_READ_WRITE);
    int err = kernelCountColise_.bindParametr (bufferArea, 0);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferArea", err);
        return err;
    }

    err = kernelRemoveCommunityAreas_.bindParametr(bufferFirstCountersColise, 0);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferFirstCountersColise", err);
        return err;
    }
    err = kernelRemoveCommunityAreas_.bindParametr(bufferSecondCountersColise, 1);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferSecondCountersColise", err);
        return err;
    }
    err = kernelRemoveCommunityAreas_.bindParametr(bufferResultFirstArea, 2);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferResultFirstArea", err);
        return err;
    }
    err = kernelRemoveCommunityAreas_.bindParametr(bufferResultSecondArea, 3);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferResultSecondArea", err);
        return err;
    }
    err = kernelRemoveCommunityAreas_.bindParametr(bufferResultThirdArea, 4);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferResultThirdArea", err);
        return err;
    }
    err = kernelRemoveCommunityAreas_.bindParametr(bufferResultSizes, 5);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferResultSizes", err);
        return err;
    }

    for(auto iterFirstArea = areas.begin(); iterFirstArea != areas.end(); ++iterFirstArea) {

        err = bufferArea.loadData(context_, 
                                  commandQueue_, 
                                  (char *)iterFirstArea->data(), 
                                  iterFirstArea->size() * sizeof(uint32_t));
        if(err != CL_SUCCESS) {
            errorMessage("Fail load data into bufferArea", err);
            return err;
        }

        err = kernelCountColise_.bindParametr(bufferFirstCountersColise, 1);
        if(err != CL_SUCCESS) {
            errorMessage("Fail bind bufferFirstCountersColise", err);
            return err;
        }
        err = bufferFirstCountersColise.loadData(context_, 
                                                 commandQueue_, 
                                                 clearMem.get(), 
                                                 bufferFirstCountersColise.getSize());
        if(err != CL_SUCCESS) {
            errorMessage("Fail zeroing bufferFirstCountersColise", err);
            return err;
        }

        uint32_t workSize = iterFirstArea->size();
        err = kernelCountColise_.complite(commandQueue_, &workSize, 1);
        if(err != CL_SUCCESS) {
            errorMessage("Fail complite kernelCountColise", err);
            return err;
        }
        auto iterSecondArea = iterFirstArea;
        iterSecondArea++;
        for(; iterSecondArea != areas.end(); ++iterSecondArea) {
            err = bufferArea.loadData(context_, 
                                      commandQueue_, 
                                      (char *)iterSecondArea->data(), 
                                      iterSecondArea->size() * sizeof(uint32_t));
            if(err != CL_SUCCESS) {
                errorMessage("Fail load data into bufferArea", err);
                return err;
            }

            err = bufferSecondCountersColise.loadData(context_, 
                                                      commandQueue_, 
                                                      clearMem.get(), 
                                                      bufferSecondCountersColise.getSize());
            if(err != CL_SUCCESS) {
                errorMessage("Fail zeroing bufferSecondCountersColise", err);
                return err;
            }
            err = kernelCountColise_.bindParametr (bufferSecondCountersColise, 1);
            if(err != CL_SUCCESS) {
                errorMessage("Fail bind bufferSecondCountersColise", err);
                return err;
            }

            workSize = iterSecondArea->size();
            err = kernelCountColise_.complite(commandQueue_, &workSize, 1);
            if(err != CL_SUCCESS) {
                errorMessage("Fail complite kernelCountColise", err);
                return err;
            }

            //
            err = bufferResultSizes.loadData(context_,
                                             commandQueue_,
                                             clearMem.get(),
                                             bufferResultSizes.getSize());
            if(err != CL_SUCCESS) {
                errorMessage("Fail zeroing bufferResultSizes", err);
                return err;
            }

            workSize = bufferVertex_.getSize() / (3 * sizeof(uint32_t));
            err = kernelRemoveCommunityAreas_.complite(commandQueue_, 
                                                       &workSize, 
                                                       1);
            if(err != CL_SUCCESS) {
                errorMessage("Fail complite kernelRemove—ommunityAreas", err);
                return err;
            }
            std::vector <uint32_t> sizes;
            err = bufferResultSizes.getData(commandQueue_, sizes);
            if(err != CL_SUCCESS) {
                errorMessage("Fail get data from bufferResultSizes", err);
                return err;
            }
            if((sizes[0] != 0) && (sizes[0] != iterFirstArea->size())) {
                iterFirstArea->clear();
                err = bufferResultFirstArea.getData(commandQueue_, *iterFirstArea);
                if(err != CL_SUCCESS) {
                    errorMessage("Fail get data from bufferResultFirstArea", err);
                    return err;
                }
                iterFirstArea->resize(sizes[0]);

                err = kernelCountColise_.bindParametr(bufferResultFirstArea, 0);
                if(err != CL_SUCCESS) {
                    errorMessage("Fail bind bufferResultFirstArea", err);
                    return err;
                }
                err = kernelCountColise_.bindParametr(bufferFirstCountersColise, 1);
                if(err != CL_SUCCESS) {
                    errorMessage("Fail bind bufferFirstCountersColise", err);
                    return err;
                }
                err = bufferFirstCountersColise.loadData(context_, 
                                                         commandQueue_, 
                                                         clearMem.get(), 
                                                         bufferFirstCountersColise.getSize());
                if(err != CL_SUCCESS) {
                    errorMessage("Fail zeroing bufferFirstCountersColise", err);
                    return err;
                }

                workSize = iterFirstArea->size();
                err = kernelCountColise_.complite(commandQueue_, &workSize, 1);
                if(err != CL_SUCCESS) {
                    errorMessage("Fail complite kernelCountColise", err);
                    return err;
                }

                err = kernelCountColise_.bindParametr(bufferArea, 0);
                if(err != CL_SUCCESS) {
                    errorMessage("Fail bind bufferArea", err);
                    return err;
                }

            }
            if((sizes[1] != 0) && (sizes[1] != iterSecondArea->size())) {
                iterSecondArea->clear();
                err = bufferResultSecondArea.getData(commandQueue_, *iterSecondArea);
                if(err != CL_SUCCESS) {
                    errorMessage("Fail get data from bufferResultSecondArea", err);
                    return err;
                }
                iterSecondArea->resize(sizes[1]);

                err = kernelCountColise_.bindParametr(bufferResultSecondArea, 0);
                if(err != CL_SUCCESS) {
                    errorMessage("Fail bind bufferResultSecondArea", err);
                    return err;
                }
                err = kernelCountColise_.bindParametr(bufferSecondCountersColise, 1);
                if(err != CL_SUCCESS) {
                    errorMessage("Fail bind bufferSecondCountersColise", err);
                    return err;
                }
                err = bufferSecondCountersColise.loadData(context_, 
                                                         commandQueue_, 
                                                         clearMem.get(), 
                                                         bufferSecondCountersColise.getSize());
                if(err != CL_SUCCESS) {
                    errorMessage("Fail zeroing bufferFirstCountersColise", err);
                    return err;
                }

                workSize = iterSecondArea->size();
                err = kernelCountColise_.complite(commandQueue_, &workSize, 1);
                if(err != CL_SUCCESS) {
                    errorMessage("Fail complite kernelCountColise", err);
                    return err;
                }

                err = kernelCountColise_.bindParametr(bufferArea, 0);
                if(err != CL_SUCCESS) {
                    errorMessage("Fail bind bufferArea", err);
                    return err;
                }
            }
            if((sizes[0] != 0) && (sizes[1] != 0) && (sizes[2] != 0)) {
                newAreas.emplace_back();
                bufferResultThirdArea.getData(commandQueue_, newAreas.back());
                newAreas.back().resize(sizes[2]);
            }
        }
    }
    return err;
}

int CWaterOpenCL::findUnionVertex(const std::list<std::vector<uint32_t>>& areas, 
                                  std::list<std::vector<uint32_t>>& unionVertex) {
    CMemObject bufferFirstArea(context_,
                               bufferFaces_.getSize() / 3,
                               CL_MEM_READ_ONLY),
               bufferSecondArea(context_,
                                bufferFaces_.getSize() / 3,
                                CL_MEM_READ_ONLY),
               bufferMarks(context_,
                           bufferVertex_.getSize() / 3,
                           CL_MEM_READ_WRITE),
               bufferResult(context_,
                            bufferFaces_.getSize() / 3,
                            CL_MEM_WRITE_ONLY),
               bufferResultSize(context_,
                                sizeof(uint32_t),
                                CL_MEM_READ_WRITE);
    CClrear clearMem(bufferMarks.getSize());
    int err;
    err = kernelFindUnionVertex_.bindParametr(bufferFaces_, 0);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferFaces", err);
        return err;
    }
    err = kernelFindUnionVertex_.bindParametr(bufferFirstArea, 1);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferFirstArea", err);
        return err;
    }
    err = kernelFindUnionVertex_.bindParametr(bufferSecondArea, 2);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferSecondArea", err);
        return err;
    }
    err = kernelFindUnionVertex_.bindParametr(bufferMarks, 3);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferMarks", err);
        return err;
    }
    err = kernelGetMarkData_.bindParametr(bufferMarks, 0);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferMarks", err);
        return err;
    }
    err = kernelGetMarkData_.bindParametr(bufferResult, 1);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferResult", err);
        return err;
    }
    err = kernelGetMarkData_.bindParametr(bufferResultSize, 2);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferMarks", err);
        return err;
    }

    for(auto iterFirst = areas.begin(); iterFirst != areas.end(); ++iterFirst) {
        err = bufferFirstArea.loadData(context_,
                                       commandQueue_,
                                       (char *)iterFirst->data(),
                                       iterFirst->size() * sizeof(uint32_t));
        if(err != CL_SUCCESS) {
            errorMessage("Fail load bufferFirstArea", err);
            return err;
        }
        
        auto iterSecond = iterFirst;
        iterSecond++;
        for(; iterSecond != areas.end(); ++iterSecond) {
            err = bufferSecondArea.loadData(context_,
                                            commandQueue_,
                                            (char *)iterSecond->data(),
                                            iterSecond->size() * sizeof(uint32_t));
            if(err != CL_SUCCESS) {
                errorMessage("Fail load bufferSecondArea", err);
                return err;
            }
            err = bufferMarks.loadData(context_,
                                       commandQueue_,
                                       clearMem.get(),
                                       bufferMarks.getSize());
            if(err != CL_SUCCESS) {
                errorMessage("Fail zeroing bufferMarks", err);
                return err;
            }

            uint32_t resultSize = 0;
            err = bufferResultSize.loadData(context_,
                                            commandQueue_,
                                            (char *)&resultSize,
                                            sizeof(uint32_t));
            if(err != CL_SUCCESS) {
                errorMessage("Fail zeroing bufferResultSize", err);
                return err;
            }

            uint32_t workSize[2] = {
                iterFirst->size(),
                iterSecond->size()
            };
            err = kernelFindUnionVertex_.complite(commandQueue_,
                                                  workSize,
                                                  2);
            if(err != CL_SUCCESS) {
                errorMessage("Fail complite kernelFindUnionVertex", err);
                return err;
            }
            uint32_t workSize1 = bufferMarks.getSize() / sizeof(uint32_t);
            err = kernelGetMarkData_.complite(commandQueue_,
                                              &workSize1,
                                              1);

            unionVertex.emplace_back();
            err = bufferResult.getData(commandQueue_, unionVertex.back());
            if(err != CL_SUCCESS) {
                errorMessage("Fail get data bufferResult", err);
                return err;
            }
            err = bufferResultSize.getData(commandQueue_, resultSize);
            if(err != CL_SUCCESS) {
                errorMessage("Fail get data bufferResultSize", err);
                return err;
            }
            unionVertex.back().resize(resultSize);
        }
    }
    return CL_SUCCESS;
}

int CWaterOpenCL::computeInnerVertex(const CMemObject &edges,
                                     const CMemObject &bufferBorder,
                                     const uint32_t   borderSize,
                                     const CMemObject &bufferInnerVertex,
                                     uint32_t &countInnerVertex) {
    int err = kernelFindInnerVertex_.bindParametr(edges, 0);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind edges", err);
        return err;
    }

    err = kernelFindInnerVertex_.bindParametr(bufferBorder, 1);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferBorder", err);
        return err;
    }

    err = kernelFindInnerVertex_.bindParametr(borderSize, 2);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind border size", err);
        return err;
    }

    err = kernelFindInnerVertex_.bindParametr(bufferVertex_, 3);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferVertex_", err);
        return err;
    }

    err = kernelFindInnerVertex_.bindParametr(bufferInnerVertex, 4);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferInnerVertex", err);
        return err;
    }
    
    CMemObject bufferCountInnerVertex(context_,
                                      sizeof(uint32_t),
                                      CL_MEM_READ_WRITE);
    err = bufferCountInnerVertex.loadData(context_,
                                          commandQueue_,
                                          (char *)&countInnerVertex,
                                          sizeof(uint32_t));
    if(err != CL_SUCCESS) {
        errorMessage("Fail zeroing bufferCountInnerVertex", err);
        return err;
    }
    err = kernelFindInnerVertex_.bindParametr(bufferCountInnerVertex, 5);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferCountInnerVertex", err);
        return err;
    }

    uint32_t workSize = bufferVertex_.getSize() / (sizeof(float) * 3);

    err = kernelFindInnerVertex_.complite(commandQueue_, &workSize, 1);
    if(err != CL_SUCCESS) {
        errorMessage("Fail complite kernelComputeInnerFaces", err);
        return err;
    }
    err = bufferCountInnerVertex.getData(commandQueue_, countInnerVertex);
    if(err != CL_SUCCESS) {
        errorMessage("Fail get data bufferCountInnerVertex", err);
        return err;
    }
    return CL_SUCCESS;
}

void CWaterOpenCL::clearOpenCl() {
    facesIn_.clear();
    edgesHoleBorders_.clear();
    bufferFaces_.resize(context_, 0);
    bufferVertex_.resize(context_, 0);
    clFinish((cl_command_queue)commandQueue_);
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
    
    CMemObject bufferBorderEdges(context_,
                                 bufferEdges.getSize(),
                                 CL_MEM_READ_WRITE);
    err = kernelFindBorderEdges_.bindParametr(bufferBorderEdges, 2);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind border edges buffer", err);
        return err;
    }

    CMemObject bufferCountBorderEdges(context_,
                                      sizeof(uint32_t),
                                      CL_MEM_READ_WRITE);
    countBorderEdges_ = 0;
    err = bufferCountBorderEdges.loadData(context_,
                                          commandQueue_,
                                          (char *)&countBorderEdges_,
                                          sizeof(uint32_t));
    err = kernelFindBorderEdges_.bindParametr(bufferCountBorderEdges, 3);
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
    
    err = bufferCountBorderEdges.getData(commandQueue_, countBorderEdges_);
    if(err != CL_SUCCESS) {
        errorMessage("Fail complite get countBorderEdges", err);
        return err;
    }

    err = bufferBorderEdges.getData(commandQueue_, edgesHoleBorders_);
    if(err != CL_SUCCESS) {
        errorMessage("Fail complite get BorderEdges", err);
        return err;
    }

    return err;
}

int CWaterOpenCL::computeFractureEdges(const CMemObject &bufferInnerFaces,
                                       const CMemObject &bufferInnerEdges) {
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

    err = kernelFindFractureEdges_.bindParametr(bufferInnerEdges, 2);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind inner edges buffer", err);
        return err;
    }

    err = kernelFindFractureEdges_.bindParametr(bufferInnerFaces, 3);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind inner edge faces buffer", err);
        return err;
    }
    CMemObject bufferFractureEdges(context_,
                                   bufferInnerEdges.getSize(),
                                   CL_MEM_READ_WRITE);
    err = kernelFindFractureEdges_.bindParametr(bufferFractureEdges, 4);
    if(err != CL_SUCCESS) {
        errorMessage("Fail bind bufferFractureEdges", err);
        return err;
    }

    CMemObject bufferCountFractureEdges(context_,
                                        sizeof(uint32_t), 
                                        CL_MEM_READ_WRITE);
    countFractureEdges_ = 0;
    err = bufferCountFractureEdges.loadData(context_,
                                            commandQueue_,
                                            (char *)&countFractureEdges_,
                                            sizeof(uint32_t));
    if(err != CL_SUCCESS) {
        errorMessage("Fail zeroing bufferCountFractureEdges", err);
        return err;
    }

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

    err = bufferFractureEdges.getData(commandQueue_, edgesHoleBorders_);
    if(err != CL_SUCCESS) {
        errorMessage("Fail get data from bufferFractureEdges", err);
        return err;
    }
    return err;
}
