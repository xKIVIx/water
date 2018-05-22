/* Copyright (c) 2018, Aleksandrov Maksim */

#ifndef CSETTINGS_MANAGER
#define CSETTINGS_MANAGER

#include <mutex>
#include <map>
#include <string>
#include <vector>

/**
 *
 * The class that manages the settings files.
 * 
 * @details Static class. When the program is terminated, 
 *         it is necessary to call method @see CSettingsManager::detach. 
 *         You also need to call the @see CSettingsManager::saveParametrs method 
 *         to write the settings to files.
 * 
 */
 class CSettingsManager {
 public:
     
     /**
      *
      * Function of obtaining parameters.
      * @details Looks for among the loaded categories and looks 
      *          for the required parameter. If the category is not found, 
      *          it tries to load the "category".conf file.
      *          Thread safe function.
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
      * Get integer parametr.
      * @see CSettingsManager::getParametr
      * 
      */
     bool getParametr(const std::string &category,
                      const std::string &dataName,
                      int &parametr);

     /**
      *
      * Writes parameters to files.
      * @details Thread safe function.
      * 
      * @return True if success write.
      * 
      */
     bool saveParametrs();

     /**
      *
      * Set path where stored *.conf files.
      * @details Thread safe function.
      * 
      * @param [in] path Path to dirictory.
      * 
      */
     void setPath(const std::string &path);

     /**
      *
      * Parameter entry function.
      * @details Looks for among the loaded categories and looks 
      *         for the required parameter. If the category is not found, 
      *         it tries to load the "category".conf file.
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
     * Parameter entry function.
     * @details Looks for among the loaded categories and looks 
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
                      const int &parametr);

     /**
      *
      * Get instance.
      * @details Not a thread-safe function.
      */
     static CSettingsManager &instance();

     /**
      *
      * Release of resources.
      * @details Not a thread-safe function.
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