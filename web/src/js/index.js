/* Copyright (c) 2018, Aleksandrov Maksim */

import * as webGL from "./webGl.js";
import * as controlSetting from "./defaultSettings/control.js";
import * as webGLcontrol from "./webGLcontrol.js";
import * as NetWork from "./net.js";

var currentWebGL = webGL.testWebGl();

var socket = new WebSocket('ws://localhost:3000/setConnect');

socket.onmessage = function(mes) {
  alert("Answer is come");
}

// Выводим сообщение при открытии WebSocket-соединения.
socket.onopen = function(event) {
  console.log('WebSocket is connected.'); 
};

document.getElementById('test-net-button').onclick = function() {
  socket.send("test");
};

webGLcontrol.initControlAera('viewport', currentWebGL);