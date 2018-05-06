/* Copyright (c) 2018, Aleksandrov Maksim */

#include "cl\cl.h"

#include "CWaterOpenCL.hpp"

CWaterOpenCL::CMemObject::CMemObject() {
    flag_ = CL_MEM_READ_WRITE;
    mem_ = nullptr;
    size_ = 0;
}

CWaterOpenCL::CMemObject::CMemObject(const CLdescriptor context,
                                     const uint32_t size,
                                     const int flag) {
    flag_ = flag;
    mem_ = nullptr;
    resize(context, size);
}

CWaterOpenCL::CMemObject::~CMemObject() {
    clReleaseMemObject((cl_mem)mem_);
}

CWaterOpenCL::CMemObject & CWaterOpenCL::CMemObject::operator=(const CMemObject & sec)
{
    return *this;

}

int CWaterOpenCL::CMemObject::loadData(const CLdescriptor context,
                                       const CLdescriptor commandQueue,
                                       const char *data,
                                       const uint32_t size) {
    if(size_ < size) {
        resize(context, size);
    }
    return clEnqueueWriteBuffer((cl_command_queue)commandQueue,
                                (cl_mem)mem_,
                                CL_FALSE,
                                0,
                                size,
                                data,
                                0,
                                NULL,
                                NULL);
}

int CWaterOpenCL::CMemObject::getData(const CLdescriptor commandQueue,
                                      char **data,
                                      uint32_t &size) const {
    if(size_ == 0) {
        size = 0;
        return CL_SUCCESS;
    }
    *data = new char[size_];
    size = size_;
    int err;
    err = clEnqueueReadBuffer((cl_command_queue)commandQueue,
                              (cl_mem)mem_,
                              CL_FALSE,
                              0,
                              size_,
                              *data,
                              0,
                              NULL,
                              NULL);
    return err;
}


uint32_t CWaterOpenCL::CMemObject::getSize() const {
    return size_;
}

int CWaterOpenCL::CMemObject::resize(const CLdescriptor context,
                                     const uint32_t size) {
    if(size_ == size) {
        return CL_SUCCESS;
    }

    if(mem_ != nullptr) {
        clReleaseMemObject((cl_mem)mem_);
    }

    int err = CL_SUCCESS;
    size_ = size;
    if(size_ == 0) {
        mem_ = nullptr;
    } else {
        mem_ = clCreateBuffer((cl_context)context,
                              flag_,
                              size_,
                              NULL,
                              &err);
    }
    return err;
}

int CWaterOpenCL::CMemObject::getData(const CLdescriptor commandQueue,
                                      std::vector<float> &data) const {
    char *buffer = nullptr;
    uint32_t sizeData = 0;
    int err = getData(commandQueue, &buffer, sizeData);
    if(err != CL_SUCCESS) {
        delete[] buffer;
        return err;
    }
    if(sizeData != 0) {
        data.reserve(sizeData / sizeof(float));
        data.insert(data.end(),
                    (float *)buffer,
                    (float *)&buffer[sizeData]);
        delete[] buffer;
    }
    return err;
}
int CWaterOpenCL::CMemObject::getData(const CLdescriptor commandQueue,
                                      std::vector<uint32_t> &data) const {
    char *buffer = nullptr;
    uint32_t sizeData = 0;
    int err = getData(commandQueue, &buffer, sizeData);
    if(err != CL_SUCCESS) {
        delete[] buffer;
        return err;
    }
    if(sizeData != 0) {
        data.reserve(sizeData / sizeof(uint32_t));
        data.insert(data.end(),
                    (uint32_t *)buffer,
                    (uint32_t *)&buffer[sizeData]);
        delete[] buffer;
    }
    return err;
}

int CWaterOpenCL::CMemObject::getData(const CLdescriptor commandQueue,
                                      uint32_t &data) const {
    char *buffer = nullptr;
    uint32_t sizeData = 0;
    int err = getData(commandQueue, &buffer, sizeData);
    if(err != CL_SUCCESS) {
        delete[] buffer;
        return err;
    }
    if(sizeData != 0) {
        if(sizeData >= sizeof(uint32_t)) {
            data = *(uint32_t*)buffer;
        } else {
            data = 0;
        }
        delete[] buffer;
    }
    return err;
}

int CWaterOpenCL::CMemObject::setFlag(const CLdescriptor context,
                                      const int flag) {
    if(flag_ == flag) {
        return CL_SUCCESS;
    }

    flag_ = flag;
    if(mem_ != nullptr) {
        clReleaseMemObject((cl_mem)mem_);
        mem_ = nullptr;
    }

    int err = CL_SUCCESS;
    if(size_ != 0) {
        mem_ = clCreateBuffer((cl_context)context,
                              flag_,
                              size_,
                              NULL,
                              &err);
    }
    return err;
}