attribute vec3 aVec3_vertexPosition;
varying float red_component;
// matrix
uniform mat4 uMat4_project,
             uMat4_cam,
             uMat4_objectPos;

void main() {
    gl_Position = uMat4_project*
                  uMat4_cam*
                  uMat4_objectPos*
                  vec4 (aVec3_vertexPosition,1.0);
    red_component = (1.0 + aVec3_vertexPosition[1]) / 2.0;
}