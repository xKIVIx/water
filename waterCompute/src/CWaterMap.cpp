
#include <iostream>
#include <algorithm>

#include "CWaterMap.hpp"

#define errorMessage(message, failCode) std::cout << message << ": " << failCode << std::endl

int CWaterMap::buildMap(const std::list<std::vector<uint32_t>> &borders,
                        const std::list<std::vector<uint32_t>> &areas) {
    std::vector<float> minHeights;
    std::vector<uint32_t> minVertIds;
    int err = findMinPoint(borders, minHeights, minVertIds);
    if(err != 0) {
        errorMessage("Fail find min point", err);
        return err;
    }
    std::vector<Vert> newVerts(areas.size());
    verts_.swap(newVerts);
    edges_.clear();
    buildGraph(borders, minHeights);
    findMSP();
    std::vector <float> heights;
    float maxHeight = edges_.back().weight_;
    for(auto iter = edges_.begin(); iter != edges_.end(); ++iter) {
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
    std::sort(heights.begin(), heights.end());
    std::vector<float> squares,
                       vals;

    err = computeAreaData(areas, heights, squares, vals);
    if(err != 0) {
        errorMessage("Fail compute areas data", err);
        return err;
    }

    return 0;
}

void CWaterMap::buildGraph(const std::list <std::vector<uint32_t>> borders,
                           const std::vector<float> minHeights) {
    auto currVert = verts_.begin(),
        secondVert = currVert;
    auto iterHeight = minHeights.begin();
    secondVert++;
    for(auto iter = borders.begin(); iter != borders.end(); ++iter) {
        if(!iter->empty()) {
            edges_.emplace_back();
            edges_.back().weight_ = *iterHeight;
            edges_.back().connectVert_[0] = &(*currVert);
            currVert->edges_.push_back(&edges_.back());
            edges_.back().connectVert_[1] = &(*secondVert);
            secondVert->edges_.push_back(&edges_.back());
        }
        iterHeight++;
        secondVert++;
        if(secondVert == verts_.end()) {
            currVert++;
            secondVert = currVert;
            secondVert++;
        }
    }
}

void CWaterMap::findMSP() {
    std::list<Edge *> sortedEdges;
    for(auto iter = edges_.begin(); iter != edges_.end(); iter++) {
        sortedEdges.push_back(&(*iter));
    }
    sortedEdges.sort([](Edge *one, Edge *two) {
        return one->weight_ < two->weight_;
    });
    uint32_t countEdgesCurr = 0,
        countEdgesNeed = verts_.size() - 1;
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
    for(auto iterVert = verts_.begin(); iterVert != verts_.end(); ++iterVert) {
        iterVert->edges_.remove_if([](Edge *item) {
            return item->state_ == NOT_USE;
        });
    }
}

int CWaterMap::findMinPoint(const std::list<std::vector<uint32_t>>& borders,
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