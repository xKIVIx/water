/*
 * Settings for webGl
 */

// Context
/** @type {number[]} background fill color [R,G,B,A] */
export var clearColor = [1.0, 1.0, 1.0, 1.0];

// Camera
/** @type {number} minimal distance */
export var minDist = 0.1;
/** @type {number} maximal distance */
export var maxDist = 50;
/** @type {number[]} camera position [x,y,z] */
export var camPosition = [0.0, 0.0, -2.0];
/** @type {number[]} camera rotate [axisX,axisY,axisZ] */
export var camRotate = [0.0, 0.0, 0.0];

// Light
/** @type {number[]} light direction */
export var lightDir = [0.0, 0.0, 1.0];