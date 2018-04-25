/* Copyright (c) 2018, Aleksandrov Maksim */

import * as webGL from "./webGl/webGl.js";
import * as controlSetting from "./defaultSettings/control.js";
import * as webGLcontrol from "./webGl/webGLcontrol.js";
import * as netWork from "./net.js";
import * as parser from "./fileParser/parser.js";
import {mat4,vec3,quat} from "./includes/GLMatrix/gl-matrix.js";

var mesh,
    waterMesh;


/**
 * 
 * @param {MessageEvent} result 
 */
function handleComeResult(result) {
    waterMesh = new Object();
    waterMesh.vertex = new Float32Array(result.data);

    console.log(result.data);
}

var gl = new webGL.WebGLcontext('viewport');
var objects = new Array();
gl.setObjectsList(objects);
gl.rend();
webGLcontrol.initControlAera('viewport', gl, objects);

document.getElementById('file-path').onchange = function() {
    document.getElementById('load-massage').style.display = 'block';
    let fileReader = new FileReader();
    let file = document.getElementById('file-path').files[0];
    fileReader.onload = function(e) {
        let meshData = parser.parseFile('ply', e.target.result);
        let shaders = [gl.getShader('vertex-shader'),
        gl.getShader('fragment-shader')];
        let program = gl.getShaderProgram(shaders);
        let object = gl.loadObject(meshData.vertex,
                                   3,
                                   meshData.normals,
                                   3,
                                   meshData.face,
                                   program,
                                   [0.0, 0.6, 0.0, 1.0],
                                   vec3.fromValues(0.0,0.0, 0.0),
                                   quat.create());
        if(object !== void(0)) {
            objects[0] = object;
            mesh = meshData;
            gl.rend();
        }
        document.getElementById('load-massage').style.display = 'none';
    }
    fileReader.onerror = function(e) {
      alert('Error load file');
      document.getElementById('load-massage').style.display = 'none';
    }
    fileReader.readAsArrayBuffer(file);
}


document.getElementById('start-button').onclick = function() {;
    let message = new Array();
    // adding opcode;
    message.push(0);
    message.push(mesh.vertex.length);
    message = message.concat(mesh.vertex);
    message.push(mesh.face.length);
    message = message.concat(mesh.face);
    netWork.sendMessage(new Uint32Array(message), handleComeResult);
}

