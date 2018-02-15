import {mat4} from "./includes/GLMatrix/gl-matrix.js";
class webGlContext {
    /**
     *  @brief constructor of webGl context
     *
     *  @param [in] viewport - canvas id
     */
    constructor(viewport) {
        var canvas = document.getElementById(viewport);
        this.context_ = canvas.getContext('webgl');
        this.context_.viewport(0,
            0,
            canvas.width,
            canvas.height);
        this.context_.getExtension('OES_element_index_uint');
        var tmp = mat4.create();
        alert (tmp);
        this.context_.clearColor(0.5, 1, 0, 200);
    }
    getShaderProgram(shaders) {
        var shaderProgram = this.context_.createProgram();

        for (var i in shaders) {
            this.context_.attachShader(shaderProgram,
                shaders[i]);
        }

        this.context_.linkProgram(shaderProgram);

        if (!this.context_.getProgramParameter(shaderProgram,
                this.context_.LINK_STATUS)) {
            alert("Can`t init shader program");
        }
        shaderProgram.vertexPositionAttribute_ = 
            this.context_.getAttribLocation(shaderProgram, 
                "aVertexPosition");
        this.context_.enableVertexAttribArray(
            shaderProgram.vertexPositionAttribute_);
        return shaderProgram;
    }
    
    /**
     *  @brief This function for compiled shader from
     *         source in html block
     *
     *  @param [in] blockId - id of html block? where
     *                        store source of sheder
     *  @return return compiled shader
     */
    getShader(blockId) {
        var type;
        const store = document.getElementById(blockId);
        var storeType = store.getAttribute('type');
        // define shader type
        if (storeType == 'x-shader/x-vertex') {
            type = this.context_.VERTEX_SHADER;
        } else if (storeType == 'x-shader/x-fragment') {
            type = this.context_.FRAGMENT_SHADER;
        } else {
            alert("Unknown shader type :" + storeType);
            return null;
        }
        var source = store.innerHTML;
        var shader = this.context_.createShader(type);
        this.context_.shaderSource(shader, source);
        this.context_.compileShader(shader);

        // chek compile
        if (!this.context_.getShaderParameter(shader,
                this.context_.COMPILE_STATUS)) {
            alert("Error compile shader: " +
                this.context_.getShaderInfoLog(shader));
            this.context_.deleteShader(shader);
            return null;
        }
        return shader;
    }

    /**
     *  @brief load mesh into buffer
     *
     *  @param [in] vertex - array with vertex
     *                       data (32bit format)
     *  @param [in] indices - indices array (32bit format)
     *  @return structure { vertexBuffer, indexBuffer }
     */
    loadMesh(vertex,
        vertexSize,
        indices) {
        // init vertex data
        var vertexBuffer = this.context_.createBuffer();
        this.context_.bindBuffer(this.context_.ARRAY_BUFFER,
            vertexBuffer);
        this.context_.bufferData(this.context_.ARRAY_BUFFER,
            new Float32Array(vertex),
            this.context_.STATIC_DRAW);
        vertexBuffer.itemSize_ = vertexSize;
        vertexBuffer.countItems_ = vertex.length;

        // init indices buffer
        var indicesBuffer = this.context_.createBuffer();
        this.context_.bindBuffer(
            this.context_.ELEMENT_ARRAY_BUFFER,
            indicesBuffer);
        this.context_.bufferData(
            this.context_.ELEMENT_ARRAY_BUFFER,
            new Uint16Array(indices),
            this.context_.STATIC_DRAW);
        indicesBuffer.itemSize_ = 1;
        indicesBuffer.countItems_ = indices.length;
        
        return {
            vertexBuffer_: vertexBuffer,
            indicesBuffer_: indicesBuffer
        };
    }

    /**
     *  @brief rend objects
     *
     *  @param [in] objects - an array of objects having
     *                        a structure {vertexBuffer_,
     *                                     indexBuffer_,
     *                                     shaderProgram_}
     */
    rend(objects) {
        this.context_.clear(this.context_.COLOR_BUFFER_BIT);
        for (var i in objects) {
            // init program
            this.context_.useProgram(objects[i].shaderProgram_);
            // bind data
            this.context_.bindBuffer(
                this.context_.ARRAY_BUFFER, 
                objects[i].vertexBuffer_);
            this.context_.bindBuffer(
                this.context_.ELEMENT_ARRAY_BUFFER,
                objects[i].indicesBuffer_);
                
            // set vertex attribute
            this.context_.vertexAttribPointer(
                objects[i].shaderProgram_.vertexPositionAttribute_, 
                objects[i].vertexBuffer_.itemSize_, 
                this.context_.FLOAT, 
                false, 
                0, 
                0);
            // rend
            this.context_.drawElements(this.context_.TRIANGLES,
                objects[i].indicesBuffer_.countItems_,
                this.context_.UNSIGNED_SHORT,
                0);
        }
    }

}
document.getElementById('test-button').onclick = function () {
    var gl = new webGlContext('viewport');
    var triangleVertices = [
        0.0, 0.5, 0.0,
        -0.5, -0.5, 0.0,
        0.5, -0.5, 0.0,
        0.5, 0.5, 0.0
    ];
    var indices = [0,1,3]; 
    var shaders = [
        gl.getShader('vertex-shader'),
        gl.getShader('fragment-shader')];
    var program = gl.getShaderProgram(shaders);
    
    var objects = [gl.loadMesh(triangleVertices, 3,indices)];
    objects[0].shaderProgram_ = gl.getShaderProgram(shaders);
    gl.rend(objects);
};