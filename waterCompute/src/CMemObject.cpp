/* Copyright (c) 2018, Aleksandrov Maksim */

#include "cl\cl.h"

#include "CWaterOpenCL.hpp"

CWaterOpenCL::CMemObject::CMemObject() {
    flag_ = CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR;
    mem_ = nullptr;
    size_ = 0;
}

CWaterOpenCL::CMemObject::CMemObject(const CLdescriptor context,
                                     const uint32_t size,
                                     const int flag) {
    flag_ = flag | CL_MEM_ALLOC_HOST_PTR;
    mem_ = nullptr;
    size_ = 0;
    resize(context,
           size);
}

CWaterOpenCL::CMemObject::~CMemObject() {
    clReleaseMemObject((cl_mem)mem_);
}

int CWaterOpenCL::CMemObject::concat(const CLdescriptor context, 
                                     const CLdescriptor commandQueue, 
                                     const CMemObject &obj1, 
                                     const CMemObject &obj2) {
    int err = resize(context, obj1.size_ + obj2.size_);
    if(err != CL_SUCCESS) {
        return err;
    }
    err = clEnqueueCopyBuffer((cl_command_queue)commandQueue,
                              (cl_mem)obj1.mem_,
                              (cl_mem)mem_,
                              0,
                              0,
                              obj1.size_,
                              0,
                              nullptr,
                              nullptr);
    if(err != CL_SUCCESS) {
        return err;
    }
    err = clEnqueueCopyBuffer((cl_command_queue)commandQueue,
                              (cl_mem)obj2.mem_,
                              (cl_mem)mem_,
                              0,
                              obj1.size_,
                              obj2.size_,
                              0,
                              nullptr,
                              nullptr);
    return err;
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
        resize(context,
               size);
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
    if(err != CL_SUCCESS) {
        return err;
    }
    return clFinish((cl_command_queue)commandQueue);
}


uint32_t CWaterOpenCL::CMemObject::getSize() const {
    return size_;
}

int CWaterOpenCL::CMemObject::resize(const CLdescriptor context,
                                     const CLdescriptor commandQueue,
                                     const uint32_t size) {
    int err = CL_SUCCESS;
    if(size_ == size) {
        return err;
    }
    size_ = size;
    
    if(mem_ == nullptr) {
        mem_ = clCreateBuffer((cl_context)context,
                              flag_,
                              size_,
                              NULL,
                              &err);
    } else {
        CLdescriptor newMem = clCreateBuffer((cl_context)context,
                                             flag_,
                                             size_,
                                             NULL,
                                             &err);
        if(err != CL_SUCCESS) {
            return err;
        }
        clEnqueueCopyBuffer((cl_command_queue)commandQueue,
                            (cl_mem)mem_,
                            (cl_mem)newMem,
                            0,
                            0,
                            size_,
                            0,
                            nullptr,
                            nullptr);
        clReleaseMemObject((cl_mem)mem_);
        mem_ = newMem;
    }
    return err;
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
        data.reserve(data.size() + sizeData / sizeof(float));
        data.insert(data.end(),
                    (float *)buffer,
                    (float *)(buffer + sizeData));
        delete[] buffer;
    }
    return err;
}

int CWaterOpenCL::CMemObject::getData(const CLdescriptor commandQueue,
                                      std::vector<bool> &data) const {
    char *buffer = nullptr;
    uint32_t sizeData = 0;
    int err = getData(commandQueue, &buffer, sizeData);
    if(err != CL_SUCCESS) {
        delete[] buffer;
        return err;
    }
    if(sizeData != 0) {
        data.reserve(data.size() + sizeData / sizeof(bool));
        data.insert(data.end(),
                    (bool *)buffer,
                    (bool *)(buffer + sizeData));
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
        data.reserve(data.size() + sizeData / sizeof(uint32_t));
        data.insert(data.end(),
                    (uint32_t *)buffer,
                    (uint32_t *)(buffer + sizeData));
        delete[] buffer;
    }
    return err;
}

int CWaterOpenCL::CMemObject::getData(const CLdescriptor commandQueue,
                                      std::vector<char> &data) const {
    char *buffer = nullptr;
    uint32_t sizeData = 0;
    int err = getData(commandQueue, &buffer, sizeData);
    if(err != CL_SUCCESS) {
        delete[] buffer;
        return err;
    }
    if(sizeData != 0) {
        data.reserve(data.size() + sizeData);
        data.insert(data.end(),
                    buffer,
                    buffer + sizeData);
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

    flag_ = flag | CL_MEM_ALLOC_HOST_PTR;
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

void CWaterOpenCL::CMemObject::swap(CMemObject & second) {
    CLdescriptor mem = mem_;
    uint32_t size = size_;
    int flag = flag_;

    mem_ = second.mem_;
    size_ = second.size_;
    flag_ = second.flag_;

    second.mem_ = mem;
    second.size_ = size;
    second.flag_ = flag;
}