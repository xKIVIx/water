/* Copyright (c) 2018, Aleksandrov Maksim */

#ifndef CWATER_MAP
#define CWATER_MAP

#include <list>
#include <vector>

#include "CWaterOpenCL.hpp"

/**
*
* Class of basic calculations.
*/
class CWaterMap : public CWaterOpenCL {
public:

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

    std::vector<Vert> verts_;
    std::list<Edge> edges_;

};



#endif // CWATER_MAP