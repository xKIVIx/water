/* Copyright (c) 2018, Aleksandrov Maksim */

#ifndef CWATER_COMPUTE
#define CWATER_COMPUTE

#include <vector>
#include <map>

#include "CWaterMap.hpp"
/**
 *
 * Class of basic calculations.
 */
class CWaterCompute : public CWaterMap{
public:
    /**
     * Prepare data to calculates water levels.
     * @param vertex Buffer for output data about vertices.
     * @param Buffer for output data about polygons.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int prepareData();
    /**
     * Cleaning of internal data.
     */
    void clear();

    
private:


    /**
     *  Search for container boundaries.
     * @param borders Buffer for output data.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int findBorderHoles(std::list <std::vector<uint32_t>> &borders);
};



#endif // CWATER_COMPUTE