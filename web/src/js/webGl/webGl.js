/* Copyright (c) 2018, Aleksandrov Maksim */

import {mat4,mat3,vec3,quat} from "./../includes/GLMatrix/gl-matrix.js";
import {webGLcam} from "./webGLcam.js";
import * as glSettings from "./../defaultSettings/webGl.js";

/**
 * @typedef BufferInfo
 * @type {object}
 * @property {Object} buffer
 * @property {number} bufferSize
 * @property {number} elemSize
 */

 /**
  * @typedef ShaderProgram
  * @type {Object}
  * @property {Object} program
  * @property {number} unifLocProjectMat
  * @property {number} unifLocCameraMat
  * @property {number} unifLocObjectMat
  * @property {number} unifLocNormalTrans
  * @property {number} unifLocLightDirect
  * @property {number} unifLocColor
  * @property {number} atrLocVertex
  * @property {number} atrLocNormals
  */

/**
 * @typedef WebGLobject
 * @type {Object}
 * @property {BufferInfo} vertexBuffer vertex data.
 * @property {BufferInfo} [normalsBuffer] normals data.
 * @property {BufferInfo} indicesBuffer indices data.
 * @property {ShaderProgram} shaderProgram shader program.
 * @property {number[]} color
 * @property {vec3} position object position.
 * @property {quat} rotation quaterion of rotation.
 * 
 */


/** 
 * The webgl context class. 
 * When created, it is attached to the canvas element. 
 */
class WebGLcontext {
    
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
        gl.depthFunc(gl.LEQUAL);
        gl.clearDepth(1.0);

        this.mat4Project_ = mat4.create();
        mat4.perspective(this.mat4Project_, 
                         Math.PI/2.0, 
                         canvas.width/canvas.height,
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
     * @return {ShaderProgram} shader program
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

        /**
         * @type {ShaderProgram}
         */
        let result = new Object();
        result.program = shaderProgram;

        result.atrLocVertex = gl.getAttribLocation(shaderProgram,
                                                   "aVec3_vertexPosition");
        result.atrLocNormals = gl.getAttribLocation(shaderProgram,
                                                   "aVec3_normal");

        result.unifLocProjectMat = gl.getUniformLocation (shaderProgram, 
                                                          "uMat4_project");
        result.unifLocCameraMat = gl.getUniformLocation (shaderProgram, 
                                                         "uMat4_cam");
        result.unifLocObjectMat = gl.getUniformLocation (shaderProgram, 
                                                         "uMat4_objectPos");
        result.unifLocNormalTrans = gl.getUniformLocation (shaderProgram, 
                                                           "uMat3_normalTrans");
        result.unifLocLightDirect = gl.getUniformLocation (shaderProgram, 
                                                           "uVec3_reverseLightDirection");
        result.unifLocColor = gl.getUniformLocation (shaderProgram, 
                                                     "uVec4_color");                                                 

        return result;
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
     * Load object into buffers.
     * @param  {number[]} vertex array with vertex
     *                        data (32bit format).
     * @param  {number} vertexSize data size of 
     *                             one vertex.
     * @param  {number[]|void} normals array with normals data.
     * @param  {number[]} indices array of indices.
     * @param  {object} shaderProgram shader program 
     *                                for rend object.
     * @param  {number[]} color object color in RGBA format.
     * @param  {vec3} position object position.
     * @param  {quat} rotation object rotation.
     * @return {WebGLobject} return added object.
     */
    loadObject(vertex,
               vertexSize,
               normals,
               normalSize,
               indices,
               shaderProgram,
               color,
               position,
               rotation) {
        let gl = this.context_;
        /**
         * @type {WebGLobject}
         */
        let result = {
            position: position,
            rotation: rotation,
            shaderProgram: shaderProgram,
            color: color
        };

        // load vertex
        var vertexBuffer = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER,
                      vertexBuffer);
        gl.bufferData(gl.ARRAY_BUFFER,
                      new Float32Array(vertex),
                      gl.STATIC_DRAW);
        result.vertexBuffer = {
            elemSize: vertexSize,
            bufferSize: vertex.length,
            buffer: vertexBuffer
        }

        // load normals
        if(normals !== void(0)) {
            let normalsBuffer = gl.createBuffer();
            gl.bindBuffer(gl.ARRAY_BUFFER,
                          normalsBuffer);
            gl.bufferData(gl.ARRAY_BUFFER,
                          new Float32Array(normals),
                          gl.STATIC_DRAW);
            result.normalsBuffer = {
                elemSize: normalSize,
                bufferSize: normals.length,
                buffer: normalsBuffer
            };
        }
        
        // load indices
        let indicesBuffer = gl.createBuffer();
        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,
                      indicesBuffer);
        gl.bufferData(gl.ELEMENT_ARRAY_BUFFER,
                      new Uint16Array(indices),
                      gl.STATIC_DRAW);
        result.indicesBuffer = {
            elemSize: 1,
            bufferSize: indices.length,
            buffer: indicesBuffer
        };
        return result;
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
            let shaderProgram = currentObject.shaderProgram;
            gl.useProgram(shaderProgram.program);
            gl.uniformMatrix4fv(shaderProgram.unifLocProjectMat,
                                false, 
                                mat4Project);
            gl.uniformMatrix4fv(shaderProgram.unifLocCameraMat,
                                false, 
                                camMatrix);
            gl.uniformMatrix4fv(shaderProgram.unifLocObjectMat,
                                false, 
                                mat4.fromRotationTranslation (mat4.create(),
                                                              currentObject.rotation,
                                                              currentObject.position));
            let normalMatrix = mat3.normalFromMat4(mat3.create(),
                                                   camMatrix);
            gl.uniformMatrix3fv(shaderProgram.unifLocNormalTrans, 
                                false, 
                                normalMatrix);
            gl.uniform3fv(shaderProgram.unifLocLightDirect,
                          glSettings.lightDir);
            gl.uniform4fv(shaderProgram.unifLocColor,
                          currentObject.color);
                               
            gl.bindBuffer(gl.ARRAY_BUFFER, 
                          currentObject.vertexBuffer.buffer); 
            gl.enableVertexAttribArray(shaderProgram.atrLocVertex);  
            gl.vertexAttribPointer(shaderProgram.atrLocVertex, 
                                   currentObject.vertexBuffer.elemSize, 
                                   gl.FLOAT,
                                   false, 
                                   0, 
                                   0);
            
            if(currentObject.normalsBuffer !== void(0)) {
                gl.bindBuffer(gl.ARRAY_BUFFER, 
                          currentObject.normalsBuffer.buffer); 
                gl.enableVertexAttribArray(shaderProgram.atrLocNormals);  
                gl.vertexAttribPointer(shaderProgram.atrLocNormals, 
                                       currentObject.normalsBuffer.elemSize, 
                                       gl.FLOAT,
                                       false, 
                                       0, 
                                       0);
            }

            gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,
                          currentObject.indicesBuffer.buffer);
            gl.drawElements(gl.TRIANGLES,
                            currentObject.indicesBuffer.bufferSize,
                            gl.UNSIGNED_SHORT,
                            0);
        } );
    }

    /**
     * Setting an array of rendered objects.
     * @param {WebGLobject[]} objects pointer to an array of objects for rendering
     */
    setObjectsList(objects) {
        this.objects_ = objects;
    }

}

export {WebGLcontext, webGLcam};