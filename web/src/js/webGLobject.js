/* Copyright (c) 2018, Aleksandrov Maksim */

import {mat4, vec3, quat} from "./includes/GLMatrix/gl-matrix";

/** 
 * A class for storing objects to display. 
 * Objects form a doubly connected list.
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
     * Add an object to the list.
     * @param  {webGLobject} object object to add.
     */
    addObject(object) {
        object.lastObject_ = this;
        object.nextObject_ = this.nextObject_;
        this.nextObject_ = object;
    }

    /**
     * Delete object from list.
     * @return if list empty then void(0), 
     *         else near object.
     */
    deleteObject() {
        var out = void(0);
        if (this.lastObject_ !== void(0)) {
            this.lastObject_.nextObject_ = this.nextObject_;
            out = this.lastObject_;
        }
        if (this.nextObject_ !== void(0)){
            this.nextObject_.lastObject_ = this.lastObject_;
            out = this.nextObject_;
        }
        return out;
    }

    /**
     * Get the first object in the list.
     * @return {webGLobject} first object in the list.
     */
    getFirstObject() {
        var out = this;
        while (out.lastObject_ !== void(0)) {
            out = out.lastObject_;
        }
        return out;
    }

    /**
     * Get the last object in the list.
     * @return {webGLobject} last object in the list.
     */
    getLastObject() {
        var out = this;
        while (out.nextObject_ !== void(0)) {
            out = out.nextObject_;
        }
        return out;
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