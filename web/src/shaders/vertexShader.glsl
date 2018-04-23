attribute vec3 aVec3_vertexPosition;
attribute vec3 aVec3_normal;

varying vec3 vVec3_normal;
// matrix
uniform mat4 uMat4_project,
             uMat4_cam,
             uMat4_objectPos;
uniform mat3 uMat3_normalTrans;

void main() {
    gl_Position = uMat4_project*
                  uMat4_cam*
                  uMat4_objectPos*
                  vec4 (aVec3_vertexPosition,1.0);
    vVec3_normal = uMat3_normalTrans * aVec3_normal;
}