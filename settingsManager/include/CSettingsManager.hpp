/* Copyright (c) 2018, Aleksandrov Maksim */

#ifndef CSETTINGS_MANAGER
#define CSETTINGS_MANAGER

#include <mutex>
#include <map>
#include <string>
#include <vector>

/**
 *
 * @brief The class that manages the settings files.
 * 
 * @detail Static class. When the program is terminated, 
 *         it is necessary to call method @see CSettingsManager::detach. 
 *         You also need to call the @see CSettingsManager::saveParametrs method 
 *         to write the settings to files.
 * 
 */
 class CSettingsManager {
 public:
     
     /**
      *
      * @brief Function of obtaining parameters.
      * @detail Looks for among the loaded categories and looks 
      *         for the required parameter. If the category is not found, 
      *         it tries to load the <category> .conf file.
      *         Thread safe function.
      * 
      * @param [in] category The name of the category (File name) 
      *                      that contains the parameter.
      * @param [in] dataName Parametr name.
      * @param [out] parametr Requested data.
      * 
      * @return true Returns true if the parameter 
      *              was found successfully, otherwise false.
      * 
      */
     bool getParametr(const std::string &category,
                      const std::string &dataName,
					  std::string &parametr);

     /**
      * @brief Get integer parametr.
      * @see CSettingsManager::getParametr
      * 
      */
     bool getParametr(const std::string &category,
                      const std::string &dataName,
                      int &parametr);

     /**
      *
      * @brief Writes parameters to files.
      * @detail Thread safe function.
      * 
      * @return True if success write.
      * 
      */
     bool saveParametrs();

     /**
      *
      * @brief Set path where stored *.conf files.
      * @detail Thread safe function.
      * 
      * @param [in] path Path to dirictory.
      * 
      */
     void setPath(const std::string &path);

     /**
      *
      * @brief Parameter entry function.
      * @detail Looks for among the loaded categories and looks 
      *         for the required parameter. If the category is not found, 
      *         it tries to load the <category>.conf file.
      *         Thread safe function.
      * 
      * @param [in] category The name of the category (File name) 
      *                      that contains the parameter.
      * @param [in] dataName Parametr name.
      * @param [in] parametr New data.
      * 
      * @return true Returns true if the parameter 
      *              was set successfully, otherwise false.
      * 
      */
     bool setParametr(const std::string &category,
                      const std::string &dataName,
                      const std::string &parametr);

     /**
      *
      * @brief Get instance.
      * @detail Not a thread-safe function.
      */
     static CSettingsManager &instance();

     /**
      *
      * @brief Release of resources.
      * @detail Not a thread-safe function.
      */
     static void detach();
 private:

     CSettingsManager();
     ~CSettingsManager();

     std::mutex manegerMutex_;
     std::string path_;
     std::map<std::string, std::map<std::string, std::string>> data_;
     static CSettingsManager *manegerPtr_;
     bool isSave = true;
 };



#endif // CSETTINGS_MANAGER