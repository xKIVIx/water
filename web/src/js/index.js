/* Copyright (c) 2018, Aleksandrov Maksim */

import * as webGL from "./webGl.js";
import * as controlSetting from "./defaultSettings/control.js";
import * as webGLcontrol from "./webGLcontrol.js";

var currentWebGL = webGL.testWebGl();

webGLcontrol.initControlAera('viewport', currentWebGL);