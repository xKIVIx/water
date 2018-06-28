/* Copyright (c) 2018, Aleksandrov Maksim */

import * as webGL from "./webGl/webGl.js";
import * as controlSetting from "./defaultSettings/control.js";
import * as webGLcontrol from "./webGl/webGLcontrol.js";
import * as netWork from "./net.js";
import * as parser from "./fileParser/parser.js";
import {mat4,vec3,quat} from "./includes/GLMatrix/gl-matrix.js";

var mesh;


/**
 * 
 * @param {MessageEvent} result 
 */
function handleComeResult(result) {
    var fileReader = new FileReader();
    fileReader.onload = function() {
        objects.length = 1;
        var arrayBuffer = this.result;
        
        let shaders = [gl.getShader('vertex-water-shader'),
                       gl.getShader('fragment-water-shader')];
        let program = gl.getShaderProgram(shaders);
        let readPoint = 0;
        while(readPoint != arrayBuffer.byteLength) {
            let size = new Uint32Array(arrayBuffer, readPoint, 1);
            readPoint += 4;
            let vertex = new Float32Array(arrayBuffer, 
                                          readPoint, 
                                          size[0]);
            readPoint += 4 * size[0];

            size = new Uint32Array(arrayBuffer, readPoint, 1);
            readPoint += 4;
            let faces = new Uint32Array(arrayBuffer, 
                                        readPoint, 
                                        size[0]);
            readPoint += 4 * size[0];
            
            let object = gl.loadObject(vertex,
                                       3,
                                       void(0),
                                       3,
                                       faces,
                                       program,
                                       [0.0, 0.0, 0.6, 1.0],
                                       vec3.fromValues(0.0,0.0, 0.0),
                                       quat.create());
            if(object !== void(0)) {
                objects.push(object);
            }
        }
        gl.rend();
    };
    fileReader.readAsArrayBuffer(result.data);
}

var gl = new webGL.WebGLcontext('viewport');
var objects = new Array();
gl.setObjectsList(objects);

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

/**
 * 
 * @param {Object[]} data 
 * @return {Uint8Array}
 */
function packData(data) {
    let size = data.reduce(function(sumSizes, curr){
        return sumSizes + curr.byteLength;
    }, 0);
    let result = new Uint8Array(size);
    let offset = 0;
    for(let i in data) {
        let buffer = new Uint8Array(data[i].buffer);
        result.set(buffer,
                   offset);
        offset += buffer.byteLength;
    }
    return result;
}

document.getElementById('start-button').onclick = function() {
    if(mesh == void(0)) {
        alert("Модель не загружена. Выберите модель.");
        return;
    }
    let valWater = document.getElementById('rain-val').value / 1000;
    if(valWater <= 0) {
        objects.length = 1;
        return;
    }
    let k = 1 / document.getElementById('scale-model').value;
    valWater *= k;
    let data = [new Uint8Array([0]), // opcode
                //water val
                new Float32Array([valWater]),
                //blocks sizes
                new Uint32Array([mesh.vertex.length * 4,
                                 mesh.face.length * 4]),
                new Float32Array(mesh.vertex),
                new Uint32Array(mesh.face)];
    let message = packData(data);
    // adding opcode;
    netWork.sendMessage(message, handleComeResult);
}

