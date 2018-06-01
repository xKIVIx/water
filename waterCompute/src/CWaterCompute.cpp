/* Copyright (c) 2018, Aleksandrov Maksim */

#include <iostream>

#include "CSettingsManager.hpp"

#include "CWaterCompute.hpp"

#define getSittingParametr(name, out) CSettingsManager::instance().getParametr("waterCompute", name, out)
#define setSittingParametr(name, out) CSettingsManager::instance().setParametr("waterCompute", name, out)

#define errorMessage(message, failCode) std::cout << message << ": " << failCode << std::endl

enum StateEdge {
    NOT_USE = 0,
    TIME_USED = 1,
    USED = 1<<1,
    END = 1<<2
};

struct Edge;

struct Vert {
    std::list <Edge *> edges_;
    StateEdge state_ = NOT_USE;
};

struct Edge {
    Vert *connectVert_[2];
    StateEdge state_ = NOT_USE;
    float weight_;
};

const char MASK_BEGIN_EDGE = TIME_USED | USED,
           MASK_CHEK_NEXT_EDGE = ~USED;

void buildGraph(const std::list <std::vector<uint32_t>> borders,
                const std::vector<float> minHeights,
                std::vector<Vert> &verts,
                std::list<Edge> &edges) {
    auto currVert = verts.begin(),
         secondVert = currVert;
    auto iterHeight = minHeights.begin();
    secondVert++;
    for(auto iter = borders.begin(); iter != borders.end(); ++iter) {
        if(!iter->empty()) {
            edges.emplace_back();
            edges.back().weight_ = *iterHeight;
            edges.back().connectVert_[0] = &(*currVert);
            currVert->edges_.push_back(&edges.back());
            edges.back().connectVert_[1] = &(*secondVert);
            secondVert->edges_.push_back(&edges.back());
        }
        iterHeight++;
        secondVert++;
        if(secondVert == verts.end()) {
            currVert++;
            secondVert = currVert;
            secondVert++;
        }
    }
}

void findMSP(std::vector<Vert> &verts,
             std::list<Edge> &edges) {
    std::list<Edge *> sortedEdges;
    for(auto iter = edges.begin(); iter != edges.end(); iter++) {
        sortedEdges.push_back(&(*iter));
    }
    sortedEdges.sort([](Edge *one, Edge *two) {
        return one->weight_ < two->weight_;
    });
    uint32_t countEdgesCurr = 0,
             countEdgesNeed = verts.size() - 1;
    for(auto iter = sortedEdges.begin(); iter != sortedEdges.end(); ++iter) {
        if(((*iter)->connectVert_[0]->state_ != USED) ||
            ((*iter)->connectVert_[1]->state_ != USED)) {

            (*iter)->connectVert_[0]->state_ = USED;
            (*iter)->connectVert_[1]->state_ = USED;
            (*iter)->state_ = USED;
            countEdgesCurr++;
            if(countEdgesCurr == countEdgesNeed) {
                break;
            }
        }
    }
    for(auto iterVert = verts.begin(); iterVert != verts.end(); ++iterVert) {
        iterVert->edges_.remove_if([](Edge *item) {
            return item->state_ == NOT_USE;
        });
    }
}

inline void roadMatOut(const std::vector<uint32_t> &edges) {
    for(uint32_t i = 0; i < edges.size(); i+=2) {
        for(uint32_t j = 0; j < edges.size(); j += 2) {
            if(i == j) {
                std::cout << "0, ";
                continue;
            }
            if((edges[i] == edges[j]) ||
                (edges[i] == edges[j + 1]) ||
               (edges[i + 1] == edges[j]) ||
               (edges[i + 1] == edges[j + 1])) {
                std::cout << "1, ";
            } else {
                std::cout << "0, ";
            }
        }
        std::cout << std::endl;
    }
}

int CWaterCompute::computeWaterLvl(std::vector <float> &vertex,
                                   std::vector <uint32_t> &faces) {
    std::list <std::vector<uint32_t>> borders,
                                      innerFaces,
                                      newArea;
    findBorderHoles(borders);
    computeInnerFaces(borders, innerFaces);
    int err = removeCommunityAreas(innerFaces, newArea);
    if(err != 0) {
        return err;
    }
    while(!newArea.empty()) {
        std::list <std::vector<uint32_t>> tmp;
        err = removeCommunityAreas(newArea, tmp);
        if(err != 0) {
            return err;
        }
        for(auto iter = newArea.begin(); iter != newArea.end(); ++iter) {
            innerFaces.emplace_back();
            innerFaces.back().swap(*iter);
        }
        newArea.swap(tmp);
    }
    borders.clear();
    err = findUnionVertex(innerFaces, borders);
    if(err != 0) {
        errorMessage("Fail find union vertex", err);
        return err;
    }
    std::vector<float> minHeights;
    std::vector<uint32_t> minVertIds;
    findMinPoint(borders, minHeights, minVertIds);
    std::vector<Vert> verts(innerFaces.size());
    std::list<Edge> edges;
    buildGraph(borders, minHeights, verts, edges);
    findMSP(verts, edges);
    std::vector <float> heights;
    float maxHeight = edges.back().weight_;
    for(auto iter = edges.begin(); iter != edges.end(); ++iter) {
        if(iter->state_ != USED) {
            continue;
        }
        bool chek = false;
        for(auto iterChek = heights.begin(); iterChek != heights.end(); ++iterChek) {
            if(*iterChek == iter->weight_) {
                chek = true;
                break;
            }
        }
        if(!chek) {
            heights.push_back(iter->weight_);
            if(iter->weight_ > maxHeight) {
                maxHeight = iter->weight_;
            }
        }
    }
    heights.push_back(maxHeight + 20.0f);
    std::vector<float> squares,
                       vals;
    err = computeAreaData(innerFaces, heights, squares, vals);
    if(err != 0) {
        errorMessage("Fail compute areas data", err);
        return err;
    }
    // todo
    return 0;
}

int CWaterCompute::findMinPoint(const std::list<std::vector<uint32_t>>& borders,
                                std::vector<float> &heights,
                                std::vector<uint32_t> &result) {
    std::vector <float> vertex;
    int err = getVertex(vertex);
    if(err != 0) {
        errorMessage("Fail get vertex", err);
        return err;
    }
    for(auto iterFirst = borders.begin(); 
        iterFirst != borders.end(); 
        ++iterFirst) {

        if(iterFirst->empty()) {
            heights.push_back(0.0f);
            result.push_back(0);
            continue;
        }

        uint32_t minId = iterFirst->back();
        float min = vertex[minId * 3 + 1];
        for(auto iterSecond = iterFirst->begin(); 
            iterSecond != iterFirst->end(); 
            ++iterSecond) {

            if(vertex[*iterSecond * 3 + 1] < min) {
                min = vertex[*iterSecond * 3 + 1];
                minId = *iterSecond;
            }
        }
        heights.push_back(min);
        result.push_back(minId);
    }
    return 0;
}

void CWaterCompute::clear() {
    clearOpenCl();
}

inline bool isNotRepertVert(const std::vector <uint32_t> &edges,
                            const std::vector <uint32_t> &cycle,
                            const uint32_t vert) {
    for(auto iter = cycle.begin(); iter != cycle.end(); iter++) {
        if(edges[*iter * 2] == vert) {
            return false;
        }
        if(edges[*iter * 2 + 1] == vert) {
            return false;
        } 
    }
    return true;
}

inline void clearStatesEdges(std::vector <char> &states) {
    for(auto iter = states.begin(); iter != states.end(); ++iter) {
        *iter &= ~TIME_USED;
    }
}

inline void useEdges(std::vector <char> &states,
                     const std::vector <uint32_t> &cycle) {
    for(auto iter = cycle.begin(); iter != cycle.end(); ++iter) {
        states[*iter] |= USED;
    }
}


int CWaterCompute::findBorderHoles(std::list <std::vector<uint32_t>> &cycles) {
    std::vector <uint32_t> cycle;
    std::vector <char> statesEdges;
    uint32_t sizeFractureEdges;
    statesEdges.resize(edgesHoleBorders_.size() / 2);
    cycle.reserve(edgesHoleBorders_.size() / 2);
    for(uint32_t idEdge = 0; idEdge < edgesHoleBorders_.size() / 2; idEdge++) {
        if(statesEdges[idEdge] != 0) {
            continue;
        }
        cycle.push_back(idEdge);
        uint32_t idBackVert = edgesHoleBorders_[idEdge * 2],
                 idFrontVert = edgesHoleBorders_[idEdge * 2 + 1];
        statesEdges[idEdge] |= MASK_BEGIN_EDGE;
        while(true) {
            StateEdge state = END;
            bool isFind = false;
            for(uint32_t i = 0; i < edgesHoleBorders_.size(); i++) {
                if(i / 2 == idEdge) {
                    continue;
                }
                if((idFrontVert == edgesHoleBorders_[i]) && (cycle.back() != (i / 2))) {
                    if((statesEdges[i / 2] & MASK_CHEK_NEXT_EDGE) == 0) {
                        uint32_t chekedVert = edgesHoleBorders_[((i % 2) == 1) ? i - 1: i + 1];

                        if(chekedVert == idBackVert) {
                            cycle.push_back(i / 2);
                            cycles.push_back(cycle);
                            clearStatesEdges(statesEdges);
                            useEdges(statesEdges, cycle);
                            cycle.clear();
                            isFind = true;
                            break;
                        }

                        if(isNotRepertVert(edgesHoleBorders_, cycle, chekedVert)) {
                            idFrontVert = chekedVert;
                            statesEdges[i / 2] |= TIME_USED;
                            cycle.push_back(i / 2);
                            i = 0;
                        } else {
                            state = TIME_USED;
                        }
                    }

                    if((statesEdges[i / 2] & TIME_USED) == TIME_USED) {
                        state = TIME_USED;
                    }
                }
            }
            if(isFind) break;
            statesEdges[cycle.back()] |= state;
            cycle.pop_back();
            if(cycle.empty()) {
                clearStatesEdges(statesEdges);
                break;
            } else {
                idFrontVert = edgesHoleBorders_[(idFrontVert == edgesHoleBorders_[cycle.back() * 2]) ?
                    cycle.back() * 2 + 1 :
                    cycle.back() * 2];               
            }
        }
    }
    return 0;
}
