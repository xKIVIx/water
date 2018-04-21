/* Copyright (c) 2018, Aleksandrov Maksim */

/**
 * Socket interface.
 * @type {WebSocket}
 */
var socket;

/**
 * Send message to server
 * @param {Int8Array} message 
 */
function sendMessage(message, messageHandler) {
    if(socket !== void(0)) {
        socket.close(1000,"");
    };
    socket = new WebSocket('ws://localhost:3000/setConnect');

    socket.onmessage = messageHandler;
    socket.onopen = function(event) {
        console.log('WebSocket is connected.');
        socket.send(message);
    };
    
    socket.onclose = function(ev) {
        console.log("Close connection.");
        socket = void(0);
    };
    
}

export {sendMessage};