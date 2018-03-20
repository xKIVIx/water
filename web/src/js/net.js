/* Copyright (c) 2018, Aleksandrov Maksim */

class net {
    constructor(adress) {
        this.socket_ = new WebSocket(adress);
    };
};

export {net};