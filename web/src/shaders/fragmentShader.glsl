precision mediump float;

uniform vec3 uVec3_reverseLightDirection;

varying vec3 vVec3_normal;
void main() {
    vec3 normal = normalize(vVec3_normal);
    float light = max(dot(normal, uVec3_reverseLightDirection), 0.0);
    light = min(light, 1.0);
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    gl_FragColor.rgb *= light;
}