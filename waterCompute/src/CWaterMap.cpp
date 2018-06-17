
#include <iostream>
#include <algorithm>

#include "CWaterMap.hpp"

#define errorMessage(message, failCode) std::cout << message << ": " << failCode << std::endl

const uint32_t COUNT_ITERATIONS = 20;

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
    heights.push_back(edges_.front().weight_);
    for(auto iter = edges_.begin(); iter != edges_.end(); ++iter) {
        if(iter->weight_ != heights.back()) {
            heights.push_back(iter->weight_);
        } 
    }
    heights.push_back(heights.back() + 20.0f);
    std::vector<float> squares,
                       vals;

    err = computeAreaData(areas, heights, squares, vals);
    if(err != 0) {
        errorMessage("Fail compute areas data", err);
        return err;
    }

    fillData(squares, areas, vals, heights);

    return 0;
}

void CWaterMap::fillData(const std::vector<float>               &squares, 
                         const std::list<std::vector<uint32_t>> faces,
                         const std::vector<float>               &vals,
                         const std::vector<float>               &heights) {
    uint32_t i = 0;
    auto iterFaces = faces.begin();
    for(auto iterVerts = verts_.begin(); iterVerts != verts_.end(); ++iterVerts) {
        
        iterVerts->faces_ = *iterFaces;
        ++iterFaces;
        iterVerts->square_ = squares[i];
        for(uint32_t k = 0; k < heights.size(); k++) {
            iterVerts->valGrad_.insert(std::pair<float, float>(heights[k], vals[i * heights.size() + k]));
        }
        i++;
    }
}

void CWaterMap::addWater(const float size) {
    
    for(auto iterVert = verts_.begin(); iterVert != verts_.end(); ++iterVert) {
        iterVert->state_ = NOT_USE;
        iterVert->currVal_ += size * iterVert->square_;
    }
    for(uint32_t i = 0; i < COUNT_ITERATIONS; i++) {
        for(auto iterEdge = edges_.begin(); iterEdge != edges_.end(); ++iterEdge) {
            Vert *oneVert = iterEdge->connectVert_[0],
                 *twoVert = iterEdge->connectVert_[1];

            auto val0 = oneVert->valGrad_.find(iterEdge->weight_),
                 val1 = twoVert->valGrad_.find(iterEdge->weight_);

            if(val0->second < oneVert->currVal_) {
                twoVert->currVal_ += oneVert->currVal_ - val0->second;
                oneVert->currVal_ = val0->second;
                if(val1->second < twoVert->currVal_) {
                    float v = twoVert->currVal_ - val1->second;
                    twoVert->currVal_ = val1->second;
                    val0++;
                    val1++;
                    float unionVal = val0->second + val1->second,
                          k = v / unionVal;

                    twoVert->currVal_ += val1->second * k;
                    oneVert->currVal_ += val0->second * k;
                }
            } else if(val1->second < twoVert->currVal_) {
                oneVert->currVal_ += twoVert->currVal_ - val1->second;
                twoVert->currVal_ = val1->second;
                if(val0->second < oneVert->currVal_) {
                    float v = oneVert->currVal_ - val0->second;
                    oneVert->currVal_ = val0->second;
                    val0++;
                    val1++;
                    float unionVal = val0->second + val1->second;
                    twoVert->currVal_ += (val1->second / unionVal) * v;
                    oneVert->currVal_ += (val0->second / unionVal) * v;
                }
            } else {
                continue;
            }

        }
    }
}

void CWaterMap::getWaterLvls(std::list<std::vector <float>> &vertex,
                             std::list<std::vector <uint32_t>> &faces) {
    for(auto iter = verts_.begin(); iter != verts_.end(); ++iter) {
        vertex.emplace_back();
        faces.emplace_back();
        auto iterVal = iter->valGrad_.begin();
        for(; iterVal != iter->valGrad_.end(); ++iterVal) {
            if(iterVal->second > iter->currVal_) {
                break;
            }
        }
        float height;
        if(iterVal == iter->valGrad_.end()) {
            auto elem = iter->valGrad_.rbegin();
            height = elem->first * (iter->currVal_ / elem->second);
        } else {
            height = iterVal->first * (iter->currVal_ / iterVal->second);
        }
        int err = getFacesToHeight(iter->faces_, 
                                   height, 
                                   faces.back(), 
                                   vertex.back());
        if(err != 0) {
            vertex.pop_back();
            faces.pop_back();
        }
    }
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
    edges_.sort([](const Edge &one, const Edge &two) {
        return one.weight_ < two.weight_;
    });
    uint32_t countEdgesCurr = 0,
        countEdgesNeed = verts_.size() - 1;
    for(auto iter = edges_.begin(); iter != edges_.end(); ++iter) {
        if( (iter->connectVert_[0]->state_ != USED) ||
            (iter->connectVert_[1]->state_ != USED)) {

            iter->connectVert_[0]->state_ = USED;
            iter->connectVert_[1]->state_ = USED;
            iter->state_ = USED;
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
    edges_.remove_if([](const Edge &item) {
        return item.state_ == NOT_USE;
    });
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