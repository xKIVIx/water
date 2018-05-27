/* Copyright (c) 2018, Aleksandrov Maksim */

#ifndef CWATER_COMPUTE
#define CWATER_COMPUTE

#include <vector>

#include "CWaterOpenCL.hpp"
/**
 *
 * Class of basic calculations.
 */
class CWaterCompute:public CWaterOpenCL {
public:
    /**
     * Calculates water levels.
     * @param vertex Buffer for output data about vertices.
     * @param Buffer for output data about polygons.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int computeWaterLvl(std::vector <float> &vertex,
                        std::vector <uint32_t> &faces);
    /**
     * Cleaning of internal data.
     */
    void clear();

    
private:
    struct CTransferPoint {
        uint32_t target_;
        float capVal_;
    };
    struct CHole {
        std::vector<uint32_t> area_;
        float currVal_,
              capVal_,
              square_;
    };
    CHole hole_;

    /**
     *  Search for container boundaries.
     * @param borders Buffer for output data.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int findBorderHoles(std::list <std::vector<uint32_t>> &borders);

    int findMinPoint(const std::list <std::vector<uint32_t>> &borders,
                     std::vector<float> &heights,
                     std::vector<uint32_t> &result);
};



#endif // CWATER_COMPUTE