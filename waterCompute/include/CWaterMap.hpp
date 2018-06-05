/* Copyright (c) 2018, Aleksandrov Maksim */

#ifndef CWATER_MAP
#define CWATER_MAP

#include <list>
#include <vector>
#include <map>

#include "CWaterOpenCL.hpp"

/**
*
* Class of basic calculations.
*/
class CWaterMap : public CWaterOpenCL {
public:

    void addWater(const float size);

    void getWaterLvls(std::list<std::vector <float>> &vertex,
                      std::list<std::vector <uint32_t>> &faces);

protected:
    int buildMap(const std::list <std::vector<uint32_t>> &borders,
                 const std::list <std::vector<uint32_t>> &areas);
    
private:
    enum State {
        NOT_USE = 0,
        USED = 1<<1
    };

    struct Edge;

    struct Vert {
        std::list <Edge *> edges_;
        std::vector<uint32_t> faces_;
        std::map<float, float> valGrad_;
        float currVal_ = 0.0f,
              square_ = 0.0f;
        State state_ = NOT_USE;
    };

    struct Edge {
        Vert *connectVert_[2];
        State state_ = NOT_USE;
        float weight_;
    };

    void buildGraph(const std::list <std::vector<uint32_t>> borders,
                    const std::vector<float> minHeights);

    void findMSP();

    int findMinPoint(const std::list <std::vector<uint32_t>> &borders,
                     std::vector<float> &heights,
                     std::vector<uint32_t> &result);

    void fillData(const std::vector<float>               &squares, 
                  const std::list<std::vector<uint32_t>> faces,
                  const std::vector<float>               &vals,
                  const std::vector<float>               &heights);

    std::vector<Vert> verts_;
    std::list<Edge> edges_;

};



#endif // CWATER_MAP