precision mediump float;
varying float red_component;
void main() {
    gl_FragColor = vec4(red_component, 0.66, 0.0, 1.0);
}