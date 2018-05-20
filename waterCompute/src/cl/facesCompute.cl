bool isColise(__global __read_only float *edgeVert0,
              __global __read_only float *edgeVert1,
              __global __read_only float *vert) {
    float z = vert[2],
          z0 = edgeVert0[2],
          z1 = edgeVert1[2],
          x0 = edgeVert0[0],
          x1 = edgeVert1[0],
          x;
    
    if(z1 != z0) {
        x = (z - z0 * (x1 - x0))/(z1 - z0) + x0;
        if(x <= vert[0]) {
            if(clamp(z,
                     min(z1,z0),
                     max(z1,z0))) {
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
    
    for(unsigned int i = 0; i < sizeBorder; i += 2) {
        idVertEdge0 = edges[edgesBorder[i] * 2];
        idVertEdge1 = edges[edgesBorder[i] * 2 + 1];
        if((idVertEdge0 == idChekedVertex)||
           (idVertEdge1 == idChekedVertex)) {
            unsigned int writePos = atomic_inc(countInnerVertex);
            innerVertex[writePos] = idChekedVertex;
            return;           
        } 
        if(isColise(&vertex[idVertEdge0 * 3],
                    &vertex[idVertEdge1 * 3],
                    &vertex[idChekedVertex * 3])) {
            countColise++;
        }
    }
    
    if((countColise % 2) == 1) {
        unsigned int writePos = atomic_inc(countInnerVertex);
        innerVertex[writePos] = idChekedVertex;
    }
}

