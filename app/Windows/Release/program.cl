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