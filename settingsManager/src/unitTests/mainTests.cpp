/* Copyright (c) 2018, Aleksandrov Maksim */

#include "gtest/gtest.h"
#include "CSettingsManager.hpp"

TEST(main, set_get) {
    std::string category = "testCategory",
                dataName = "testDataName",
                parametr = "testParamtr";
    ASSERT_TRUE( CSettingsManager::instance().setParametr(category,
                                                           dataName,
                                                           parametr)) <<
        "Fail set data\n";
    std::string testData;
    ASSERT_TRUE( CSettingsManager::instance().getParametr(category,
                                                           dataName,
                                                           testData)) << 
        "Fail get data\n";
    ASSERT_STREQ(testData.c_str(), parametr.c_str()) << "Uncorrect data\n";
    CSettingsManager::detach();
}

TEST(main, file_save) {
    std::string category = "testCategory",
                dataName = "testDataName",
                parametr = "testParamtr";
    ASSERT_TRUE( CSettingsManager::instance().setParametr(category,
                                                           dataName,
                                                           parametr)) <<
        "Fail set data\n";
    ASSERT_TRUE( CSettingsManager::instance().saveParametrs() ) << 
        "Fail save data\n";
    CSettingsManager::detach();
    std::string testData;
    ASSERT_TRUE( CSettingsManager::instance().getParametr(category,
                                                           dataName,
                                                           testData)) << 
        "Fail get data\n";
    ASSERT_STREQ(testData.c_str(), parametr.c_str()) << "Uncorrect data\n";
    CSettingsManager::detach();
}