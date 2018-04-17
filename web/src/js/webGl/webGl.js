/* Copyright (c) 2018, Aleksandrov Maksim */

import {mat4,vec3,quat} from "./../includes/GLMatrix/gl-matrix.js";
import {webGLcam} from "./webGLcam.js";
import {webGLobject} from "./webGLobject.js";
import * as glSettings from "./../defaultSettings/webGl.js";

/** 
 * The webgl context class. 
 * When created, it is attached to the canvas element. 
 */
class webGLcontext {
    
    /**
     * @constructor
     * @param  {string} viewport canvas id
     */
    constructor(viewport) {
        let canvas = document.getElementById(viewport);
        let gl = canvas.getContext('webgl');

        this.context_ = gl;
        gl.viewport(0,
                    0,
                    canvas.width,
                    canvas.height);

        gl.getExtension('OES_element_index_uint');

        gl.clearColor(glSettings.clearColor[0],
                      glSettings.clearColor[1],
                      glSettings.clearColor[2],
                      glSettings.clearColor[3]);

        gl.enable(gl.DEPTH_TEST);
        gl.depthFunc(gl.LESS);
        gl.clearDepth(1.0);

        this.mat4Project_ = mat4.create();
        mat4.perspective(this.mat4Project_, 
                         Math.PI/1.5, 
                         1, 
                         glSettings.minDist, 
                         glSettings.maxDist);

        let camRotate = quat.fromEuler(quat.create(),
                                       glSettings.camRotate[0],
                                       glSettings.camRotate[1],
                                       glSettings.camRotate[2]);

        this.camera_ = new webGLcam(glSettings.camPosition,
                                    camRotate);
    }

    /**
     * Create shader program from shaders
     * @param  {object[]} shaders shaders for the created program. 
     *                            (should contain one shader of each type).
     * @return shader program
     */
    getShaderProgram(shaders) {
        let gl = this.context_;
        let shaderProgram = gl.createProgram();

        for (var i in shaders) {
            gl.attachShader(shaderProgram,
                            shaders[i]);
        };

        gl.linkProgram(shaderProgram);

        if (!gl.getProgramParameter(shaderProgram,
                                    gl.LINK_STATUS)) {
            alert("Can`t init shader program");
            return;
        };

        shaderProgram.vertexPositionAttribute_ = 
                gl.getAttribLocation(shaderProgram,
                "aVec3_vertexPosition");

        gl.enableVertexAttribArray(shaderProgram.vertexPositionAttribute_);

        shaderProgram.matrix_ = {
            mat4Project_: gl.getUniformLocation (shaderProgram, 
                                                 "uMat4_project"),
            mat4Cam_: gl.getUniformLocation (shaderProgram, 
                                             "uMat4_cam"),
            mat4ObjectPos_: gl.getUniformLocation (shaderProgram, 
                                                   "uMat4_objectPos")
        };
        return shaderProgram;
    }
    
    /**
     * This function for compiled shader from
     * source in html block
     * @param  {string} blockId id of html block where
     *                          store source of sheder
     * @return Shader used for create shader programm.
     *         If an error occurs during compilation, it returns undefined.
     */
    getShader(blockId) {
        let gl = this.context_;
        let type;
        let store = document.getElementById(blockId);
        let storeType = store.getAttribute('type');

        if (storeType == 'x-shader/x-vertex') {
            type = gl.VERTEX_SHADER;
        } else if (storeType == 'x-shader/x-fragment') {
            type = gl.FRAGMENT_SHADER;
        } else {
            alert("Unknown shader type :" + storeType);
            return void(0);
        }

        let source = store.innerHTML;
        let shader = gl.createShader(type);
        gl.shaderSource(shader, source);
        gl.compileShader(shader);

        if (!gl.getShaderParameter(shader,
                gl.COMPILE_STATUS)) {
            alert("Error compile shader: " +
                gl.getShaderInfoLog(shader));
            gl.deleteShader(shader);
            return void(0);
        }
        return shader;
    }

    /**
     * load object into buffer
     * @param  {number[]} vertex array with vertex
     *                        data (32bit format)
     * @param  {number} vertexSize data size of 
     *                             one vertex
     * @param  {number[]} indices array of indices
     * @param  {object} shaderProgram shader program 
     *                                for rend object
     * @param  {vec3} position object position
     * @param  {quat} rotation object rotation
     * @return {webGLobject} return added object
     */
    loadObject(vertex,
               vertexSize,
               indices,
               shaderProgram,
               position,
               rotation) {
        let gl = this.context_;

        let vertexBuffer = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER,
                      vertexBuffer);
        gl.bufferData(gl.ARRAY_BUFFER,
                      new Float32Array(vertex),
                      gl.STATIC_DRAW);
        vertexBuffer.itemSize_ = vertexSize;
        vertexBuffer.countItems_ = vertex.length;

        let indicesBuffer = gl.createBuffer();
        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,
                      indicesBuffer);
        gl.bufferData(gl.ELEMENT_ARRAY_BUFFER,
                      new Uint16Array(indices),
                      gl.STATIC_DRAW);
        indicesBuffer.itemSize_ = 1;
        indicesBuffer.countItems_ = indices.length;
        
        let newObject = new webGLobject(vertexBuffer,
                                        indicesBuffer,
                                        shaderProgram,
                                        position,
                                        rotation);

        return new webGLobject(vertexBuffer,
                               indicesBuffer,
                               shaderProgram,
                               position,
                               rotation);
    }

    /**
     * Rend objects.
     */
    rend() {
        let gl = this.context_;
        let camMatrix = this.camera_.getMatrix();
        let mat4Project = this.mat4Project_;
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
        this.objects_.forEach( function (currentObject) {
            let shaderProgram = currentObject.shaderProgram_;
            gl.useProgram(shaderProgram);
            gl.uniformMatrix4fv(shaderProgram.matrix_.mat4Project_,
                                false, 
                                mat4Project);
            gl.uniformMatrix4fv(shaderProgram.matrix_.mat4Cam_,
                                false, 
                                camMatrix);
            gl.uniformMatrix4fv(shaderProgram.matrix_.mat4ObjectPos_,
                                false, 
                                currentObject.getMatrix());

            gl.bindBuffer(gl.ARRAY_BUFFER, 
                          currentObject.vertexBuffer_);
            gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,
                          currentObject.indicesBuffer_);
                
            gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute_, 
                                   currentObject.vertexBuffer_.itemSize_, 
                                   gl.FLOAT, 
                                   false, 
                                   0, 
                                   0);

            gl.drawElements(gl.TRIANGLES,
                            currentObject.indicesBuffer_.countItems_,
                            gl.UNSIGNED_SHORT,
                            0);
        } );
    }

    /**
     * Setting an array of rendered objects.
     * @param {webGLobject[]} objects pointer to an array of objects for rendering
     */
    setObjectsList(objects) {
        this.objects_ = objects;
    }

}

/**
 * Get the object for testing.
 * @param {webGLcontext} gl context.
 * @return {webGLobject} test object.
 */
function testWebGl(gl) {
    var vertices =[
        -0.5, -0.5, 0.5,
        -0.5, 0.5, 0.5,
         0.5, 0.5, 0.5,
         0.5, -0.5, 0.5,
        -0.5, -0.5, -0.5,
        -0.5, 0.5, -0.5,
         0.5, 0.5, -0.5,
         0.5, -0.5, -0.5
         ];
          
    var indices = [
        0, 1, 2, 
        2, 3, 0,
        0, 4, 7, 
        7, 3, 0,
        0, 1, 5,
        5, 4, 0, 
        2, 3, 7, 
        7, 6, 2,
        2, 1, 6,
        6, 5, 1,
        4, 5, 6,
        6, 7, 4,
    ]; 
    var shaders = [gl.getShader('vertex-shader'),
                   gl.getShader('fragment-shader')];
    var program = gl.getShaderProgram(shaders);
    let object = gl.loadObject(vertices,
                               3,
                               indices,
                               program,
                               vec3.fromValues(0.0,0.0,-3.0),
                               quat.create());
    return object;
}

export {webGLcontext,testWebGl,webGLcam};