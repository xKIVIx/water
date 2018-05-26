
bool isColise(float2 vert0,
              float2 vert1,
              float2 vert) {
    if(vert1.y != vert0.y) {
        float x = (vert.y - vert0.y) * (vert1.x - vert0.x) / (vert1.y - vert0.y) + vert0.x;
        if(x < vert.x) {
            if((vert.y >= min(vert1.y, vert0.y)) && 
               (max(vert1.y, vert0.y) > vert.y)) {
                return true;
            }
        }
    }
    return false;
}

__kernel void findInnerVertex(__global __read_only unsigned int *edges,
                              __global __read_only unsigned int *edgesBorder,
                                                   unsigned int sizeBorder,
                              __global __read_only float        *vertex,
                              __global             unsigned int *innerVertex,
                              __global             unsigned int *countInnerVertex) {
    unsigned int idChekedVertex = get_global_id(0),
                 countColise = 0,
                 idVertEdge0,
                 idVertEdge1;
    
    for(unsigned int i = 0; i < sizeBorder; i ++) {
        idVertEdge0 = edges[edgesBorder[i] * 2];
        idVertEdge1 = edges[edgesBorder[i] * 2 + 1];
        if((idVertEdge0 == idChekedVertex)||
           (idVertEdge1 == idChekedVertex)) {
            unsigned int writePos = atomic_inc(countInnerVertex);
            innerVertex[writePos] = idChekedVertex;
            return;           
        }
        float2 vert0 = (float2){vertex[idVertEdge0 * 3],  
                                vertex[idVertEdge0 * 3 + 2]},
               vert1 = (float2){vertex[idVertEdge1 * 3], 
                                vertex[idVertEdge1 * 3 + 2]},
               vert2 = (float2){vertex[idChekedVertex * 3], 
                                vertex[idChekedVertex * 3 + 2]};
        if(isColise(vert0, vert1, vert2)) {
            countColise++;
        }
    }
    
    if((countColise % 2) == 1) {
        unsigned int writePos = atomic_inc(countInnerVertex);
        innerVertex[writePos] = idChekedVertex;
    }
}

__kernel void findInnerFaces(__global __read_only unsigned int *faces,
                                                  unsigned int sizeFaces,
                             __global __read_only unsigned int *innerVertex,
                             __global             unsigned int *innerFaces,
                             __global             unsigned int *countInnerFaces) {
    unsigned int idVert0 = get_global_id(0),
                 idVert1 = get_global_id(1),
                 idVert2 = get_global_id(2);
    
    if((idVert0 == idVert1) || 
       (idVert1 == idVert2) ||
       (idVert0 == idVert2)) {
        return;
    }
    for(unsigned int posFace = 0; posFace < sizeFaces; posFace += 3) {
        // 0 1 2
        bool a = (innerVertex[idVert0] == faces[posFace]) && 
                 (innerVertex[idVert1] == faces[posFace + 1]) && 
                 (innerVertex[idVert2] == faces[posFace + 2]);
        if(a) {
            unsigned int writePos = atomic_inc(countInnerFaces);
            innerFaces[writePos] = posFace / 3;
        }
    }
}

__kernel void countColise(__global __read_only unsigned int  *area,
                          __global             unsigned char *counters) {
    unsigned int i = get_global_id(0);
    counters[area[i]] = 1;
}

__kernel void removeCommunityAreas(__global __read_only unsigned char *countersFirst,
                                   __global __read_only unsigned char *countersSecond,
                                   __global             unsigned int  *resultFirst,
                                   __global             unsigned int  *resultSecond,
                                   __global             unsigned int  *resultThird,
                                   __global             unsigned int  *resultSizes) {
    unsigned int i = get_global_id(0),
                 writePos;
    if((countersFirst[i] + countersSecond[i]) == 2) {
        writePos = atomic_inc(&resultSizes[2]);
        resultThird[writePos] = i;
    } else if(countersFirst[i] == 1) {
        writePos = atomic_inc(&resultSizes[0]);
        resultFirst[writePos] = i;
    } else if(countersSecond[i] == 1) {
        writePos = atomic_inc(&resultSizes[1]);
        resultSecond[writePos] = i;
    }
}

__kernel void findUnionVertex(__global __read_only  unsigned int  *faces,
                              __global __read_only  unsigned int  *firstArea,
                              __global __read_only  unsigned int  *secondAera,
                              __global              unsigned int  *unionVertexMark) {
                                  
    unsigned int idFirstFace = get_global_id(0),
                 idSecondFace = get_global_id(1);
    for(unsigned char i = 0; i < 3; i++) {
        for(unsigned char k = 0; k < 3; k++) {
            if(faces[firstArea[idFirstFace] * 3 + i] == 
               faces[secondAera[idSecondFace] * 3 + k]) {
                atomic_max(&unionVertexMark[firstArea[idFirstFace] * 3 + i], 1);
            }
        }
    }
    
}
