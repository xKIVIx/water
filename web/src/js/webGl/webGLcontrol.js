/* Copyright (c) 2018, Aleksandrov Maksim */

import {vec2, mat4, vec3} from "./../includes/GLMatrix/gl-matrix.js";
import {webGLcontext} from "./webGl.js";
import {keys as ControlSittings} from "./../defaultSettings/control.js" ;


/**
 * Context for control.
 * @type {webGLcontext} 
 */
var webGL;
/**
 * Last mause pos.
 * @type {vec2}
 */
var lastMousePos;

/**
 * Mouse movement handler.
 * @param  {event} e event.
 */
function handleMouseMove(e) {
    let newMousePos = vec2.create();
    if (document.all) { 
        newMousePos[0] = event.x + document.body.scrollLeft; 
        newMousePos[1] = event.y + document.body.scrollTop; 
    } else {
        newMousePos[0] = e.pageX;
        newMousePos[1] = e.pageY;
    }
    let moveVector = vec2.subtract(vec2.create(),
                                   newMousePos,
                                   lastMousePos);
    webGL.camera_.rotate([moveVector[1],
                          moveVector[0],
                          0.0]);
    lastMousePos = newMousePos;
    webGL.rend();
}
/**
 * Handle press keyboard button.
 * @param  {event} e event.
 */
function handlePressButton(e) {
    switch(e.keyCode){
        case ControlSittings.zoomUp:
            webGL.camera_.move( vec3.fromValues(0.0, 0.0, 1.0) );
            break;
        case ControlSittings.zoomDown:
            webGL.camera_.move( vec3.fromValues(0.0, 0.0, -1.0) );
            break;
        case ControlSittings.left:
            webGL.camera_.move( vec3.fromValues(1.0, 0.0, 0.0) );
            break;
        case ControlSittings.forward:
            webGL.camera_.move( vec3.fromValues(0.0, -1.0, 0.0) );
            break;
        case ControlSittings.right:
            webGL.camera_.move( vec3.fromValues(-1.0, 0.0, 0.0) );
            break;
        case ControlSittings.back:
            webGL.camera_.move( vec3.fromValues(0.0, 1.0, 0.0) );
            break;
    }
    webGL.rend();
}
 

/**
 * Setting control functions to the area.
 * @param  {string} elemId area id.
 * @param  {webGLcontext} context The context that will be controlled.
 */
export function initControlAera(elemId, context) {
    webGL = context;
    var element = document.getElementById(elemId);
    addEventListener("keydown", handlePressButton);
    element.onmousedown = function(e){
        lastMousePos = vec2.fromValues(e.pageX, e.pageY);
        element.onmousemove = handleMouseMove;
    };
    element.onmouseup = function(e){
        element.onmousemove = void(0);
    };

}