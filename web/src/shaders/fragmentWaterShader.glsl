precision mediump float;

uniform vec3 uVec3_reverseLightDirection;
uniform vec4 uVec4_color;

varying vec3 vVec3_normal;
void main() {
    vec3 normal = normalize(vVec3_normal);
    float light = max(dot(normal, uVec3_reverseLightDirection), 0.05);
    light = min(light, 1.0);
    gl_FragColor = uVec4_color;
    gl_FragColor.rgb *= light;
}