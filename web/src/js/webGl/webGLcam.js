/* Copyright (c) 2018, Aleksandrov Maksim */

import {mat4, vec3, quat} from "./../includes/GLMatrix/gl-matrix";
import * as glControlSetting from "./../defaultSettings/control.js"

/**
 * Class to create and manage the camera.
 */
class webGLcam {
    /**
     * @constructor
     * @param  {vec3} position vec3 camera position.
     * @param  {quat} rotate quat camera rotate.
     */
    constructor(position, rotate) {
        this.matrix_ = mat4.create();
        this.mat4Position_ = position;
        this.quatRotate_ = rotate;
    }

    /**
     * Camera displacement in a certain direction.
     * @param  {vec3} moveVector The camera position shift vector.
     */
    move(moveVector) {
        vec3.scaleAndAdd(this.mat4Position_,
                         this.mat4Position_,
                         moveVector,
                         glControlSetting.transSensitivity);
        
    }

    /**
     * Rotate the camera along the axis of the vector by a value depending
     * on the length of the vector and the set sensitivity.
     * @param  {vec3} rotateVector Vector of rotation.
     */
    rotate(rotateVector) {
        let angle = vec3.length(rotateVector)*
                    glControlSetting.rotateSensitivity;
        
        let rotateQuat = quat.setAxisAngle(quat.create(),
                                           rotateVector,
                                           angle);
        quat.multiply(this.quatRotate_,
                      this.quatRotate_,
                      rotateQuat);
        quat.normalize(this.quatRotate_,
                       this.quatRotate_);
    }
    /**
     * Get camera matrix.
     * @return {mat4} camera matrix.
     */
    getMatrix() {
        return mat4.fromRotationTranslation(mat4.create(),
                                            this.quatRotate_,
                                            this.mat4Position_)
    }
}
export {webGLcam};