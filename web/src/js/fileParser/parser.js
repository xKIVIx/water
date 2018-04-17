/* Copyright (c) 2018, Aleksandrov Maksim */

import * as ply from "./parserPly.js"

const SupportedFormat = {
    ply: ply.parse
}

/**
 * Parsing model file.
 * @param {string} format File format.
 * @param {ArrayBuffer} file  File data
 * @return {{vertex:number[], face:number[]}|void} Model data
 *  or void if fail parse.
 */
function parseFile(format, file) {
    let formatHandler = SupportedFormat[format];
    if(formatHandler === void(0)) {
        alert('Unsupport format.');
        return;
    }
    let returnStruct = {
        vertex: ['x', 'y', 'z'],
        face: ['vertex_indices']
    };
    return formatHandler(file, returnStruct);
}

export {parseFile};