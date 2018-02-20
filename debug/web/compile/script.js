!function(t){var n={};function e(r){if(n[r])return n[r].exports;var a=n[r]={i:r,l:!1,exports:{}};return t[r].call(a.exports,a,a.exports,e),a.l=!0,a.exports}e.m=t,e.c=n,e.d=function(t,n,r){e.o(t,n)||Object.defineProperty(t,n,{configurable:!1,enumerable:!0,get:r})},e.n=function(t){var n=t&&t.__esModule?function(){return t.default}:function(){return t};return e.d(n,"a",n),n},e.o=function(t,n){return Object.prototype.hasOwnProperty.call(t,n)},e.p="",e(e.s=7)}([function(t,n,e){"use strict";e.d(n,"a",function(){return a});const r=1e-6;n.b=r;let a="undefined"!=typeof Float32Array?Float32Array:Array;const o=Math.random;n.c=o;Math.PI},function(t,n,e){"use strict";e(0),e(8),e(9),e(4);var r=e(10),a=e(11),o=e(12),i=e(5);e(6);e.d(n,"a",function(){return r}),e.d(n,"b",function(){return a}),e.d(n,"c",function(){return o}),e.d(n,"d",function(){return i})},function(t,n,e){"use strict";e.d(n,"c",function(){return r}),e.d(n,"b",function(){return a}),e.d(n,"a",function(){return o});var r=.5,a=.005,o={zoomUp:33,zoomDown:34,left:37,forward:38,right:39,back:40}},function(t,n,e){"use strict";e.d(n,"a",function(){return c});var r=e(1),a=e(13),o=e(14),i=e(15);class u{constructor(t){var n=document.getElementById(t);this.context_=n.getContext("webgl"),this.context_.viewport(0,0,n.width,n.height),this.context_.getExtension("OES_element_index_uint"),this.context_.clearColor(i.c[0],i.c[1],i.c[2],i.c[3]),this.context_.enable(this.context_.DEPTH_TEST),this.context_.depthFunc(this.context_.LESS),this.context_.clearDepth(1),this.mat4Project_=r.a.create(),r.a.perspective(this.mat4Project_,Math.PI/1.5,1,i.e,i.d);let e=r.b.fromEuler(r.b.create(),i.b[0],i.b[1],i.b[2]);this.camera_=new a.a(i.a,e)}getShaderProgram(t){var n=this.context_.createProgram();for(var e in t)this.context_.attachShader(n,t[e]);return this.context_.linkProgram(n),this.context_.getProgramParameter(n,this.context_.LINK_STATUS)||alert("Can`t init shader program"),n.vertexPositionAttribute_=this.context_.getAttribLocation(n,"aVec3_vertexPosition"),this.context_.enableVertexAttribArray(n.vertexPositionAttribute_),n.matrix_={mat4Project_:this.context_.getUniformLocation(n,"uMat4_project"),mat4Cam_:this.context_.getUniformLocation(n,"uMat4_cam"),mat4ObjectPos_:this.context_.getUniformLocation(n,"uMat4_objectPos")},n}getShader(t){var n;const e=document.getElementById(t);var r=e.getAttribute("type");if("x-shader/x-vertex"==r)n=this.context_.VERTEX_SHADER;else{if("x-shader/x-fragment"!=r)return alert("Unknown shader type :"+r),null;n=this.context_.FRAGMENT_SHADER}var a=e.innerHTML,o=this.context_.createShader(n);return this.context_.shaderSource(o,a),this.context_.compileShader(o),this.context_.getShaderParameter(o,this.context_.COMPILE_STATUS)?o:(alert("Error compile shader: "+this.context_.getShaderInfoLog(o)),this.context_.deleteShader(o),null)}loadObject(t,n,e,r,a,i){var u=this.context_.createBuffer();this.context_.bindBuffer(this.context_.ARRAY_BUFFER,u),this.context_.bufferData(this.context_.ARRAY_BUFFER,new Float32Array(t),this.context_.STATIC_DRAW),u.itemSize_=n,u.countItems_=t.length;var c=this.context_.createBuffer();this.context_.bindBuffer(this.context_.ELEMENT_ARRAY_BUFFER,c),this.context_.bufferData(this.context_.ELEMENT_ARRAY_BUFFER,new Uint16Array(e),this.context_.STATIC_DRAW),c.itemSize_=1,c.countItems_=e.length;let s=new o.a(u,c,r,a,i);return void 0===this.objectsList_?this.objectsList_=s:this.objectsList_.addObject(s),s}rend(){let t=this.objectsList_.getFirstObject(),n=this.camera_.getMatrix();for(this.context_.clear(this.context_.COLOR_BUFFER_BIT|this.context_.DEPTH_BUFFER_BIT);void 0!==t;){let e=t.shaderProgram_;this.context_.useProgram(e),this.context_.uniformMatrix4fv(e.matrix_.mat4Project_,!1,this.mat4Project_),this.context_.uniformMatrix4fv(e.matrix_.mat4Cam_,!1,n),this.context_.uniformMatrix4fv(e.matrix_.mat4ObjectPos_,!1,t.getMatrix()),this.context_.bindBuffer(this.context_.ARRAY_BUFFER,t.vertexBuffer_),this.context_.bindBuffer(this.context_.ELEMENT_ARRAY_BUFFER,t.indicesBuffer_),this.context_.vertexAttribPointer(e.vertexPositionAttribute_,t.vertexBuffer_.itemSize_,this.context_.FLOAT,!1,0,0),this.context_.drawElements(this.context_.TRIANGLES,t.indicesBuffer_.countItems_,this.context_.UNSIGNED_SHORT,0),t=t.nextObject_}}}function c(){var t=new u("viewport"),n=[t.getShader("vertex-shader"),t.getShader("fragment-shader")],e=t.getShaderProgram(n);t.loadObject([-.5,-.5,.5,-.5,.5,.5,.5,.5,.5,.5,-.5,.5,-.5,-.5,-.5,-.5,.5,-.5,.5,.5,-.5,.5,-.5,-.5],3,[0,1,2,2,3,0,0,4,7,7,3,0,0,1,5,5,4,0,2,3,7,7,6,2,2,1,6,6,5,1,4,5,6,6,7,4],e,r.d.fromValues(0,0,-3),r.b.create());return t.rend(),t}},function(t,n,e){"use strict";n.a=function(){let t=new r.a(9);return t[0]=1,t[1]=0,t[2]=0,t[3]=0,t[4]=1,t[5]=0,t[6]=0,t[7]=0,t[8]=1,t};var r=e(0)},function(t,n,e){"use strict";Object.defineProperty(n,"__esModule",{value:!0}),n.create=a,n.clone=function(t){var n=new r.a(3);return n[0]=t[0],n[1]=t[1],n[2]=t[2],n},n.length=o,n.fromValues=i,n.copy=function(t,n){return t[0]=n[0],t[1]=n[1],t[2]=n[2],t},n.set=function(t,n,e,r){return t[0]=n,t[1]=e,t[2]=r,t},n.add=function(t,n,e){return t[0]=n[0]+e[0],t[1]=n[1]+e[1],t[2]=n[2]+e[2],t},n.subtract=u,n.multiply=c,n.divide=s,n.ceil=function(t,n){return t[0]=Math.ceil(n[0]),t[1]=Math.ceil(n[1]),t[2]=Math.ceil(n[2]),t},n.floor=function(t,n){return t[0]=Math.floor(n[0]),t[1]=Math.floor(n[1]),t[2]=Math.floor(n[2]),t},n.min=function(t,n,e){return t[0]=Math.min(n[0],e[0]),t[1]=Math.min(n[1],e[1]),t[2]=Math.min(n[2],e[2]),t},n.max=function(t,n,e){return t[0]=Math.max(n[0],e[0]),t[1]=Math.max(n[1],e[1]),t[2]=Math.max(n[2],e[2]),t},n.round=function(t,n){return t[0]=Math.round(n[0]),t[1]=Math.round(n[1]),t[2]=Math.round(n[2]),t},n.scale=function(t,n,e){return t[0]=n[0]*e,t[1]=n[1]*e,t[2]=n[2]*e,t},n.scaleAndAdd=function(t,n,e,r){return t[0]=n[0]+e[0]*r,t[1]=n[1]+e[1]*r,t[2]=n[2]+e[2]*r,t},n.distance=h,n.squaredDistance=f,n.squaredLength=l,n.negate=function(t,n){return t[0]=-n[0],t[1]=-n[1],t[2]=-n[2],t},n.inverse=function(t,n){return t[0]=1/n[0],t[1]=1/n[1],t[2]=1/n[2],t},n.normalize=M,n.dot=b,n.cross=function(t,n,e){let r=n[0],a=n[1],o=n[2],i=e[0],u=e[1],c=e[2];return t[0]=a*c-o*u,t[1]=o*i-r*c,t[2]=r*u-a*i,t},n.lerp=function(t,n,e,r){let a=n[0],o=n[1],i=n[2];return t[0]=a+r*(e[0]-a),t[1]=o+r*(e[1]-o),t[2]=i+r*(e[2]-i),t},n.hermite=function(t,n,e,r,a,o){let i=o*o,u=i*(2*o-3)+1,c=i*(o-2)+o,s=i*(o-1),h=i*(3-2*o);return t[0]=n[0]*u+e[0]*c+r[0]*s+a[0]*h,t[1]=n[1]*u+e[1]*c+r[1]*s+a[1]*h,t[2]=n[2]*u+e[2]*c+r[2]*s+a[2]*h,t},n.bezier=function(t,n,e,r,a,o){let i=1-o,u=i*i,c=o*o,s=u*i,h=3*o*u,f=3*c*i,l=c*o;return t[0]=n[0]*s+e[0]*h+r[0]*f+a[0]*l,t[1]=n[1]*s+e[1]*h+r[1]*f+a[1]*l,t[2]=n[2]*s+e[2]*h+r[2]*f+a[2]*l,t},n.random=function(t,n){n=n||1;let e=2*r.c()*Math.PI,a=2*r.c()-1,o=Math.sqrt(1-a*a)*n;return t[0]=Math.cos(e)*o,t[1]=Math.sin(e)*o,t[2]=a*n,t},n.transformMat4=function(t,n,e){let r=n[0],a=n[1],o=n[2],i=e[3]*r+e[7]*a+e[11]*o+e[15];return i=i||1,t[0]=(e[0]*r+e[4]*a+e[8]*o+e[12])/i,t[1]=(e[1]*r+e[5]*a+e[9]*o+e[13])/i,t[2]=(e[2]*r+e[6]*a+e[10]*o+e[14])/i,t},n.transformMat3=function(t,n,e){let r=n[0],a=n[1],o=n[2];return t[0]=r*e[0]+a*e[3]+o*e[6],t[1]=r*e[1]+a*e[4]+o*e[7],t[2]=r*e[2]+a*e[5]+o*e[8],t},n.transformQuat=function(t,n,e){let r=n[0],a=n[1],o=n[2],i=e[0],u=e[1],c=e[2],s=e[3],h=s*r+u*o-c*a,f=s*a+c*r-i*o,l=s*o+i*a-u*r,M=-i*r-u*a-c*o;return t[0]=h*s+M*-i+f*-c-l*-u,t[1]=f*s+M*-u+l*-i-h*-c,t[2]=l*s+M*-c+h*-u-f*-i,t},n.rotateX=function(t,n,e,r){let a=[],o=[];return a[0]=n[0]-e[0],a[1]=n[1]-e[1],a[2]=n[2]-e[2],o[0]=a[0],o[1]=a[1]*Math.cos(r)-a[2]*Math.sin(r),o[2]=a[1]*Math.sin(r)+a[2]*Math.cos(r),t[0]=o[0]+e[0],t[1]=o[1]+e[1],t[2]=o[2]+e[2],t},n.rotateY=function(t,n,e,r){let a=[],o=[];return a[0]=n[0]-e[0],a[1]=n[1]-e[1],a[2]=n[2]-e[2],o[0]=a[2]*Math.sin(r)+a[0]*Math.cos(r),o[1]=a[1],o[2]=a[2]*Math.cos(r)-a[0]*Math.sin(r),t[0]=o[0]+e[0],t[1]=o[1]+e[1],t[2]=o[2]+e[2],t},n.rotateZ=function(t,n,e,r){let a=[],o=[];return a[0]=n[0]-e[0],a[1]=n[1]-e[1],a[2]=n[2]-e[2],o[0]=a[0]*Math.cos(r)-a[1]*Math.sin(r),o[1]=a[0]*Math.sin(r)+a[1]*Math.cos(r),o[2]=a[2],t[0]=o[0]+e[0],t[1]=o[1]+e[1],t[2]=o[2]+e[2],t},n.angle=function(t,n){let e=i(t[0],t[1],t[2]),r=i(n[0],n[1],n[2]);M(e,e),M(r,r);let a=b(e,r);return a>1?0:a<-1?Math.PI:Math.acos(a)},n.str=function(t){return"vec3("+t[0]+", "+t[1]+", "+t[2]+")"},n.exactEquals=function(t,n){return t[0]===n[0]&&t[1]===n[1]&&t[2]===n[2]},n.equals=function(t,n){let e=t[0],a=t[1],o=t[2],i=n[0],u=n[1],c=n[2];return Math.abs(e-i)<=r.b*Math.max(1,Math.abs(e),Math.abs(i))&&Math.abs(a-u)<=r.b*Math.max(1,Math.abs(a),Math.abs(u))&&Math.abs(o-c)<=r.b*Math.max(1,Math.abs(o),Math.abs(c))};var r=e(0);function a(){let t=new r.a(3);return t[0]=0,t[1]=0,t[2]=0,t}function o(t){let n=t[0],e=t[1],r=t[2];return Math.sqrt(n*n+e*e+r*r)}function i(t,n,e){let a=new r.a(3);return a[0]=t,a[1]=n,a[2]=e,a}function u(t,n,e){return t[0]=n[0]-e[0],t[1]=n[1]-e[1],t[2]=n[2]-e[2],t}function c(t,n,e){return t[0]=n[0]*e[0],t[1]=n[1]*e[1],t[2]=n[2]*e[2],t}function s(t,n,e){return t[0]=n[0]/e[0],t[1]=n[1]/e[1],t[2]=n[2]/e[2],t}function h(t,n){let e=n[0]-t[0],r=n[1]-t[1],a=n[2]-t[2];return Math.sqrt(e*e+r*r+a*a)}function f(t,n){let e=n[0]-t[0],r=n[1]-t[1],a=n[2]-t[2];return e*e+r*r+a*a}function l(t){let n=t[0],e=t[1],r=t[2];return n*n+e*e+r*r}function M(t,n){let e=n[0],r=n[1],a=n[2],o=e*e+r*r+a*a;return o>0&&(o=1/Math.sqrt(o),t[0]=n[0]*o,t[1]=n[1]*o,t[2]=n[2]*o),t}function b(t,n){return t[0]*n[0]+t[1]*n[1]+t[2]*n[2]}const m=u;n.sub=m;const d=c;n.mul=d;const _=s;n.div=_;const x=h;n.dist=x;const v=f;n.sqrDist=v;const g=o;n.len=g;const p=l;n.sqrLen=p;const w=function(){let t=a();return function(n,e,r,a,o,i){let u,c;for(e||(e=3),r||(r=0),c=a?Math.min(a*e+r,n.length):n.length,u=r;u<c;u+=e)t[0]=n[u],t[1]=n[u+1],t[2]=n[u+2],o(t,t,i),n[u]=t[0],n[u+1]=t[1],n[u+2]=t[2];return n}}();n.forEach=w},function(t,n,e){"use strict";n.b=function(t){let n=new r.a(4);return n[0]=t[0],n[1]=t[1],n[2]=t[2],n[3]=t[3],n},n.g=function(t,n,e,a){let o=new r.a(4);return o[0]=t,o[1]=n,o[2]=e,o[3]=a,o},n.c=function(t,n){return t[0]=n[0],t[1]=n[1],t[2]=n[2],t[3]=n[3],t},n.l=function(t,n,e,r,a){return t[0]=n,t[1]=e,t[2]=r,t[3]=a,t},n.a=function(t,n,e){return t[0]=n[0]+e[0],t[1]=n[1]+e[1],t[2]=n[2]+e[2],t[3]=n[3]+e[3],t},n.k=a,n.h=o,n.m=i,n.j=u,n.d=function(t,n){return t[0]*n[0]+t[1]*n[1]+t[2]*n[2]+t[3]*n[3]},n.i=function(t,n,e,r){let a=n[0],o=n[1],i=n[2],u=n[3];return t[0]=a+r*(e[0]-a),t[1]=o+r*(e[1]-o),t[2]=i+r*(e[2]-i),t[3]=u+r*(e[3]-u),t},n.f=function(t,n){return t[0]===n[0]&&t[1]===n[1]&&t[2]===n[2]&&t[3]===n[3]},n.e=function(t,n){let e=t[0],a=t[1],o=t[2],i=t[3],u=n[0],c=n[1],s=n[2],h=n[3];return Math.abs(e-u)<=r.b*Math.max(1,Math.abs(e),Math.abs(u))&&Math.abs(a-c)<=r.b*Math.max(1,Math.abs(a),Math.abs(c))&&Math.abs(o-s)<=r.b*Math.max(1,Math.abs(o),Math.abs(s))&&Math.abs(i-h)<=r.b*Math.max(1,Math.abs(i),Math.abs(h))};var r=e(0);function a(t,n,e){return t[0]=n[0]*e,t[1]=n[1]*e,t[2]=n[2]*e,t[3]=n[3]*e,t}function o(t){let n=t[0],e=t[1],r=t[2],a=t[3];return Math.sqrt(n*n+e*e+r*r+a*a)}function i(t){let n=t[0],e=t[1],r=t[2],a=t[3];return n*n+e*e+r*r+a*a}function u(t,n){let e=n[0],r=n[1],a=n[2],o=n[3],i=e*e+r*r+a*a+o*o;return i>0&&(i=1/Math.sqrt(i),t[0]=e*i,t[1]=r*i,t[2]=a*i,t[3]=o*i),t}!function(){let t=function(){let t=new r.a(4);return t[0]=0,t[1]=0,t[2]=0,t[3]=0,t}()}()},function(t,n,e){"use strict";Object.defineProperty(n,"__esModule",{value:!0});var r=e(3),a=(e(2),e(16)),o=r.a();a.a("viewport",o)},function(t,n,e){"use strict";e(0)},function(t,n,e){"use strict";e(0)},function(t,n,e){"use strict";Object.defineProperty(n,"__esModule",{value:!0}),n.create=function(){let t=new r.a(16);return t[0]=1,t[1]=0,t[2]=0,t[3]=0,t[4]=0,t[5]=1,t[6]=0,t[7]=0,t[8]=0,t[9]=0,t[10]=1,t[11]=0,t[12]=0,t[13]=0,t[14]=0,t[15]=1,t},n.clone=function(t){let n=new r.a(16);return n[0]=t[0],n[1]=t[1],n[2]=t[2],n[3]=t[3],n[4]=t[4],n[5]=t[5],n[6]=t[6],n[7]=t[7],n[8]=t[8],n[9]=t[9],n[10]=t[10],n[11]=t[11],n[12]=t[12],n[13]=t[13],n[14]=t[14],n[15]=t[15],n},n.copy=function(t,n){return t[0]=n[0],t[1]=n[1],t[2]=n[2],t[3]=n[3],t[4]=n[4],t[5]=n[5],t[6]=n[6],t[7]=n[7],t[8]=n[8],t[9]=n[9],t[10]=n[10],t[11]=n[11],t[12]=n[12],t[13]=n[13],t[14]=n[14],t[15]=n[15],t},n.fromValues=function(t,n,e,a,o,i,u,c,s,h,f,l,M,b,m,d){let _=new r.a(16);return _[0]=t,_[1]=n,_[2]=e,_[3]=a,_[4]=o,_[5]=i,_[6]=u,_[7]=c,_[8]=s,_[9]=h,_[10]=f,_[11]=l,_[12]=M,_[13]=b,_[14]=m,_[15]=d,_},n.set=function(t,n,e,r,a,o,i,u,c,s,h,f,l,M,b,m,d){return t[0]=n,t[1]=e,t[2]=r,t[3]=a,t[4]=o,t[5]=i,t[6]=u,t[7]=c,t[8]=s,t[9]=h,t[10]=f,t[11]=l,t[12]=M,t[13]=b,t[14]=m,t[15]=d,t},n.identity=a,n.transpose=function(t,n){if(t===n){let e=n[1],r=n[2],a=n[3],o=n[6],i=n[7],u=n[11];t[1]=n[4],t[2]=n[8],t[3]=n[12],t[4]=e,t[6]=n[9],t[7]=n[13],t[8]=r,t[9]=o,t[11]=n[14],t[12]=a,t[13]=i,t[14]=u}else t[0]=n[0],t[1]=n[4],t[2]=n[8],t[3]=n[12],t[4]=n[1],t[5]=n[5],t[6]=n[9],t[7]=n[13],t[8]=n[2],t[9]=n[6],t[10]=n[10],t[11]=n[14],t[12]=n[3],t[13]=n[7],t[14]=n[11],t[15]=n[15];return t},n.invert=function(t,n){let e=n[0],r=n[1],a=n[2],o=n[3],i=n[4],u=n[5],c=n[6],s=n[7],h=n[8],f=n[9],l=n[10],M=n[11],b=n[12],m=n[13],d=n[14],_=n[15],x=e*u-r*i,v=e*c-a*i,g=e*s-o*i,p=r*c-a*u,w=r*s-o*u,A=a*s-o*c,j=h*m-f*b,q=h*d-l*b,P=h*_-M*b,E=f*d-l*m,O=f*_-M*m,R=l*_-M*d,y=x*R-v*O+g*E+p*P-w*q+A*j;if(!y)return null;return y=1/y,t[0]=(u*R-c*O+s*E)*y,t[1]=(a*O-r*R-o*E)*y,t[2]=(m*A-d*w+_*p)*y,t[3]=(l*w-f*A-M*p)*y,t[4]=(c*P-i*R-s*q)*y,t[5]=(e*R-a*P+o*q)*y,t[6]=(d*g-b*A-_*v)*y,t[7]=(h*A-l*g+M*v)*y,t[8]=(i*O-u*P+s*j)*y,t[9]=(r*P-e*O-o*j)*y,t[10]=(b*w-m*g+_*x)*y,t[11]=(f*g-h*w-M*x)*y,t[12]=(u*q-i*E-c*j)*y,t[13]=(e*E-r*q+a*j)*y,t[14]=(m*v-b*p-d*x)*y,t[15]=(h*p-f*v+l*x)*y,t},n.adjoint=function(t,n){let e=n[0],r=n[1],a=n[2],o=n[3],i=n[4],u=n[5],c=n[6],s=n[7],h=n[8],f=n[9],l=n[10],M=n[11],b=n[12],m=n[13],d=n[14],_=n[15];return t[0]=u*(l*_-M*d)-f*(c*_-s*d)+m*(c*M-s*l),t[1]=-(r*(l*_-M*d)-f*(a*_-o*d)+m*(a*M-o*l)),t[2]=r*(c*_-s*d)-u*(a*_-o*d)+m*(a*s-o*c),t[3]=-(r*(c*M-s*l)-u*(a*M-o*l)+f*(a*s-o*c)),t[4]=-(i*(l*_-M*d)-h*(c*_-s*d)+b*(c*M-s*l)),t[5]=e*(l*_-M*d)-h*(a*_-o*d)+b*(a*M-o*l),t[6]=-(e*(c*_-s*d)-i*(a*_-o*d)+b*(a*s-o*c)),t[7]=e*(c*M-s*l)-i*(a*M-o*l)+h*(a*s-o*c),t[8]=i*(f*_-M*m)-h*(u*_-s*m)+b*(u*M-s*f),t[9]=-(e*(f*_-M*m)-h*(r*_-o*m)+b*(r*M-o*f)),t[10]=e*(u*_-s*m)-i*(r*_-o*m)+b*(r*s-o*u),t[11]=-(e*(u*M-s*f)-i*(r*M-o*f)+h*(r*s-o*u)),t[12]=-(i*(f*d-l*m)-h*(u*d-c*m)+b*(u*l-c*f)),t[13]=e*(f*d-l*m)-h*(r*d-a*m)+b*(r*l-a*f),t[14]=-(e*(u*d-c*m)-i*(r*d-a*m)+b*(r*c-a*u)),t[15]=e*(u*l-c*f)-i*(r*l-a*f)+h*(r*c-a*u),t},n.determinant=function(t){let n=t[0],e=t[1],r=t[2],a=t[3],o=t[4],i=t[5],u=t[6],c=t[7],s=t[8],h=t[9],f=t[10],l=t[11],M=t[12],b=t[13],m=t[14],d=t[15];return(n*i-e*o)*(f*d-l*m)-(n*u-r*o)*(h*d-l*b)+(n*c-a*o)*(h*m-f*b)+(e*u-r*i)*(s*d-l*M)-(e*c-a*i)*(s*m-f*M)+(r*c-a*u)*(s*b-h*M)},n.multiply=o,n.translate=function(t,n,e){let r,a,o,i,u,c,s,h,f,l,M,b,m=e[0],d=e[1],_=e[2];n===t?(t[12]=n[0]*m+n[4]*d+n[8]*_+n[12],t[13]=n[1]*m+n[5]*d+n[9]*_+n[13],t[14]=n[2]*m+n[6]*d+n[10]*_+n[14],t[15]=n[3]*m+n[7]*d+n[11]*_+n[15]):(r=n[0],a=n[1],o=n[2],i=n[3],u=n[4],c=n[5],s=n[6],h=n[7],f=n[8],l=n[9],M=n[10],b=n[11],t[0]=r,t[1]=a,t[2]=o,t[3]=i,t[4]=u,t[5]=c,t[6]=s,t[7]=h,t[8]=f,t[9]=l,t[10]=M,t[11]=b,t[12]=r*m+u*d+f*_+n[12],t[13]=a*m+c*d+l*_+n[13],t[14]=o*m+s*d+M*_+n[14],t[15]=i*m+h*d+b*_+n[15]);return t},n.scale=function(t,n,e){let r=e[0],a=e[1],o=e[2];return t[0]=n[0]*r,t[1]=n[1]*r,t[2]=n[2]*r,t[3]=n[3]*r,t[4]=n[4]*a,t[5]=n[5]*a,t[6]=n[6]*a,t[7]=n[7]*a,t[8]=n[8]*o,t[9]=n[9]*o,t[10]=n[10]*o,t[11]=n[11]*o,t[12]=n[12],t[13]=n[13],t[14]=n[14],t[15]=n[15],t},n.rotate=function(t,n,e,a){let o,i,u,c,s,h,f,l,M,b,m,d,_,x,v,g,p,w,A,j,q,P,E,O,R=a[0],y=a[1],S=a[2],T=Math.sqrt(R*R+y*y+S*S);if(Math.abs(T)<r.b)return null;R*=T=1/T,y*=T,S*=T,o=Math.sin(e),i=Math.cos(e),u=1-i,c=n[0],s=n[1],h=n[2],f=n[3],l=n[4],M=n[5],b=n[6],m=n[7],d=n[8],_=n[9],x=n[10],v=n[11],g=R*R*u+i,p=y*R*u+S*o,w=S*R*u-y*o,A=R*y*u-S*o,j=y*y*u+i,q=S*y*u+R*o,P=R*S*u+y*o,E=y*S*u-R*o,O=S*S*u+i,t[0]=c*g+l*p+d*w,t[1]=s*g+M*p+_*w,t[2]=h*g+b*p+x*w,t[3]=f*g+m*p+v*w,t[4]=c*A+l*j+d*q,t[5]=s*A+M*j+_*q,t[6]=h*A+b*j+x*q,t[7]=f*A+m*j+v*q,t[8]=c*P+l*E+d*O,t[9]=s*P+M*E+_*O,t[10]=h*P+b*E+x*O,t[11]=f*P+m*E+v*O,n!==t&&(t[12]=n[12],t[13]=n[13],t[14]=n[14],t[15]=n[15]);return t},n.rotateX=function(t,n,e){let r=Math.sin(e),a=Math.cos(e),o=n[4],i=n[5],u=n[6],c=n[7],s=n[8],h=n[9],f=n[10],l=n[11];n!==t&&(t[0]=n[0],t[1]=n[1],t[2]=n[2],t[3]=n[3],t[12]=n[12],t[13]=n[13],t[14]=n[14],t[15]=n[15]);return t[4]=o*a+s*r,t[5]=i*a+h*r,t[6]=u*a+f*r,t[7]=c*a+l*r,t[8]=s*a-o*r,t[9]=h*a-i*r,t[10]=f*a-u*r,t[11]=l*a-c*r,t},n.rotateY=function(t,n,e){let r=Math.sin(e),a=Math.cos(e),o=n[0],i=n[1],u=n[2],c=n[3],s=n[8],h=n[9],f=n[10],l=n[11];n!==t&&(t[4]=n[4],t[5]=n[5],t[6]=n[6],t[7]=n[7],t[12]=n[12],t[13]=n[13],t[14]=n[14],t[15]=n[15]);return t[0]=o*a-s*r,t[1]=i*a-h*r,t[2]=u*a-f*r,t[3]=c*a-l*r,t[8]=o*r+s*a,t[9]=i*r+h*a,t[10]=u*r+f*a,t[11]=c*r+l*a,t},n.rotateZ=function(t,n,e){let r=Math.sin(e),a=Math.cos(e),o=n[0],i=n[1],u=n[2],c=n[3],s=n[4],h=n[5],f=n[6],l=n[7];n!==t&&(t[8]=n[8],t[9]=n[9],t[10]=n[10],t[11]=n[11],t[12]=n[12],t[13]=n[13],t[14]=n[14],t[15]=n[15]);return t[0]=o*a+s*r,t[1]=i*a+h*r,t[2]=u*a+f*r,t[3]=c*a+l*r,t[4]=s*a-o*r,t[5]=h*a-i*r,t[6]=f*a-u*r,t[7]=l*a-c*r,t},n.fromTranslation=function(t,n){return t[0]=1,t[1]=0,t[2]=0,t[3]=0,t[4]=0,t[5]=1,t[6]=0,t[7]=0,t[8]=0,t[9]=0,t[10]=1,t[11]=0,t[12]=n[0],t[13]=n[1],t[14]=n[2],t[15]=1,t},n.fromScaling=function(t,n){return t[0]=n[0],t[1]=0,t[2]=0,t[3]=0,t[4]=0,t[5]=n[1],t[6]=0,t[7]=0,t[8]=0,t[9]=0,t[10]=n[2],t[11]=0,t[12]=0,t[13]=0,t[14]=0,t[15]=1,t},n.fromRotation=function(t,n,e){let a,o,i,u=e[0],c=e[1],s=e[2],h=Math.sqrt(u*u+c*c+s*s);if(Math.abs(h)<r.b)return null;return u*=h=1/h,c*=h,s*=h,a=Math.sin(n),o=Math.cos(n),i=1-o,t[0]=u*u*i+o,t[1]=c*u*i+s*a,t[2]=s*u*i-c*a,t[3]=0,t[4]=u*c*i-s*a,t[5]=c*c*i+o,t[6]=s*c*i+u*a,t[7]=0,t[8]=u*s*i+c*a,t[9]=c*s*i-u*a,t[10]=s*s*i+o,t[11]=0,t[12]=0,t[13]=0,t[14]=0,t[15]=1,t},n.fromXRotation=function(t,n){let e=Math.sin(n),r=Math.cos(n);return t[0]=1,t[1]=0,t[2]=0,t[3]=0,t[4]=0,t[5]=r,t[6]=e,t[7]=0,t[8]=0,t[9]=-e,t[10]=r,t[11]=0,t[12]=0,t[13]=0,t[14]=0,t[15]=1,t},n.fromYRotation=function(t,n){let e=Math.sin(n),r=Math.cos(n);return t[0]=r,t[1]=0,t[2]=-e,t[3]=0,t[4]=0,t[5]=1,t[6]=0,t[7]=0,t[8]=e,t[9]=0,t[10]=r,t[11]=0,t[12]=0,t[13]=0,t[14]=0,t[15]=1,t},n.fromZRotation=function(t,n){let e=Math.sin(n),r=Math.cos(n);return t[0]=r,t[1]=e,t[2]=0,t[3]=0,t[4]=-e,t[5]=r,t[6]=0,t[7]=0,t[8]=0,t[9]=0,t[10]=1,t[11]=0,t[12]=0,t[13]=0,t[14]=0,t[15]=1,t},n.fromRotationTranslation=function(t,n,e){let r=n[0],a=n[1],o=n[2],i=n[3],u=r+r,c=a+a,s=o+o,h=r*u,f=r*c,l=r*s,M=a*c,b=a*s,m=o*s,d=i*u,_=i*c,x=i*s;return t[0]=1-(M+m),t[1]=f+x,t[2]=l-_,t[3]=0,t[4]=f-x,t[5]=1-(h+m),t[6]=b+d,t[7]=0,t[8]=l+_,t[9]=b-d,t[10]=1-(h+M),t[11]=0,t[12]=e[0],t[13]=e[1],t[14]=e[2],t[15]=1,t},n.getTranslation=function(t,n){return t[0]=n[12],t[1]=n[13],t[2]=n[14],t},n.getScaling=function(t,n){let e=n[0],r=n[1],a=n[2],o=n[4],i=n[5],u=n[6],c=n[8],s=n[9],h=n[10];return t[0]=Math.sqrt(e*e+r*r+a*a),t[1]=Math.sqrt(o*o+i*i+u*u),t[2]=Math.sqrt(c*c+s*s+h*h),t},n.getRotation=function(t,n){let e=n[0]+n[5]+n[10],r=0;e>0?(r=2*Math.sqrt(e+1),t[3]=.25*r,t[0]=(n[6]-n[9])/r,t[1]=(n[8]-n[2])/r,t[2]=(n[1]-n[4])/r):n[0]>n[5]&n[0]>n[10]?(r=2*Math.sqrt(1+n[0]-n[5]-n[10]),t[3]=(n[6]-n[9])/r,t[0]=.25*r,t[1]=(n[1]+n[4])/r,t[2]=(n[8]+n[2])/r):n[5]>n[10]?(r=2*Math.sqrt(1+n[5]-n[0]-n[10]),t[3]=(n[8]-n[2])/r,t[0]=(n[1]+n[4])/r,t[1]=.25*r,t[2]=(n[6]+n[9])/r):(r=2*Math.sqrt(1+n[10]-n[0]-n[5]),t[3]=(n[1]-n[4])/r,t[0]=(n[8]+n[2])/r,t[1]=(n[6]+n[9])/r,t[2]=.25*r);return t},n.fromRotationTranslationScale=function(t,n,e,r){let a=n[0],o=n[1],i=n[2],u=n[3],c=a+a,s=o+o,h=i+i,f=a*c,l=a*s,M=a*h,b=o*s,m=o*h,d=i*h,_=u*c,x=u*s,v=u*h,g=r[0],p=r[1],w=r[2];return t[0]=(1-(b+d))*g,t[1]=(l+v)*g,t[2]=(M-x)*g,t[3]=0,t[4]=(l-v)*p,t[5]=(1-(f+d))*p,t[6]=(m+_)*p,t[7]=0,t[8]=(M+x)*w,t[9]=(m-_)*w,t[10]=(1-(f+b))*w,t[11]=0,t[12]=e[0],t[13]=e[1],t[14]=e[2],t[15]=1,t},n.fromRotationTranslationScaleOrigin=function(t,n,e,r,a){let o=n[0],i=n[1],u=n[2],c=n[3],s=o+o,h=i+i,f=u+u,l=o*s,M=o*h,b=o*f,m=i*h,d=i*f,_=u*f,x=c*s,v=c*h,g=c*f,p=r[0],w=r[1],A=r[2],j=a[0],q=a[1],P=a[2];return t[0]=(1-(m+_))*p,t[1]=(M+g)*p,t[2]=(b-v)*p,t[3]=0,t[4]=(M-g)*w,t[5]=(1-(l+_))*w,t[6]=(d+x)*w,t[7]=0,t[8]=(b+v)*A,t[9]=(d-x)*A,t[10]=(1-(l+m))*A,t[11]=0,t[12]=e[0]+j-(t[0]*j+t[4]*q+t[8]*P),t[13]=e[1]+q-(t[1]*j+t[5]*q+t[9]*P),t[14]=e[2]+P-(t[2]*j+t[6]*q+t[10]*P),t[15]=1,t},n.fromQuat=function(t,n){let e=n[0],r=n[1],a=n[2],o=n[3],i=e+e,u=r+r,c=a+a,s=e*i,h=r*i,f=r*u,l=a*i,M=a*u,b=a*c,m=o*i,d=o*u,_=o*c;return t[0]=1-f-b,t[1]=h+_,t[2]=l-d,t[3]=0,t[4]=h-_,t[5]=1-s-b,t[6]=M+m,t[7]=0,t[8]=l+d,t[9]=M-m,t[10]=1-s-f,t[11]=0,t[12]=0,t[13]=0,t[14]=0,t[15]=1,t},n.frustum=function(t,n,e,r,a,o,i){let u=1/(e-n),c=1/(a-r),s=1/(o-i);return t[0]=2*o*u,t[1]=0,t[2]=0,t[3]=0,t[4]=0,t[5]=2*o*c,t[6]=0,t[7]=0,t[8]=(e+n)*u,t[9]=(a+r)*c,t[10]=(i+o)*s,t[11]=-1,t[12]=0,t[13]=0,t[14]=i*o*2*s,t[15]=0,t},n.perspective=function(t,n,e,r,a){let o=1/Math.tan(n/2),i=1/(r-a);return t[0]=o/e,t[1]=0,t[2]=0,t[3]=0,t[4]=0,t[5]=o,t[6]=0,t[7]=0,t[8]=0,t[9]=0,t[10]=(a+r)*i,t[11]=-1,t[12]=0,t[13]=0,t[14]=2*a*r*i,t[15]=0,t},n.perspectiveFromFieldOfView=function(t,n,e,r){let a=Math.tan(n.upDegrees*Math.PI/180),o=Math.tan(n.downDegrees*Math.PI/180),i=Math.tan(n.leftDegrees*Math.PI/180),u=Math.tan(n.rightDegrees*Math.PI/180),c=2/(i+u),s=2/(a+o);return t[0]=c,t[1]=0,t[2]=0,t[3]=0,t[4]=0,t[5]=s,t[6]=0,t[7]=0,t[8]=-(i-u)*c*.5,t[9]=(a-o)*s*.5,t[10]=r/(e-r),t[11]=-1,t[12]=0,t[13]=0,t[14]=r*e/(e-r),t[15]=0,t},n.ortho=function(t,n,e,r,a,o,i){let u=1/(n-e),c=1/(r-a),s=1/(o-i);return t[0]=-2*u,t[1]=0,t[2]=0,t[3]=0,t[4]=0,t[5]=-2*c,t[6]=0,t[7]=0,t[8]=0,t[9]=0,t[10]=2*s,t[11]=0,t[12]=(n+e)*u,t[13]=(a+r)*c,t[14]=(i+o)*s,t[15]=1,t},n.lookAt=function(t,n,e,o){let i,u,c,s,h,f,l,M,b,m,d=n[0],_=n[1],x=n[2],v=o[0],g=o[1],p=o[2],w=e[0],A=e[1],j=e[2];if(Math.abs(d-w)<r.b&&Math.abs(_-A)<r.b&&Math.abs(x-j)<r.b)return a(t);l=d-w,M=_-A,b=x-j,m=1/Math.sqrt(l*l+M*M+b*b),i=g*(b*=m)-p*(M*=m),u=p*(l*=m)-v*b,c=v*M-g*l,(m=Math.sqrt(i*i+u*u+c*c))?(i*=m=1/m,u*=m,c*=m):(i=0,u=0,c=0);s=M*c-b*u,h=b*i-l*c,f=l*u-M*i,(m=Math.sqrt(s*s+h*h+f*f))?(s*=m=1/m,h*=m,f*=m):(s=0,h=0,f=0);return t[0]=i,t[1]=s,t[2]=l,t[3]=0,t[4]=u,t[5]=h,t[6]=M,t[7]=0,t[8]=c,t[9]=f,t[10]=b,t[11]=0,t[12]=-(i*d+u*_+c*x),t[13]=-(s*d+h*_+f*x),t[14]=-(l*d+M*_+b*x),t[15]=1,t},n.targetTo=function(t,n,e,r){let a=n[0],o=n[1],i=n[2],u=r[0],c=r[1],s=r[2],h=a-e[0],f=o-e[1],l=i-e[2],M=h*h+f*f+l*l;M>0&&(M=1/Math.sqrt(M),h*=M,f*=M,l*=M);let b=c*l-s*f,m=s*h-u*l,d=u*f-c*h;return t[0]=b,t[1]=m,t[2]=d,t[3]=0,t[4]=f*d-l*m,t[5]=l*b-h*d,t[6]=h*m-f*b,t[7]=0,t[8]=h,t[9]=f,t[10]=l,t[11]=0,t[12]=a,t[13]=o,t[14]=i,t[15]=1,t},n.str=function(t){return"mat4("+t[0]+", "+t[1]+", "+t[2]+", "+t[3]+", "+t[4]+", "+t[5]+", "+t[6]+", "+t[7]+", "+t[8]+", "+t[9]+", "+t[10]+", "+t[11]+", "+t[12]+", "+t[13]+", "+t[14]+", "+t[15]+")"},n.frob=function(t){return Math.sqrt(Math.pow(t[0],2)+Math.pow(t[1],2)+Math.pow(t[2],2)+Math.pow(t[3],2)+Math.pow(t[4],2)+Math.pow(t[5],2)+Math.pow(t[6],2)+Math.pow(t[7],2)+Math.pow(t[8],2)+Math.pow(t[9],2)+Math.pow(t[10],2)+Math.pow(t[11],2)+Math.pow(t[12],2)+Math.pow(t[13],2)+Math.pow(t[14],2)+Math.pow(t[15],2))},n.add=function(t,n,e){return t[0]=n[0]+e[0],t[1]=n[1]+e[1],t[2]=n[2]+e[2],t[3]=n[3]+e[3],t[4]=n[4]+e[4],t[5]=n[5]+e[5],t[6]=n[6]+e[6],t[7]=n[7]+e[7],t[8]=n[8]+e[8],t[9]=n[9]+e[9],t[10]=n[10]+e[10],t[11]=n[11]+e[11],t[12]=n[12]+e[12],t[13]=n[13]+e[13],t[14]=n[14]+e[14],t[15]=n[15]+e[15],t},n.subtract=i,n.multiplyScalar=function(t,n,e){return t[0]=n[0]*e,t[1]=n[1]*e,t[2]=n[2]*e,t[3]=n[3]*e,t[4]=n[4]*e,t[5]=n[5]*e,t[6]=n[6]*e,t[7]=n[7]*e,t[8]=n[8]*e,t[9]=n[9]*e,t[10]=n[10]*e,t[11]=n[11]*e,t[12]=n[12]*e,t[13]=n[13]*e,t[14]=n[14]*e,t[15]=n[15]*e,t},n.multiplyScalarAndAdd=function(t,n,e,r){return t[0]=n[0]+e[0]*r,t[1]=n[1]+e[1]*r,t[2]=n[2]+e[2]*r,t[3]=n[3]+e[3]*r,t[4]=n[4]+e[4]*r,t[5]=n[5]+e[5]*r,t[6]=n[6]+e[6]*r,t[7]=n[7]+e[7]*r,t[8]=n[8]+e[8]*r,t[9]=n[9]+e[9]*r,t[10]=n[10]+e[10]*r,t[11]=n[11]+e[11]*r,t[12]=n[12]+e[12]*r,t[13]=n[13]+e[13]*r,t[14]=n[14]+e[14]*r,t[15]=n[15]+e[15]*r,t},n.exactEquals=function(t,n){return t[0]===n[0]&&t[1]===n[1]&&t[2]===n[2]&&t[3]===n[3]&&t[4]===n[4]&&t[5]===n[5]&&t[6]===n[6]&&t[7]===n[7]&&t[8]===n[8]&&t[9]===n[9]&&t[10]===n[10]&&t[11]===n[11]&&t[12]===n[12]&&t[13]===n[13]&&t[14]===n[14]&&t[15]===n[15]},n.equals=function(t,n){let e=t[0],a=t[1],o=t[2],i=t[3],u=t[4],c=t[5],s=t[6],h=t[7],f=t[8],l=t[9],M=t[10],b=t[11],m=t[12],d=t[13],_=t[14],x=t[15],v=n[0],g=n[1],p=n[2],w=n[3],A=n[4],j=n[5],q=n[6],P=n[7],E=n[8],O=n[9],R=n[10],y=n[11],S=n[12],T=n[13],L=n[14],F=n[15];return Math.abs(e-v)<=r.b*Math.max(1,Math.abs(e),Math.abs(v))&&Math.abs(a-g)<=r.b*Math.max(1,Math.abs(a),Math.abs(g))&&Math.abs(o-p)<=r.b*Math.max(1,Math.abs(o),Math.abs(p))&&Math.abs(i-w)<=r.b*Math.max(1,Math.abs(i),Math.abs(w))&&Math.abs(u-A)<=r.b*Math.max(1,Math.abs(u),Math.abs(A))&&Math.abs(c-j)<=r.b*Math.max(1,Math.abs(c),Math.abs(j))&&Math.abs(s-q)<=r.b*Math.max(1,Math.abs(s),Math.abs(q))&&Math.abs(h-P)<=r.b*Math.max(1,Math.abs(h),Math.abs(P))&&Math.abs(f-E)<=r.b*Math.max(1,Math.abs(f),Math.abs(E))&&Math.abs(l-O)<=r.b*Math.max(1,Math.abs(l),Math.abs(O))&&Math.abs(M-R)<=r.b*Math.max(1,Math.abs(M),Math.abs(R))&&Math.abs(b-y)<=r.b*Math.max(1,Math.abs(b),Math.abs(y))&&Math.abs(m-S)<=r.b*Math.max(1,Math.abs(m),Math.abs(S))&&Math.abs(d-T)<=r.b*Math.max(1,Math.abs(d),Math.abs(T))&&Math.abs(_-L)<=r.b*Math.max(1,Math.abs(_),Math.abs(L))&&Math.abs(x-F)<=r.b*Math.max(1,Math.abs(x),Math.abs(F))};var r=e(0);function a(t){return t[0]=1,t[1]=0,t[2]=0,t[3]=0,t[4]=0,t[5]=1,t[6]=0,t[7]=0,t[8]=0,t[9]=0,t[10]=1,t[11]=0,t[12]=0,t[13]=0,t[14]=0,t[15]=1,t}function o(t,n,e){let r=n[0],a=n[1],o=n[2],i=n[3],u=n[4],c=n[5],s=n[6],h=n[7],f=n[8],l=n[9],M=n[10],b=n[11],m=n[12],d=n[13],_=n[14],x=n[15],v=e[0],g=e[1],p=e[2],w=e[3];return t[0]=v*r+g*u+p*f+w*m,t[1]=v*a+g*c+p*l+w*d,t[2]=v*o+g*s+p*M+w*_,t[3]=v*i+g*h+p*b+w*x,v=e[4],g=e[5],p=e[6],w=e[7],t[4]=v*r+g*u+p*f+w*m,t[5]=v*a+g*c+p*l+w*d,t[6]=v*o+g*s+p*M+w*_,t[7]=v*i+g*h+p*b+w*x,v=e[8],g=e[9],p=e[10],w=e[11],t[8]=v*r+g*u+p*f+w*m,t[9]=v*a+g*c+p*l+w*d,t[10]=v*o+g*s+p*M+w*_,t[11]=v*i+g*h+p*b+w*x,v=e[12],g=e[13],p=e[14],w=e[15],t[12]=v*r+g*u+p*f+w*m,t[13]=v*a+g*c+p*l+w*d,t[14]=v*o+g*s+p*M+w*_,t[15]=v*i+g*h+p*b+w*x,t}function i(t,n,e){return t[0]=n[0]-e[0],t[1]=n[1]-e[1],t[2]=n[2]-e[2],t[3]=n[3]-e[3],t[4]=n[4]-e[4],t[5]=n[5]-e[5],t[6]=n[6]-e[6],t[7]=n[7]-e[7],t[8]=n[8]-e[8],t[9]=n[9]-e[9],t[10]=n[10]-e[10],t[11]=n[11]-e[11],t[12]=n[12]-e[12],t[13]=n[13]-e[13],t[14]=n[14]-e[14],t[15]=n[15]-e[15],t}const u=o;n.mul=u;const c=i;n.sub=c},function(t,n,e){"use strict";Object.defineProperty(n,"__esModule",{value:!0}),n.create=u,n.identity=function(t){return t[0]=0,t[1]=0,t[2]=0,t[3]=1,t},n.setAxisAngle=c,n.getAxisAngle=function(t,n){let e=2*Math.acos(n[3]),r=Math.sin(e/2);0!=r?(t[0]=n[0]/r,t[1]=n[1]/r,t[2]=n[2]/r):(t[0]=1,t[1]=0,t[2]=0);return e},n.multiply=s,n.rotateX=function(t,n,e){e*=.5;let r=n[0],a=n[1],o=n[2],i=n[3],u=Math.sin(e),c=Math.cos(e);return t[0]=r*c+i*u,t[1]=a*c+o*u,t[2]=o*c-a*u,t[3]=i*c-r*u,t},n.rotateY=function(t,n,e){e*=.5;let r=n[0],a=n[1],o=n[2],i=n[3],u=Math.sin(e),c=Math.cos(e);return t[0]=r*c-o*u,t[1]=a*c+i*u,t[2]=o*c+r*u,t[3]=i*c-a*u,t},n.rotateZ=function(t,n,e){e*=.5;let r=n[0],a=n[1],o=n[2],i=n[3],u=Math.sin(e),c=Math.cos(e);return t[0]=r*c+a*u,t[1]=a*c-r*u,t[2]=o*c+i*u,t[3]=i*c-o*u,t},n.calculateW=function(t,n){let e=n[0],r=n[1],a=n[2];return t[0]=e,t[1]=r,t[2]=a,t[3]=Math.sqrt(Math.abs(1-e*e-r*r-a*a)),t},n.slerp=h,n.invert=function(t,n){let e=n[0],r=n[1],a=n[2],o=n[3],i=e*e+r*r+a*a+o*o,u=i?1/i:0;return t[0]=-e*u,t[1]=-r*u,t[2]=-a*u,t[3]=o*u,t},n.conjugate=function(t,n){return t[0]=-n[0],t[1]=-n[1],t[2]=-n[2],t[3]=n[3],t},n.fromMat3=f,n.fromEuler=function(t,n,e,r){let a=.5*Math.PI/180;n*=a,e*=a,r*=a;let o=Math.sin(n),i=Math.cos(n),u=Math.sin(e),c=Math.cos(e),s=Math.sin(r),h=Math.cos(r);return t[0]=o*c*h-i*u*s,t[1]=i*u*h+o*c*s,t[2]=i*c*s-o*u*h,t[3]=i*c*h+o*u*s,t},n.str=function(t){return"quat("+t[0]+", "+t[1]+", "+t[2]+", "+t[3]+")"};var r=e(0),a=e(4),o=e(5),i=e(6);function u(){let t=new r.a(4);return t[0]=0,t[1]=0,t[2]=0,t[3]=1,t}function c(t,n,e){e*=.5;let r=Math.sin(e);return t[0]=r*n[0],t[1]=r*n[1],t[2]=r*n[2],t[3]=Math.cos(e),t}function s(t,n,e){let r=n[0],a=n[1],o=n[2],i=n[3],u=e[0],c=e[1],s=e[2],h=e[3];return t[0]=r*h+i*u+a*s-o*c,t[1]=a*h+i*c+o*u-r*s,t[2]=o*h+i*s+r*c-a*u,t[3]=i*h-r*u-a*c-o*s,t}function h(t,n,e,r){let a,o,i,u,c,s=n[0],h=n[1],f=n[2],l=n[3],M=e[0],b=e[1],m=e[2],d=e[3];return(o=s*M+h*b+f*m+l*d)<0&&(o=-o,M=-M,b=-b,m=-m,d=-d),1-o>1e-6?(a=Math.acos(o),i=Math.sin(a),u=Math.sin((1-r)*a)/i,c=Math.sin(r*a)/i):(u=1-r,c=r),t[0]=u*s+c*M,t[1]=u*h+c*b,t[2]=u*f+c*m,t[3]=u*l+c*d,t}function f(t,n){let e,r=n[0]+n[4]+n[8];if(r>0)e=Math.sqrt(r+1),t[3]=.5*e,e=.5/e,t[0]=(n[5]-n[7])*e,t[1]=(n[6]-n[2])*e,t[2]=(n[1]-n[3])*e;else{let r=0;n[4]>n[0]&&(r=1),n[8]>n[3*r+r]&&(r=2);let a=(r+1)%3,o=(r+2)%3;e=Math.sqrt(n[3*r+r]-n[3*a+a]-n[3*o+o]+1),t[r]=.5*e,e=.5/e,t[3]=(n[3*a+o]-n[3*o+a])*e,t[a]=(n[3*a+r]+n[3*r+a])*e,t[o]=(n[3*o+r]+n[3*r+o])*e}return t}const l=i.b;n.clone=l;const M=i.g;n.fromValues=M;const b=i.c;n.copy=b;const m=i.l;n.set=m;const d=i.a;n.add=d;const _=s;n.mul=_;const x=i.k;n.scale=x;const v=i.d;n.dot=v;const g=i.i;n.lerp=g;const p=i.h;n.length=p;const w=p;n.len=w;const A=i.m;n.squaredLength=A;const j=A;n.sqrLen=j;const q=i.j;n.normalize=q;const P=i.f;n.exactEquals=P;const E=i.e;n.equals=E;const O=function(){let t=o.create(),n=o.fromValues(1,0,0),e=o.fromValues(0,1,0);return function(r,a,i){let u=o.dot(a,i);return u<-.999999?(o.cross(t,n,a),o.len(t)<1e-6&&o.cross(t,e,a),o.normalize(t,t),c(r,t,Math.PI),r):u>.999999?(r[0]=0,r[1]=0,r[2]=0,r[3]=1,r):(o.cross(t,a,i),r[0]=t[0],r[1]=t[1],r[2]=t[2],r[3]=1+u,q(r,r))}}();n.rotationTo=O;const R=function(){let t=u(),n=u();return function(e,r,a,o,i,u){return h(t,r,i,u),h(n,a,o,u),h(e,t,n,2*u*(1-u)),e}}();n.sqlerp=R;const y=function(){let t=a.a();return function(n,e,r,a){return t[0]=r[0],t[3]=r[1],t[6]=r[2],t[1]=a[0],t[4]=a[1],t[7]=a[2],t[2]=-e[0],t[5]=-e[1],t[8]=-e[2],q(n,f(n,t))}}();n.setAxes=y},function(t,n,e){"use strict";Object.defineProperty(n,"__esModule",{value:!0}),n.create=a,n.clone=function(t){let n=new r.a(2);return n[0]=t[0],n[1]=t[1],n},n.fromValues=function(t,n){let e=new r.a(2);return e[0]=t,e[1]=n,e},n.copy=function(t,n){return t[0]=n[0],t[1]=n[1],t},n.set=function(t,n,e){return t[0]=n,t[1]=e,t},n.add=function(t,n,e){return t[0]=n[0]+e[0],t[1]=n[1]+e[1],t},n.subtract=o,n.multiply=i,n.divide=u,n.ceil=function(t,n){return t[0]=Math.ceil(n[0]),t[1]=Math.ceil(n[1]),t},n.floor=function(t,n){return t[0]=Math.floor(n[0]),t[1]=Math.floor(n[1]),t},n.min=function(t,n,e){return t[0]=Math.min(n[0],e[0]),t[1]=Math.min(n[1],e[1]),t},n.max=function(t,n,e){return t[0]=Math.max(n[0],e[0]),t[1]=Math.max(n[1],e[1]),t},n.round=function(t,n){return t[0]=Math.round(n[0]),t[1]=Math.round(n[1]),t},n.scale=function(t,n,e){return t[0]=n[0]*e,t[1]=n[1]*e,t},n.scaleAndAdd=function(t,n,e,r){return t[0]=n[0]+e[0]*r,t[1]=n[1]+e[1]*r,t},n.distance=c,n.squaredDistance=s,n.length=h,n.squaredLength=f,n.negate=function(t,n){return t[0]=-n[0],t[1]=-n[1],t},n.inverse=function(t,n){return t[0]=1/n[0],t[1]=1/n[1],t},n.normalize=function(t,n){var e=n[0],r=n[1],a=e*e+r*r;a>0&&(a=1/Math.sqrt(a),t[0]=n[0]*a,t[1]=n[1]*a);return t},n.dot=function(t,n){return t[0]*n[0]+t[1]*n[1]},n.cross=function(t,n,e){var r=n[0]*e[1]-n[1]*e[0];return t[0]=t[1]=0,t[2]=r,t},n.lerp=function(t,n,e,r){var a=n[0],o=n[1];return t[0]=a+r*(e[0]-a),t[1]=o+r*(e[1]-o),t},n.random=function(t,n){n=n||1;var e=2*r.c()*Math.PI;return t[0]=Math.cos(e)*n,t[1]=Math.sin(e)*n,t},n.transformMat2=function(t,n,e){var r=n[0],a=n[1];return t[0]=e[0]*r+e[2]*a,t[1]=e[1]*r+e[3]*a,t},n.transformMat2d=function(t,n,e){var r=n[0],a=n[1];return t[0]=e[0]*r+e[2]*a+e[4],t[1]=e[1]*r+e[3]*a+e[5],t},n.transformMat3=function(t,n,e){var r=n[0],a=n[1];return t[0]=e[0]*r+e[3]*a+e[6],t[1]=e[1]*r+e[4]*a+e[7],t},n.transformMat4=function(t,n,e){let r=n[0],a=n[1];return t[0]=e[0]*r+e[4]*a+e[12],t[1]=e[1]*r+e[5]*a+e[13],t},n.str=function(t){return"vec2("+t[0]+", "+t[1]+")"},n.exactEquals=function(t,n){return t[0]===n[0]&&t[1]===n[1]},n.equals=function(t,n){let e=t[0],a=t[1],o=n[0],i=n[1];return Math.abs(e-o)<=r.b*Math.max(1,Math.abs(e),Math.abs(o))&&Math.abs(a-i)<=r.b*Math.max(1,Math.abs(a),Math.abs(i))};var r=e(0);function a(){let t=new r.a(2);return t[0]=0,t[1]=0,t}function o(t,n,e){return t[0]=n[0]-e[0],t[1]=n[1]-e[1],t}function i(t,n,e){return t[0]=n[0]*e[0],t[1]=n[1]*e[1],t}function u(t,n,e){return t[0]=n[0]/e[0],t[1]=n[1]/e[1],t}function c(t,n){var e=n[0]-t[0],r=n[1]-t[1];return Math.sqrt(e*e+r*r)}function s(t,n){var e=n[0]-t[0],r=n[1]-t[1];return e*e+r*r}function h(t){var n=t[0],e=t[1];return Math.sqrt(n*n+e*e)}function f(t){var n=t[0],e=t[1];return n*n+e*e}const l=h;n.len=l;const M=o;n.sub=M;const b=i;n.mul=b;const m=u;n.div=m;const d=c;n.dist=d;const _=s;n.sqrDist=_;const x=f;n.sqrLen=x;const v=function(){let t=a();return function(n,e,r,a,o,i){let u,c;for(e||(e=2),r||(r=0),c=a?Math.min(a*e+r,n.length):n.length,u=r;u<c;u+=e)t[0]=n[u],t[1]=n[u+1],o(t,t,i),n[u]=t[0],n[u+1]=t[1];return n}}();n.forEach=v},function(t,n,e){"use strict";e.d(n,"a",function(){return o});var r=e(1),a=e(2);class o{constructor(t,n){this.matrix_=r.a.create(),this.position_=t,this.rotate_=n}move(t){r.d.scaleAndAdd(this.position_,this.position_,t,a.c)}rotate(t){let n=r.d.length(t)*a.b,e=r.b.setAxisAngle(r.b.create(),t,n);r.b.multiply(this.rotate_,this.rotate_,e)}getMatrix(){return r.a.fromRotationTranslation(r.a.create(),this.rotate_,this.position_)}}},function(t,n,e){"use strict";e.d(n,"a",function(){return a});var r=e(1);class a{constructor(t,n,e,r,a){this.vertexBuffer_=t,this.indicesBuffer_=n,this.shaderProgram_=e,this.vec3Position_=r,this.quatRotation_=a}addObject(t){t.lastObject_=this,t.nextObject_=this.nextObject_,this.nextObject_=t}deleteObject(){var t=void 0;return void 0!==this.lastObject_&&(this.lastObject_.nextObject_=this.nextObject_,t=this.lastObject_),void 0!==this.nextObject_&&(this.nextObject_.lastObject_=this.lastObject_,t=this.nextObject_),t}getFirstObject(){for(var t=this;void 0!==t.lastObject_;)t=t.lastObject_;return t}getLastObject(){for(var t=this;void 0!==t.nextObject_;)t=t.nextObject_;return t}getMatrix(){return r.a.fromRotationTranslation(r.a.create(),this.quatRotation_,this.vec3Position_)}}},function(t,n,e){"use strict";e.d(n,"c",function(){return r}),e.d(n,"e",function(){return a}),e.d(n,"d",function(){return o}),e.d(n,"a",function(){return i}),e.d(n,"b",function(){return u});var r=[.5,1,0,200],a=.1,o=50,i=[0,0,0],u=[0,0,0]},function(t,n,e){"use strict";n.a=function(t,n){r=n;var e=document.getElementById(t);addEventListener("keydown",c),e.onmousedown=function(t){a=o.c.fromValues(t.pageX,t.pageY),e.onmousemove=u},e.onmouseup=function(t){e.onmousemove=void 0}};var r,a,o=e(1),i=(e(3),e(2));function u(t){let n=o.c.create();document.all?(n[0]=event.x+document.body.scrollLeft,n[1]=event.y+document.body.scrollTop):(n[0]=t.pageX,n[1]=t.pageY);let e=o.c.subtract(o.c.create(),n,a);r.camera_.rotate([e[1],e[0],0]),a=n,r.rend()}function c(t){switch(t.keyCode){case i.a.zoomUp:r.camera_.move(o.d.fromValues(0,0,1));break;case i.a.zoomDown:r.camera_.move(o.d.fromValues(0,0,-1));break;case i.a.left:r.camera_.move(o.d.fromValues(1,0,0));break;case i.a.forward:r.camera_.move(o.d.fromValues(0,-1,0));break;case i.a.right:r.camera_.move(o.d.fromValues(-1,0,0));break;case i.a.back:r.camera_.move(o.d.fromValues(0,1,0))}r.rend()}}]);
//# sourceMappingURL=script.js.map