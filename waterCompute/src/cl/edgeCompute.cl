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

__constant float COMPUTE_ERR = 0.00000001f;

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
    float yCup = (D - A * vertex[posVertex3_1] - C * vertex[posVertex3_1 + 2]) / B - COMPUTE_ERR;
    if(yCup < vertex[posVertex3_1 + 1]) {
        return;
    }
    
    // chek second point
    
    yCup = (D - A * vertex[posVertex3_2] - C * vertex[posVertex3_2 + 2]) / B - COMPUTE_ERR;
    if(yCup < vertex[posVertex3_2 + 1]) {
        return;
    }
    unsigned int writePos = atomic_inc(countEdgesFracture);
    edgesFracture[writePos * 2] = edgesInner[posEdge];
    edgesFracture[writePos * 2 + 1] = edgesInner[posEdge + 1];
}
                