
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