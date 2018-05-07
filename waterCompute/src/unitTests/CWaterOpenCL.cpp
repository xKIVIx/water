/* Copyright (c) 2018, Aleksandrov Maksim */

#include <vector>

#include "gtest\gtest.h"

#include "CWaterOpenCL.hpp"

CWaterOpenCL *waterOpenCL;

// must first test
TEST(CWaterOpenCL, loadData) {
    ASSERT_NO_THROW(waterOpenCL = new CWaterOpenCL()) << "Fail create waterOpenCL\n";
    int err = 0;
    std::vector<float> vertex = {
        1.000000f, 1.000000f, -0.999999f,
        -1.000000f, 1.000000f, 1.000000f,
        0.999999f, -0.073351f, 1.000001f,
        1.000000f, -1.000000f, -1.000000f,
        2.059009f, -1.000000f, 2.045238f,
        -1.000000f, -1.000000f, 2.436433f,
        -2.934224f, -1.000000f, -1.000000f,
        -1.000000f, 1.000000f, -1.000000f
    };
    std::vector<uint32_t> faces = {
        0, 1, 2,
        3, 2, 4,
        2, 5, 4,
        1, 6, 5,
        0, 7, 1,
        3, 0, 2,
        2, 1, 5,
        1, 7, 6
    };
    ASSERT_NO_THROW(err = waterOpenCL->loadData(vertex, faces));
    ASSERT_EQ(err, 0);
}

bool eqVector(const std::vector<uint32_t> &v1,
              const std::vector<uint32_t> &v2) {
    for(auto iter1 = v1.begin(); iter1 != v1.begin(); ++iter1) {
        bool chek = false;
        for(auto iter2 = v2.begin(); iter2 != v2.begin(); ++iter2) {
            if(*iter1 == *iter2) {
                chek = true;
            }
        }
        if(!chek) {
            return false;
        }
    }
    return true;
}

TEST(CWaterOpenCL, innerFaces) {
    std::vector<uint32_t> result = {0, 6},
                          testResult;
    int err;
    ASSERT_NO_THROW(err = waterOpenCL->getInnerFaces(testResult));
    ASSERT_EQ(err, 0);
    ASSERT_EQ(result.size(), testResult.size());
    ASSERT_TRUE(eqVector(result, testResult));
}

TEST(CWaterOpenCL, borderFaces) {
    std::vector<uint32_t> result = {1, 2, 3, 4, 5, 7},
                          testResult;
    int err;
    ASSERT_NO_THROW(err = waterOpenCL->getBorderFaces(testResult));
    ASSERT_EQ(err, 0);
    ASSERT_EQ(result.size(), testResult.size());
    ASSERT_TRUE(eqVector(result, testResult));
}

TEST(CWaterOpenCL, fractureEdges) {
    std::vector<uint32_t> result = {2, 4, 1, 5},
                          testResult;
    int err;
    ASSERT_NO_THROW(err = waterOpenCL->getFractureEdges(testResult));
    ASSERT_EQ(err, 0);
    ASSERT_EQ(result.size(), testResult.size());
    ASSERT_TRUE(eqVector(result, testResult));
}

// must last test
TEST(CWaterOpenCL, destruct) {
    ASSERT_NO_THROW(delete waterOpenCL) << "Fail delete waterOpenCL\n";
}
