/* Copyright (c) 2018, Aleksandrov Maksim */

const regexElement = /element +(\w+) +([0-9]+)/g;
const regexHeader = /ply([\w\W]+)end_header/g;
const regexProperty = /property +(\w+) +(\w+) *(\w*) *(\w*)/g;
const regexDataFormat = /format (\w+)/g;

/**
 * The property of the elements.
 * @typedef elemProperty
 * @type {Object}
 * @property {string} valType The type of the value.
 * @property {string} [listSizeType] The type of the list size value. 
 *                                 Defined if the item type is a list.
 * @property {string} [listValType] The value type of the list. 
 *                                Defined if the item type is a list.
 * @property {number} data Service value.
 */

/**
 * Contains block element parameters.
 * @typedef element
 * @property {elemProperty[]} struct Structure of the content of elements.
 *                                   Associative array. The key is the name of the parameter.
 * @property {number} count The number of elements of this type.
 * @property {string} nameElem The name of the element.
 */

/**
 * Parsing struct of element.
 * @param {string[]} elements An array of strings of the following type: 
 *                            <element name 1>, <number of elements 1>, <element content 1>, 
 *                            <element name 2>, <element count 2>, <element 2 content>, .....
 * @return {element[]} Processed data.
 */
function parseElemsStructs(elements) {
    /**
     * @type {element[]}
     */
    let result = new Array();
    for(let i = 0; i < elements.length; i += 3) {
        /**
         * @type {element}
         */
        let newElem = new Object();
        newElem.nameElem = elements[i];
        newElem.count = Number.parseInt(elements[i + 1]);
        newElem.struct = new Object();
        let propertys = elements[i+2].split(/\r\n|\n/g);

        propertys.forEach( function (val) {
            if(val.length !== 0) {
                let words = val.match(/\w+/g);
                if(words.length > 1) {
                    let name = words.pop();
                    newElem['struct'][name] = new Object();
                    newElem.struct[name].valType = words[1];
                    if(newElem.struct[name].valType === 'list') {
                        newElem.struct[name].listSizeType = words[2];
                        newElem.struct[name].listValType = words[3];
                    };
                };
            };
        } );
        result.push(newElem);
    };
    return result;
}

/**
 * Parsing ACSII format data.
 * @param {element[]} elements An array of contained elements. Elesenty 
 *                             should be in the order presented in the file.
 * @param {string[]} returnStruct Associative array. The key is the name of the element.
 *                                An object containing the output 
 *                                structure describing the structure. Structure: 
 *                                <element name>: <parameter name 1> <parameter name 2>...
 * @param {number} offset The number of bytes to back out in the file.
 * @param {ArrayBuffer} file The file data.
 * @return {number[]} Associative array. The key is the name of the element.
 *                    Processed data. According to the returnStruct.
 */
function parseDataACSII (elements, returnStruct, offset, file) {
    /**
     * Storing result. Associative array. The key is the name of the element.
     * @type {number[]}
     */
    let result = new Object();
    for(let elemName in returnStruct) {
        result[elemName] = new Array();
    }

    /**
     * Decoded text.
     * @type {string}
     */
    let text = new TextDecoder('utf-8').decode(new Uint8Array(file,
                                                              offset));
    let splitedText = text.match(/[0-9. -]+/g);
    let lastLine = 0;
    elements.forEach( function(val) {
        let line = lastLine;
        lastLine = lastLine + val.count;

        result[val.nameElem] = new Array();
        for(; line < lastLine; line++) {
            let splitedString = splitedText[line].match(/[0-9.-]+/g);
            let currentWord = 0;

            // load data in struct
            for(let pName in val.struct) {
                let valStruct = val.struct[pName];

                switch(valStruct.valType) {
                    case 'float':
                        valStruct.data = parseFloat(splitedString[currentWord]);
                        break;
                    case 'list':
                        let listSize = parseInt(splitedString[currentWord]);
                        currentWord++;
                        valStruct.data = new Array();
                        for(let i = 0; i < listSize; i++) {
                            if(valStruct.listValType === 'float') {
                                valStruct.data.push( parseFloat(splitedString[currentWord]) );
                            } else {
                                valStruct.data.push( parseInt(splitedString[currentWord]) );
                            };
                            currentWord++;
                        }
                        currentWord--;
                        break;
                    default:
                        valStruct.data = parseInt(splitedString[currentWord]);
                        break;
                }
                currentWord++;
            } // load data in struct
            
            // load data in result struct
            for(let elemName in returnStruct) {
                for(let i in returnStruct[elemName]) {
                    let propName = returnStruct[elemName][i];
                    if(propName in val.struct) {
                        let prop = val.struct[propName];
                        result[elemName] = result[elemName].concat(prop.data);
                    }
                }
            }
        };
    });
    return result;
}

/**
 * Parsing file.
 * @param {ArrayBuffer} file File data.
 * @param {Object} returnStruct An object containing the output 
 *                             structure describing the structure. Structure: 
 *                             <element name>: <parameter name 1> <parameter name 2>...
 * @return {{vertex:number[], face:number[]}|void} Model data.
 */
function parse(file, returnStruct) {
    let text = new TextDecoder('utf-8').decode(new Uint8Array(file));
    
    /**
     * Header.
     * @type {string}
     */
    let header = text.match(regexHeader);
    if(header === null) {
        alert('Don`t find file head');
        return;
    }
    header = header[0];

    let dataFormat = header.match(regexDataFormat);
    if(dataFormat === null) {
        alert('Don`t find data format');
        return;
    }

    let elements = header.split(regexElement);
    elements.shift();
    let parsedElems = parseElemsStructs(elements);

    if(dataFormat[0] === 'format ascii') {
        return( parseDataACSII(parsedElems,
                               returnStruct,
                               header.length,
                               file) );
    } else {
        alert('Unknown data format');
        return;
    }
}

export {parse};