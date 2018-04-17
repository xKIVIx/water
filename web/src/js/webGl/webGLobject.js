/* Copyright (c) 2018, Aleksandrov Maksim */

import {mat4, vec3, quat} from "./../includes/GLMatrix/gl-matrix";

/** 
 * A class for storing object.
 */
class webGLobject {
    /**
     * @constructor
     * @param  {object} vertexBuffer object with vertex data.
     * @param  {object} indicesBuffer object with index data.
     * @param  {object} shaderProgram shader program.
     * @param  {vec3} position object position.
     * @param  {quat} rotation quaterion of rotation.
     */
    constructor(vertexBuffer,
                indicesBuffer,
                shaderProgram,
                position,
                rotation) {
        this.vertexBuffer_ = vertexBuffer;
        this.indicesBuffer_ = indicesBuffer;
        this.shaderProgram_ = shaderProgram;
        this.vec3Position_ = position;
        this.quatRotation_ = rotation;
    }

    /**
     * Get the object matrix.
     * @return {mat4} matrix
     */
    getMatrix() {
        let out = mat4.create();
        mat4.fromRotationTranslation (out,
                                      this.quatRotation_,
                                      this.vec3Position_);
        return out;
    }
}

export {webGLobject};