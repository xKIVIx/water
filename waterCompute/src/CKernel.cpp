/* Copyright (c) 2018, Aleksandrov Maksim */

#include "cl\cl.h"

#include "CWaterOpenCL.hpp"

CWaterOpenCL::CKernel::CKernel() {
    kernel_ = nullptr;
}

CWaterOpenCL::CKernel::~CKernel() {
    if(kernel_ != nullptr) {
        clReleaseKernel((cl_kernel)kernel_);
    }
}

int CWaterOpenCL::CKernel::bindParametr(const CMemObject &memObject,
                                        const uint32_t paramId) {
    return clSetKernelArg((cl_kernel)kernel_, 
                          paramId, 
                          sizeof(cl_mem),
                          (cl_mem *)&memObject.mem_);
}

int CWaterOpenCL::CKernel::complite(const CLdescriptor commandQueue,
                                    const uint32_t *workSizes,
                                    const uint32_t countWorkSizes) {
    return clEnqueueNDRangeKernel((cl_command_queue)commandQueue,
                                  (cl_kernel)kernel_,
                                  countWorkSizes,
                                  NULL,
                                  workSizes,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL);
}

int CWaterOpenCL::CKernel::setFunction(const CLdescriptor program,
                                       const char * name) {
    int err = CL_SUCCESS;
    kernel_ = clCreateKernel((cl_program)program, name, &err);
    return err;
}