/* Copyright (c) 2018, Aleksandrov Maksim */

/**
 * Socket interface.
 * @type {WebSocket}
 */
var socket;


/**
 * Set connetion to server.
 */
function connect() {
    if(socket !== void(0)) {
        socket.close(1000,"");
    };
    socket = new WebSocket('ws://localhost:3000/setConnect');

    socket.onopen = function(event) {
        console.log('WebSocket is connected.'); 
    };
    
    socket.onclose = function(ev) {
        console.log("Close connection.");
        socket = void(0);
    };
}

/**
 * Disconnect from server.
 */
function disconnect() {
    if(socket !== void(0)) {
        socket.close(1000,"");
    }
    socket = void(0);
}

/**
 * Set handler for coming message.
 * @param {function(MessageEvent)} handler 
 */
function setMessageHandler(handler) {
    socket.onmessage = handler;
}

/**
 * Send message to server
 * @param {string} message 
 */
function sendMessage(message) {
    if(socket === void(0)) {
        connect();
    }
    socket.send(message);
}

export {connect, 
        disconnect,
        setMessageHandler,
        sendMessage};