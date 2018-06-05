__kernel void findEdges(__global __read_only unsigned int *faces, 
                        __global             unsigned int *edges) {
    unsigned int facePos = get_global_id(0) * 3,
                 edgePos = get_global_id(0) * 6;

    edges[edgePos] = faces[facePos];
    edges[edgePos + 1] = faces[facePos + 1];

    edges[edgePos + 2] = faces[facePos + 1];
    edges[edgePos + 3] = faces[facePos + 2];

    edges[edgePos + 4] = faces[facePos + 2];
    edges[edgePos + 5] = faces[facePos];
}
bool isEquelEdge(__global __read_only unsigned int *one,
                 __global __read_only unsigned int *two) {
    bool a = one[0] == two[0],
         b = one[1] == two[1],
         c = one[0] == two[1],
         d = one[1] == two[0];
    return (a && b) || (c && d);
}
__kernel void findInnerEdges(__global __read_only unsigned int  *edgesRaw, 
							 __global             unsigned int  *edgesInner,
							 __global             bool          *edgesNoneBorderMark,
                             __global             unsigned int  *countEdgesInner,
                             __global             unsigned int  *facesInner) {                                
    unsigned int idChekedEdge = get_global_id(0),
				 idSecondEdge = get_global_id(1),
			     chekedEdge = idChekedEdge * 2,
                 secondEdge = idSecondEdge * 2;
    if(isEquelEdge(&edgesRaw[chekedEdge], &edgesRaw[secondEdge])) {
        if(chekedEdge < secondEdge) {
            unsigned int writePos = atomic_inc(countEdgesInner) * 2;
            edgesInner[writePos] = edgesRaw[chekedEdge];
            edgesInner[writePos + 1] = edgesRaw[chekedEdge + 1];
            facesInner[writePos] = idChekedEdge / 3;
            facesInner[writePos + 1] = idSecondEdge / 3;
			edgesNoneBorderMark[idChekedEdge] = true;
			edgesNoneBorderMark[idSecondEdge] = true;
        }
    }
}

__kernel void findBorderEdges(__global __read_only unsigned int  *edgesRaw,
                              __global __read_only bool          *edgesNoneBorderMark,
                              __global             unsigned int  *edgesBorder,
                              __global             unsigned int  *countEdgesBorder) {
    unsigned int idEdge = get_global_id(0);
    if(edgesNoneBorderMark[idEdge] == false) {
        unsigned int writePos = atomic_inc(countEdgesBorder);
        edgesBorder[writePos * 2] = edgesRaw[idEdge * 2];
        edgesBorder[writePos * 2 + 1] = edgesRaw[idEdge * 2 + 1];
    }
}

unsigned int getThirdFacePoint (__global __read_only unsigned int  *face,
                                __global __read_only unsigned int  *edge) {
    for(char i = 0; i < 3; i++) {
        if((edge[0] != face[i])&&(edge[1] != face[i])) {
            return face[i];
        }
    }
    return 0xffffffff;
}

__kernel void findFractureEdges(__global __read_only float        *vertex,
								__global __read_only unsigned int *faces,
                                __global __read_only unsigned int *edgesInner,
								__global __read_only unsigned int *facesInner,
                                __global             unsigned int *edgesFracture,
                                __global             unsigned int *countEdgesFracture) {
    unsigned int posEdge = get_global_id(0) * 2,
                 posVertex1 = edgesInner[posEdge] * 3,
                 posVertex2 = edgesInner[posEdge + 1] * 3,
                 posFace1 = facesInner[posEdge] * 3,
                 posFace2 = facesInner[posEdge + 1] * 3;
                 
    float3 vert1 = (float3){vertex[posVertex1],
                            vertex[posVertex1 + 1],
                            vertex[posVertex1 + 2]},
           vert2 = (float3){vertex[posVertex2],
                            vertex[posVertex2 + 1],
                            vertex[posVertex2 + 2]};
                            
    if((vert1.x == vert2.x)&&(vert1.z == vert2.z)) {
        return;
    }
    
    float3 vector1 = vert2 - vert1,
           vector2 = (float3){0.0f,
                              1.0f, 
                              0.0f};
    float3 vector3 = cross(vector1, vector2);
    float A = vector1.y * vector3.z - vector1.z * vector3.y,
          B = vector1.z * vector3.x - vector1.x * vector3.z,
          C = vector1.x * vector3.y - vector1.y * vector3.x,
          D =  vert1.x * A + vert1.y * B + vert1.z * C;
    
    // check first point
    unsigned int posVertex3_1 = getThirdFacePoint(&faces[posFace1], &edgesInner[posEdge]) * 3,
                 posVertex3_2 = getThirdFacePoint(&faces[posFace2], &edgesInner[posEdge]) * 3;;
    float yCup = (D - A * vertex[posVertex3_1] - C * vertex[posVertex3_1 + 2]) / B;
    if(yCup <= vertex[posVertex3_1 + 1]) {
        return;
    }
    
    // chek second point
    
    yCup = (D - A * vertex[posVertex3_2] - C * vertex[posVertex3_2 + 2]) / B;
    if(yCup <= vertex[posVertex3_2 + 1]) {
        return;
    }
    unsigned int writePos = atomic_inc(countEdgesFracture);
    edgesFracture[writePos * 2] = edgesInner[posEdge];
    edgesFracture[writePos * 2 + 1] = edgesInner[posEdge + 1];
}
                
__kernel void deleteDoubleVert(__global __read_only float        *vertex,
							   __global             unsigned int *faces) {
    unsigned int id = get_global_id(0),
                 idCurr = faces[id];
    for(unsigned int i = 0; i < idCurr; i++) {
        if((vertex[i * 3] == vertex[idCurr * 3]) &&
           (vertex[i * 3 + 1] == vertex[idCurr * 3 + 1]) &&
           (vertex[i * 3 + 2] == vertex[idCurr * 3 + 2])) {
            faces[id] = i;
            return;
        }
    }
}

__kernel void getMarkData(__global __read_only unsigned int *markers,
                          __global             unsigned int *result,
                          __global             unsigned int *resultSize) {
    unsigned int i = get_global_id(0);
    if(markers[i] != 0) {
        unsigned int pos = atomic_inc(resultSize);
        result[pos] = i;
    }
}
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

__kernel void findSquare(__global __read_only float         *vertex,
                         __global __read_only unsigned int  *faces,
                         __global __read_only unsigned int  *area,
                         __global             float         *result) {
    unsigned int idArea = get_global_id(0),
                 posFace = area[idArea] * 3;
    float2 vert0 = (float2) {vertex[faces[posFace] * 3],
                             vertex[faces[posFace] * 3 + 2]},
           vert1 = (float2) {vertex[faces[posFace + 1] * 3],
                             vertex[faces[posFace + 1] * 3 + 2]},
           vert2 = (float2) {vertex[faces[posFace + 2] * 3],
                             vertex[faces[posFace + 2] * 3 + 2]};
                             
    result[idArea] = fabs((vert1.x - vert0.x) * (vert2.y - vert0.y) - (vert1.y - vert0.y) * (vert2.x - vert0.x)) / 2.0f;
}

__kernel void findVal(__global __read_only float        *vertex,
                      __global __read_only unsigned int *faces,
                      __global __read_only unsigned int *area,
                      __global __read_only float        *squares,
                                           float        height,
                      __global             float        *result) {
                                  
    unsigned int idArea = get_global_id(0),
                 posFace = area[idArea] * 3;
    float vert0 = vertex[faces[posFace] * 3 + 1],
          vert1 = vertex[faces[posFace + 1] * 3 + 1],
          vert2 = vertex[faces[posFace + 2] * 3 + 1];
                             
    float minHeight = vert0;
    minHeight = min(minHeight, vert1);
    minHeight = min(minHeight, vert2);
    
    if(minHeight >= height) {
        result[idArea] = 0.0f;
        return;
    } 
    
    float maxHeight = vert0;
    maxHeight = max(maxHeight, vert1);
    maxHeight = max(maxHeight, vert2);
    
    if(height < maxHeight) {
        result[idArea] = squares[idArea] * (height - minHeight) / 2.0f;
    } else {
        result[idArea] = squares[idArea] * (maxHeight - minHeight) / 2.0f;
        result[idArea] += squares[idArea] * (height - maxHeight) ;
    }
}
__kernel void getFacesToHeight (__global __read_only float        *vertex,
                                __global __read_only unsigned int *faces,
                                __global __read_only unsigned int *area,
                                                     float         height,
                                __global             unsigned int *resultFaces,
                                __global             float        *resultVertex,
                                __global             unsigned int *resultSize) {
    unsigned int idFace = area[get_global_id(0)] * 3,
                 idVert0 = faces[idFace] * 3,
                 idVert1 = faces[idFace + 1] * 3,
                 idVert2 = faces[idFace + 2] * 3;
    float minHeight = vertex[idVert0 + 1];
    minHeight = min(minHeight, vertex[idVert1 + 1]);
    minHeight = min(minHeight, vertex[idVert2 + 1]);
    
    if(minHeight < height) {
        unsigned int writePos = atomic_add(resultSize, 3),
                     writePosVert = writePos * 3;
        resultVertex[writePosVert] = vertex[idVert0];
        resultVertex[writePosVert + 1] = height;
        resultVertex[writePosVert + 2] = vertex[idVert0 + 2];
        
        resultVertex[writePosVert + 3] = vertex[idVert1];
        resultVertex[writePosVert + 4] = height;
        resultVertex[writePosVert + 5] = vertex[idVert1 + 2];
        
        resultVertex[writePosVert + 6] = vertex[idVert2];
        resultVertex[writePosVert + 7] = height;
        resultVertex[writePosVert + 8] = vertex[idVert2 + 2];
        
        resultFaces[writePos] = writePosVert;
        resultFaces[writePos + 1] = writePosVert + 3;
        resultFaces[writePos + 2] = writePosVert + 6;
    }
}
