__kernel void findEdges(__global __read_only unsigned int *faces, 
                        __global             unsigned int *edges) {
    unsigned int facePos = get_global_id(0) * 3,
                 edgePos = get_global_id(0) * 6;

    edges[edgePos] = min(faces[facePos], faces[facePos + 1]);
    edges[edgePos + 1] = max(faces[facePos], faces[facePos + 1]);

    edges[edgePos + 2] = min(faces[facePos + 1], faces[facePos + 2]);
    edges[edgePos + 3] = max(faces[facePos + 1], faces[facePos + 2]);

    edges[edgePos + 4] = min(faces[facePos + 2], faces[facePos]);
    edges[edgePos + 5] = max(faces[facePos + 2], faces[facePos]);
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
    if((edgesRaw[chekedEdge] == edgesRaw[secondEdge])&&
       (edgesRaw[chekedEdge + 1] == edgesRaw[secondEdge + 1])) {
        if(chekedEdge < secondEdge) {
            unsigned int writePos = atomic_inc(countEdgesInner) * 2;
            edgesInner[writePos] = edgesRaw[chekedEdge];
            edgesInner[writePos + 1] = edgesRaw[chekedEdge + 1];
            facesInner[writePos] = idChekedEdge / 3;
            facesInner[writePos + 1] = idSecondEdge / 3;
			edgesNoneBorderMark[chekedEdge / 2] = true;
			edgesNoneBorderMark[secondEdge / 2] = true;
        }
    }
}

__kernel void findBorderEdges(__global __read_only unsigned int  *edgesRaw,
                              __global __read_only bool          *edgesNoneBorderMark,
                              __global             unsigned int  *edgesBorder,
                              __global             unsigned int  *facesBorder,
                              __global             unsigned int  *countEdgesBorder) {
    unsigned int idEdge = get_global_id(0);
    if(edgesNoneBorderMark[idEdge] == 0) {
        unsigned int writePos = atomic_inc(countEdgesBorder);
        edgesBorder[writePos * 2] = edgesRaw[idEdge * 2];
        edgesBorder[writePos * 2 + 1] = edgesRaw[idEdge * 2 + 1];
        facesBorder[writePos] = idEdge / 3;
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
                              vert2.y, 
                              0.0f};
    float3 vector3 = cross(vector1, vector2);
    float A = vector1.y * vector3.z - vector1.z * vector3.y,
          B = vector1.z * vector3.x - vector1.x * vector3.z,
          C = vector1.x * vector3.y - vector1.y * vector3.x,
          D =  vert1.x * A + vert1.y * B + vert1.z * C;
    
    // check first point
    unsigned int posVertex3 = getThirdFacePoint(&faces[posFace1], &edgesInner[posEdge]) * 3;
    float yCup = (D - A * vertex[posVertex3] - C * vertex[posVertex3 + 2]) / B;
    if(yCup <= vertex[posVertex3 + 1]) {
        return;
    }
    
    // chek second point
    posVertex3 = getThirdFacePoint(&faces[posFace2], &edgesInner[posEdge]) * 3;
    yCup = (D - A * vertex[posVertex3] - C * vertex[posVertex3 + 2]) / B;
    if(yCup <= vertex[posVertex3 + 1]) {
        return;
    }
    unsigned int writePos = atomic_inc(countEdgesFracture);
    edgesFracture[writePos] = posEdge / 2;
}

__kernel void computeRoadMatrix(__global __read_only unsigned int *edges,
                                __global             bool         *roadMatrix,
                                __global             unsigned int *countRoad) {
    unsigned posChekedEdge = get_global_id(0) * 2,
             posSecondEdge = get_global_id(1) * 2,
             countIter = get_global_size(0);
    if(posChekedEdge != posSecondEdge) {
            if((edges[posChekedEdge] == edges[posSecondEdge])||
           (edges[posChekedEdge] == edges[posSecondEdge + 1])||
           (edges[posChekedEdge + 1] == edges[posSecondEdge])||
           (edges[posChekedEdge + 1] == edges[posSecondEdge + 1])) {
            roadMatrix[countIter*posChekedEdge + posSecondEdge] = true;
            atomic_inc(&countRoad[posChekedEdge]);
        }
    }
    roadMatrix[countIter*posChekedEdge + posSecondEdge] = false;
}                             