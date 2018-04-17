!function(t){var n={};function e(r){if(n[r])return n[r].exports;var a=n[r]={i:r,l:!1,exports:{}};return t[r].call(a.exports,a,a.exports,e),a.l=!0,a.exports}e.m=t,e.c=n,e.d=function(t,n,r){e.o(t,n)||Object.defineProperty(t,n,{configurable:!1,enumerable:!0,get:r})},e.n=function(t){var n=t&&t.__esModule?function(){return t.default}:function(){return t};return e.d(n,"a",n),n},e.o=function(t,n){return Object.prototype.hasOwnProperty.call(t,n)},e.p="",e(e.s=7)}([function(t,n,e){"use strict";e.d(n,"a",function(){return a});const r=1e-6;n.b=r;let a="undefined"!=typeof Float32Array?Float32Array:Array;const o=Math.random;n.c=o;Math.PI},function(t,n,e){"use strict";e(0),e(8),e(9),e(4);var r=e(10),a=e(11),o=e(12),u=e(5);e(6);e.d(n,"a",function(){return r}),e.d(n,"b",function(){return a}),e.d(n,"c",function(){return o}),e.d(n,"d",function(){return u})},function(t,n,e){"use strict";e.d(n,"c",function(){return r}),e.d(n,"b",function(){return a}),e.d(n,"a",function(){return o});var r=.5,a=.005,o={zoomUp:33,zoomDown:34,left:37,forward:38,right:39,back:40}},function(t,n,e){"use strict";e.d(n,"a",function(){return c});var r=e(1),a=e(13),o=e(14),u=e(15);class c{constructor(t){let n=document.getElementById(t),e=n.getContext("webgl");this.context_=e,e.viewport(0,0,n.width,n.height),e.getExtension("OES_element_index_uint"),e.clearColor(u.c[0],u.c[1],u.c[2],u.c[3]),e.enable(e.DEPTH_TEST),e.depthFunc(e.LESS),e.clearDepth(1),this.mat4Project_=r.a.create(),r.a.perspective(this.mat4Project_,Math.PI/1.5,1,u.e,u.d);let o=r.b.fromEuler(r.b.create(),u.b[0],u.b[1],u.b[2]);this.camera_=new a.a(u.a,o)}getShaderProgram(t){let n=this.context_,e=n.createProgram();for(var r in t)n.attachShader(e,t[r]);if(n.linkProgram(e),n.getProgramParameter(e,n.LINK_STATUS))return e.vertexPositionAttribute_=n.getAttribLocation(e,"aVec3_vertexPosition"),n.enableVertexAttribArray(e.vertexPositionAttribute_),e.matrix_={mat4Project_:n.getUniformLocation(e,"uMat4_project"),mat4Cam_:n.getUniformLocation(e,"uMat4_cam"),mat4ObjectPos_:n.getUniformLocation(e,"uMat4_objectPos")},e;alert("Can`t init shader program")}getShader(t){let n,e=this.context_,r=document.getElementById(t),a=r.getAttribute("type");if("x-shader/x-vertex"==a)n=e.VERTEX_SHADER;else{if("x-shader/x-fragment"!=a)return void alert("Unknown shader type :"+a);n=e.FRAGMENT_SHADER}let o=r.innerHTML,u=e.createShader(n);return e.shaderSource(u,o),e.compileShader(u),e.getShaderParameter(u,e.COMPILE_STATUS)?u:(alert("Error compile shader: "+e.getShaderInfoLog(u)),void e.deleteShader(u))}loadObject(t,n,e,r,a,u){let c=this.context_,i=c.createBuffer();c.bindBuffer(c.ARRAY_BUFFER,i),c.bufferData(c.ARRAY_BUFFER,new Float32Array(t),c.STATIC_DRAW),i.itemSize_=n,i.countItems_=t.length;let s=c.createBuffer();c.bindBuffer(c.ELEMENT_ARRAY_BUFFER,s),c.bufferData(c.ELEMENT_ARRAY_BUFFER,new Uint16Array(e),c.STATIC_DRAW),s.itemSize_=1,s.countItems_=e.length;new o.a(i,s,r,a,u);return new o.a(i,s,r,a,u)}rend(){let t=this.context_,n=this.camera_.getMatrix(),e=this.mat4Project_;t.clear(t.COLOR_BUFFER_BIT|t.DEPTH_BUFFER_BIT),this.objects_.forEach(function(r){let a=r.shaderProgram_;t.useProgram(a),t.uniformMatrix4fv(a.matrix_.mat4Project_,!1,e),t.uniformMatrix4fv(a.matrix_.mat4Cam_,!1,n),t.uniformMatrix4fv(a.matrix_.mat4ObjectPos_,!1,r.getMatrix()),t.bindBuffer(t.ARRAY_BUFFER,r.vertexBuffer_),t.bindBuffer(t.ELEMENT_ARRAY_BUFFER,r.indicesBuffer_),t.vertexAttribPointer(a.vertexPositionAttribute_,r.vertexBuffer_.itemSize_,t.FLOAT,!1,0,0),t.drawElements(t.TRIANGLES,r.indicesBuffer_.countItems_,t.UNSIGNED_SHORT,0)})}setObjectsList(t){this.objects_=t}}},function(t,n,e){"use strict";n.a=function(){let t=new r.a(9);return t[0]=1,t[1]=0,t[2]=0,t[3]=0,t[4]=1,t[5]=0,t[6]=0,t[7]=0,t[8]=1,t};var r=e(0)},function(t,n,e){"use strict";Object.defineProperty(n,"__esModule",{value:!0}),n.create=a,n.clone=function(t){var n=new r.a(3);return n[0]=t[0],n[1]=t[1],n[2]=t[2],n},n.length=o,n.fromValues=u,n.copy=function(t,n){return t[0]=n[0],t[1]=n[1],t[2]=n[2],t},n.set=function(t,n,e,r){return t[0]=n,t[1]=e,t[2]=r,t},n.add=function(t,n,e){return t[0]=n[0]+e[0],t[1]=n[1]+e[1],t[2]=n[2]+e[2],t},n.subtract=c,n.multiply=i,n.divide=s,n.ceil=function(t,n){return t[0]=Math.ceil(n[0]),t[1]=Math.ceil(n[1]),t[2]=Math.ceil(n[2]),t},n.floor=function(t,n){return t[0]=Math.floor(n[0]),t[1]=Math.floor(n[1]),t[2]=Math.floor(n[2]),t},n.min=function(t,n,e){return t[0]=Math.min(n[0],e[0]),t[1]=Math.min(n[1],e[1]),t[2]=Math.min(n[2],e[2]),t},n.max=function(t,n,e){return t[0]=Math.max(n[0],e[0]),t[1]=Math.max(n[1],e[1]),t[2]=Math.max(n[2],e[2]),t},n.round=function(t,n){return t[0]=Math.round(n[0]),t[1]=Math.round(n[1]),t[2]=Math.round(n[2]),t},n.scale=function(t,n,e){return t[0]=n[0]*e,t[1]=n[1]*e,t[2]=n[2]*e,t},n.scaleAndAdd=function(t,n,e,r){return t[0]=n[0]+e[0]*r,t[1]=n[1]+e[1]*r,t[2]=n[2]+e[2]*r,t},n.distance=f,n.squaredDistance=l,n.squaredLength=h,n.negate=function(t,n){return t[0]=-n[0],t[1]=-n[1],t[2]=-n[2],t},n.inverse=function(t,n){return t[0]=1/n[0],t[1]=1/n[1],t[2]=1/n[2],t},n.normalize=M,n.dot=m,n.cross=function(t,n,e){let r=n[0],a=n[1],o=n[2],u=e[0],c=e[1],i=e[2];return t[0]=a*i-o*c,t[1]=o*u-r*i,t[2]=r*c-a*u,t},n.lerp=function(t,n,e,r){let a=n[0],o=n[1],u=n[2];return t[0]=a+r*(e[0]-a),t[1]=o+r*(e[1]-o),t[2]=u+r*(e[2]-u),t},n.hermite=function(t,n,e,r,a,o){let u=o*o,c=u*(2*o-3)+1,i=u*(o-2)+o,s=u*(o-1),f=u*(3-2*o);return t[0]=n[0]*c+e[0]*i+r[0]*s+a[0]*f,t[1]=n[1]*c+e[1]*i+r[1]*s+a[1]*f,t[2]=n[2]*c+e[2]*i+r[2]*s+a[2]*f,t},n.bezier=function(t,n,e,r,a,o){let u=1-o,c=u*u,i=o*o,s=c*u,f=3*o*c,l=3*i*u,h=i*o;return t[0]=n[0]*s+e[0]*f+r[0]*l+a[0]*h,t[1]=n[1]*s+e[1]*f+r[1]*l+a[1]*h,t[2]=n[2]*s+e[2]*f+r[2]*l+a[2]*h,t},n.random=function(t,n){n=n||1;let e=2*r.c()*Math.PI,a=2*r.c()-1,o=Math.sqrt(1-a*a)*n;return t[0]=Math.cos(e)*o,t[1]=Math.sin(e)*o,t[2]=a*n,t},n.transformMat4=function(t,n,e){let r=n[0],a=n[1],o=n[2],u=e[3]*r+e[7]*a+e[11]*o+e[15];return u=u||1,t[0]=(e[0]*r+e[4]*a+e[8]*o+e[12])/u,t[1]=(e[1]*r+e[5]*a+e[9]*o+e[13])/u,t[2]=(e[2]*r+e[6]*a+e[10]*o+e[14])/u,t},n.transformMat3=function(t,n,e){let r=n[0],a=n[1],o=n[2];return t[0]=r*e[0]+a*e[3]+o*e[6],t[1]=r*e[1]+a*e[4]+o*e[7],t[2]=r*e[2]+a*e[5]+o*e[8],t},n.transformQuat=function(t,n,e){let r=n[0],a=n[1],o=n[2],u=e[0],c=e[1],i=e[2],s=e[3],f=s*r+c*o-i*a,l=s*a+i*r-u*o,h=s*o+u*a-c*r,M=-u*r-c*a-i*o;return t[0]=f*s+M*-u+l*-i-h*-c,t[1]=l*s+M*-c+h*-u-f*-i,t[2]=h*s+M*-i+f*-c-l*-u,t},n.rotateX=function(t,n,e,r){let a=[],o=[];return a[0]=n[0]-e[0],a[1]=n[1]-e[1],a[2]=n[2]-e[2],o[0]=a[0],o[1]=a[1]*Math.cos(r)-a[2]*Math.sin(r),o[2]=a[1]*Math.sin(r)+a[2]*Math.cos(r),t[0]=o[0]+e[0],t[1]=o[1]+e[1],t[2]=o[2]+e[2],t},n.rotateY=function(t,n,e,r){let a=[],o=[];return a[0]=n[0]-e[0],a[1]=n[1]-e[1],a[2]=n[2]-e[2],o[0]=a[2]*Math.sin(r)+a[0]*Math.cos(r),o[1]=a[1],o[2]=a[2]*Math.cos(r)-a[0]*Math.sin(r),t[0]=o[0]+e[0],t[1]=o[1]+e[1],t[2]=o[2]+e[2],t},n.rotateZ=function(t,n,e,r){let a=[],o=[];return a[0]=n[0]-e[0],a[1]=n[1]-e[1],a[2]=n[2]-e[2],o[0]=a[0]*Math.cos(r)-a[1]*Math.sin(r),o[1]=a[0]*Math.sin(r)+a[1]*Math.cos(r),o[2]=a[2],t[0]=o[0]+e[0],t[1]=o[1]+e[1],t[2]=o[2]+e[2],t},n.angle=function(t,n){let e=u(t[0],t[1],t[2]),r=u(n[0],n[1],n[2]);M(e,e),M(r,r);let a=m(e,r);return a>1?0:a<-1?Math.PI:Math.acos(a)},n.str=function(t){return"vec3("+t[0]+", "+t[1]+", "+t[2]+")"},n.exactEquals=function(t,n){return t[0]===n[0]&&t[1]===n[1]&&t[2]===n[2]},n.equals=function(t,n){let e=t[0],a=t[1],o=t[2],u=n[0],c=n[1],i=n[2];return Math.abs(e-u)<=r.b*Math.max(1,Math.abs(e),Math.abs(u))&&Math.abs(a-c)<=r.b*Math.max(1,Math.abs(a),Math.abs(c))&&Math.abs(o-i)<=r.b*Math.max(1,Math.abs(o),Math.abs(i))};var r=e(0);function a(){let t=new r.a(3);return t[0]=0,t[1]=0,t[2]=0,t}function o(t){let n=t[0],e=t[1],r=t[2];return Math.sqrt(n*n+e*e+r*r)}function u(t,n,e){let a=new r.a(3);return a[0]=t,a[1]=n,a[2]=e,a}function c(t,n,e){return t[0]=n[0]-e[0],t[1]=n[1]-e[1],t[2]=n[2]-e[2],t}function i(t,n,e){return t[0]=n[0]*e[0],t[1]=n[1]*e[1],t[2]=n[2]*e[2],t}function s(t,n,e){return t[0]=n[0]/e[0],t[1]=n[1]/e[1],t[2]=n[2]/e[2],t}function f(t,n){let e=n[0]-t[0],r=n[1]-t[1],a=n[2]-t[2];return Math.sqrt(e*e+r*r+a*a)}function l(t,n){let e=n[0]-t[0],r=n[1]-t[1],a=n[2]-t[2];return e*e+r*r+a*a}function h(t){let n=t[0],e=t[1],r=t[2];return n*n+e*e+r*r}function M(t,n){let e=n[0],r=n[1],a=n[2],o=e*e+r*r+a*a;return o>0&&(o=1/Math.sqrt(o),t[0]=n[0]*o,t[1]=n[1]*o,t[2]=n[2]*o),t}function m(t,n){return t[0]*n[0]+t[1]*n[1]+t[2]*n[2]}const d=c;n.sub=d;const b=i;n.mul=b;const p=s;n.div=p;const _=f;n.dist=_;const v=l;n.sqrDist=v;const g=o;n.len=g;const x=h;n.sqrLen=x;const w=function(){let t=a();return function(n,e,r,a,o,u){let c,i;for(e||(e=3),r||(r=0),i=a?Math.min(a*e+r,n.length):n.length,c=r;c<i;c+=e)t[0]=n[c],t[1]=n[c+1],t[2]=n[c+2],o(t,t,u),n[c]=t[0],n[c+1]=t[1],n[c+2]=t[2];return n}}();n.forEach=w},function(t,n,e){"use strict";n.b=function(t){let n=new r.a(4);return n[0]=t[0],n[1]=t[1],n[2]=t[2],n[3]=t[3],n},n.g=function(t,n,e,a){let o=new r.a(4);return o[0]=t,o[1]=n,o[2]=e,o[3]=a,o},n.c=function(t,n){return t[0]=n[0],t[1]=n[1],t[2]=n[2],t[3]=n[3],t},n.l=function(t,n,e,r,a){return t[0]=n,t[1]=e,t[2]=r,t[3]=a,t},n.a=function(t,n,e){return t[0]=n[0]+e[0],t[1]=n[1]+e[1],t[2]=n[2]+e[2],t[3]=n[3]+e[3],t},n.k=a,n.h=o,n.m=u,n.j=c,n.d=function(t,n){return t[0]*n[0]+t[1]*n[1]+t[2]*n[2]+t[3]*n[3]},n.i=function(t,n,e,r){let a=n[0],o=n[1],u=n[2],c=n[3];return t[0]=a+r*(e[0]-a),t[1]=o+r*(e[1]-o),t[2]=u+r*(e[2]-u),t[3]=c+r*(e[3]-c),t},n.f=function(t,n){return t[0]===n[0]&&t[1]===n[1]&&t[2]===n[2]&&t[3]===n[3]},n.e=function(t,n){let e=t[0],a=t[1],o=t[2],u=t[3],c=n[0],i=n[1],s=n[2],f=n[3];return Math.abs(e-c)<=r.b*Math.max(1,Math.abs(e),Math.abs(c))&&Math.abs(a-i)<=r.b*Math.max(1,Math.abs(a),Math.abs(i))&&Math.abs(o-s)<=r.b*Math.max(1,Math.abs(o),Math.abs(s))&&Math.abs(u-f)<=r.b*Math.max(1,Math.abs(u),Math.abs(f))};var r=e(0);function a(t,n,e){return t[0]=n[0]*e,t[1]=n[1]*e,t[2]=n[2]*e,t[3]=n[3]*e,t}function o(t){let n=t[0],e=t[1],r=t[2],a=t[3];return Math.sqrt(n*n+e*e+r*r+a*a)}function u(t){let n=t[0],e=t[1],r=t[2],a=t[3];return n*n+e*e+r*r+a*a}function c(t,n){let e=n[0],r=n[1],a=n[2],o=n[3],u=e*e+r*r+a*a+o*o;return u>0&&(u=1/Math.sqrt(u),t[0]=e*u,t[1]=r*u,t[2]=a*u,t[3]=o*u),t}!function(){let t=function(){let t=new r.a(4);return t[0]=0,t[1]=0,t[2]=0,t[3]=0,t}()}()},function(t,n,e){"use strict";Object.defineProperty(n,"__esModule",{value:!0});var r=e(3),a=(e(2),e(16)),o=e(17),u=e(18),c=e(1),i=new r.a("viewport"),s=new Array;i.setObjectsList(s),i.rend(),a.a("viewport",i,s),document.getElementById("test-net-button").onclick=function(){o.a("hello")},document.getElementById("start-button").onclick=function(){var t=new FileReader,n=document.getElementById("file-path").files[0];t.onload=function(t){var n=u.a("ply",t.target.result),e=[i.getShader("vertex-shader"),i.getShader("fragment-shader")],r=i.getShaderProgram(e);let a=i.loadObject(n.vertex,3,n.face,r,c.d.fromValues(0,0,-3),c.b.create());s.push(a),i.rend()},t.onerror=function(t){alert("Error")},t.readAsArrayBuffer(n)}},function(t,n,e){"use strict";e(0)},function(t,n,e){"use strict";e(0)},function(t,n,e){"use strict";Object.defineProperty(n,"__esModule",{value:!0}),n.create=function(){let t=new r.a(16);return t[0]=1,t[1]=0,t[2]=0,t[3]=0,t[4]=0,t[5]=1,t[6]=0,t[7]=0,t[8]=0,t[9]=0,t[10]=1,t[11]=0,t[12]=0,t[13]=0,t[14]=0,t[15]=1,t},n.clone=function(t){let n=new r.a(16);return n[0]=t[0],n[1]=t[1],n[2]=t[2],n[3]=t[3],n[4]=t[4],n[5]=t[5],n[6]=t[6],n[7]=t[7],n[8]=t[8],n[9]=t[9],n[10]=t[10],n[11]=t[11],n[12]=t[12],n[13]=t[13],n[14]=t[14],n[15]=t[15],n},n.copy=function(t,n){return t[0]=n[0],t[1]=n[1],t[2]=n[2],t[3]=n[3],t[4]=n[4],t[5]=n[5],t[6]=n[6],t[7]=n[7],t[8]=n[8],t[9]=n[9],t[10]=n[10],t[11]=n[11],t[12]=n[12],t[13]=n[13],t[14]=n[14],t[15]=n[15],t},n.fromValues=function(t,n,e,a,o,u,c,i,s,f,l,h,M,m,d,b){let p=new r.a(16);return p[0]=t,p[1]=n,p[2]=e,p[3]=a,p[4]=o,p[5]=u,p[6]=c,p[7]=i,p[8]=s,p[9]=f,p[10]=l,p[11]=h,p[12]=M,p[13]=m,p[14]=d,p[15]=b,p},n.set=function(t,n,e,r,a,o,u,c,i,s,f,l,h,M,m,d,b){return t[0]=n,t[1]=e,t[2]=r,t[3]=a,t[4]=o,t[5]=u,t[6]=c,t[7]=i,t[8]=s,t[9]=f,t[10]=l,t[11]=h,t[12]=M,t[13]=m,t[14]=d,t[15]=b,t},n.identity=a,n.transpose=function(t,n){if(t===n){let e=n[1],r=n[2],a=n[3],o=n[6],u=n[7],c=n[11];t[1]=n[4],t[2]=n[8],t[3]=n[12],t[4]=e,t[6]=n[9],t[7]=n[13],t[8]=r,t[9]=o,t[11]=n[14],t[12]=a,t[13]=u,t[14]=c}else t[0]=n[0],t[1]=n[4],t[2]=n[8],t[3]=n[12],t[4]=n[1],t[5]=n[5],t[6]=n[9],t[7]=n[13],t[8]=n[2],t[9]=n[6],t[10]=n[10],t[11]=n[14],t[12]=n[3],t[13]=n[7],t[14]=n[11],t[15]=n[15];return t},n.invert=function(t,n){let e=n[0],r=n[1],a=n[2],o=n[3],u=n[4],c=n[5],i=n[6],s=n[7],f=n[8],l=n[9],h=n[10],M=n[11],m=n[12],d=n[13],b=n[14],p=n[15],_=e*c-r*u,v=e*i-a*u,g=e*s-o*u,x=r*i-a*c,w=r*s-o*c,A=a*s-o*i,E=f*d-l*m,y=f*b-h*m,q=f*p-M*m,P=l*b-h*d,R=l*p-M*d,T=h*p-M*b,S=_*T-v*R+g*P+x*q-w*y+A*E;if(!S)return null;return S=1/S,t[0]=(c*T-i*R+s*P)*S,t[1]=(a*R-r*T-o*P)*S,t[2]=(d*A-b*w+p*x)*S,t[3]=(h*w-l*A-M*x)*S,t[4]=(i*q-u*T-s*y)*S,t[5]=(e*T-a*q+o*y)*S,t[6]=(b*g-m*A-p*v)*S,t[7]=(f*A-h*g+M*v)*S,t[8]=(u*R-c*q+s*E)*S,t[9]=(r*q-e*R-o*E)*S,t[10]=(m*w-d*g+p*_)*S,t[11]=(l*g-f*w-M*_)*S,t[12]=(c*y-u*P-i*E)*S,t[13]=(e*P-r*y+a*E)*S,t[14]=(d*v-m*x-b*_)*S,t[15]=(f*x-l*v+h*_)*S,t},n.adjoint=function(t,n){let e=n[0],r=n[1],a=n[2],o=n[3],u=n[4],c=n[5],i=n[6],s=n[7],f=n[8],l=n[9],h=n[10],M=n[11],m=n[12],d=n[13],b=n[14],p=n[15];return t[0]=c*(h*p-M*b)-l*(i*p-s*b)+d*(i*M-s*h),t[1]=-(r*(h*p-M*b)-l*(a*p-o*b)+d*(a*M-o*h)),t[2]=r*(i*p-s*b)-c*(a*p-o*b)+d*(a*s-o*i),t[3]=-(r*(i*M-s*h)-c*(a*M-o*h)+l*(a*s-o*i)),t[4]=-(u*(h*p-M*b)-f*(i*p-s*b)+m*(i*M-s*h)),t[5]=e*(h*p-M*b)-f*(a*p-o*b)+m*(a*M-o*h),t[6]=-(e*(i*p-s*b)-u*(a*p-o*b)+m*(a*s-o*i)),t[7]=e*(i*M-s*h)-u*(a*M-o*h)+f*(a*s-o*i),t[8]=u*(l*p-M*d)-f*(c*p-s*d)+m*(c*M-s*l),t[9]=-(e*(l*p-M*d)-f*(r*p-o*d)+m*(r*M-o*l)),t[10]=e*(c*p-s*d)-u*(r*p-o*d)+m*(r*s-o*c),t[11]=-(e*(c*M-s*l)-u*(r*M-o*l)+f*(r*s-o*c)),t[12]=-(u*(l*b-h*d)-f*(c*b-i*d)+m*(c*h-i*l)),t[13]=e*(l*b-h*d)-f*(r*b-a*d)+m*(r*h-a*l),t[14]=-(e*(c*b-i*d)-u*(r*b-a*d)+m*(r*i-a*c)),t[15]=e*(c*h-i*l)-u*(r*h-a*l)+f*(r*i-a*c),t},n.determinant=function(t){let n=t[0],e=t[1],r=t[2],a=t[3],o=t[4],u=t[5],c=t[6],i=t[7],s=t[8],f=t[9],l=t[10],h=t[11],M=t[12],m=t[13],d=t[14],b=t[15];return(n*u-e*o)*(l*b-h*d)-(n*c-r*o)*(f*b-h*m)+(n*i-a*o)*(f*d-l*m)+(e*c-r*u)*(s*b-h*M)-(e*i-a*u)*(s*d-l*M)+(r*i-a*c)*(s*m-f*M)},n.multiply=o,n.translate=function(t,n,e){let r,a,o,u,c,i,s,f,l,h,M,m,d=e[0],b=e[1],p=e[2];n===t?(t[12]=n[0]*d+n[4]*b+n[8]*p+n[12],t[13]=n[1]*d+n[5]*b+n[9]*p+n[13],t[14]=n[2]*d+n[6]*b+n[10]*p+n[14],t[15]=n[3]*d+n[7]*b+n[11]*p+n[15]):(r=n[0],a=n[1],o=n[2],u=n[3],c=n[4],i=n[5],s=n[6],f=n[7],l=n[8],h=n[9],M=n[10],m=n[11],t[0]=r,t[1]=a,t[2]=o,t[3]=u,t[4]=c,t[5]=i,t[6]=s,t[7]=f,t[8]=l,t[9]=h,t[10]=M,t[11]=m,t[12]=r*d+c*b+l*p+n[12],t[13]=a*d+i*b+h*p+n[13],t[14]=o*d+s*b+M*p+n[14],t[15]=u*d+f*b+m*p+n[15]);return t},n.scale=function(t,n,e){let r=e[0],a=e[1],o=e[2];return t[0]=n[0]*r,t[1]=n[1]*r,t[2]=n[2]*r,t[3]=n[3]*r,t[4]=n[4]*a,t[5]=n[5]*a,t[6]=n[6]*a,t[7]=n[7]*a,t[8]=n[8]*o,t[9]=n[9]*o,t[10]=n[10]*o,t[11]=n[11]*o,t[12]=n[12],t[13]=n[13],t[14]=n[14],t[15]=n[15],t},n.rotate=function(t,n,e,a){let o,u,c,i,s,f,l,h,M,m,d,b,p,_,v,g,x,w,A,E,y,q,P,R,T=a[0],S=a[1],I=a[2],j=Math.sqrt(T*T+S*S+I*I);if(Math.abs(j)<r.b)return null;T*=j=1/j,S*=j,I*=j,o=Math.sin(e),u=Math.cos(e),c=1-u,i=n[0],s=n[1],f=n[2],l=n[3],h=n[4],M=n[5],m=n[6],d=n[7],b=n[8],p=n[9],_=n[10],v=n[11],g=T*T*c+u,x=S*T*c+I*o,w=I*T*c-S*o,A=T*S*c-I*o,E=S*S*c+u,y=I*S*c+T*o,q=T*I*c+S*o,P=S*I*c-T*o,R=I*I*c+u,t[0]=i*g+h*x+b*w,t[1]=s*g+M*x+p*w,t[2]=f*g+m*x+_*w,t[3]=l*g+d*x+v*w,t[4]=i*A+h*E+b*y,t[5]=s*A+M*E+p*y,t[6]=f*A+m*E+_*y,t[7]=l*A+d*E+v*y,t[8]=i*q+h*P+b*R,t[9]=s*q+M*P+p*R,t[10]=f*q+m*P+_*R,t[11]=l*q+d*P+v*R,n!==t&&(t[12]=n[12],t[13]=n[13],t[14]=n[14],t[15]=n[15]);return t},n.rotateX=function(t,n,e){let r=Math.sin(e),a=Math.cos(e),o=n[4],u=n[5],c=n[6],i=n[7],s=n[8],f=n[9],l=n[10],h=n[11];n!==t&&(t[0]=n[0],t[1]=n[1],t[2]=n[2],t[3]=n[3],t[12]=n[12],t[13]=n[13],t[14]=n[14],t[15]=n[15]);return t[4]=o*a+s*r,t[5]=u*a+f*r,t[6]=c*a+l*r,t[7]=i*a+h*r,t[8]=s*a-o*r,t[9]=f*a-u*r,t[10]=l*a-c*r,t[11]=h*a-i*r,t},n.rotateY=function(t,n,e){let r=Math.sin(e),a=Math.cos(e),o=n[0],u=n[1],c=n[2],i=n[3],s=n[8],f=n[9],l=n[10],h=n[11];n!==t&&(t[4]=n[4],t[5]=n[5],t[6]=n[6],t[7]=n[7],t[12]=n[12],t[13]=n[13],t[14]=n[14],t[15]=n[15]);return t[0]=o*a-s*r,t[1]=u*a-f*r,t[2]=c*a-l*r,t[3]=i*a-h*r,t[8]=o*r+s*a,t[9]=u*r+f*a,t[10]=c*r+l*a,t[11]=i*r+h*a,t},n.rotateZ=function(t,n,e){let r=Math.sin(e),a=Math.cos(e),o=n[0],u=n[1],c=n[2],i=n[3],s=n[4],f=n[5],l=n[6],h=n[7];n!==t&&(t[8]=n[8],t[9]=n[9],t[10]=n[10],t[11]=n[11],t[12]=n[12],t[13]=n[13],t[14]=n[14],t[15]=n[15]);return t[0]=o*a+s*r,t[1]=u*a+f*r,t[2]=c*a+l*r,t[3]=i*a+h*r,t[4]=s*a-o*r,t[5]=f*a-u*r,t[6]=l*a-c*r,t[7]=h*a-i*r,t},n.fromTranslation=function(t,n){return t[0]=1,t[1]=0,t[2]=0,t[3]=0,t[4]=0,t[5]=1,t[6]=0,t[7]=0,t[8]=0,t[9]=0,t[10]=1,t[11]=0,t[12]=n[0],t[13]=n[1],t[14]=n[2],t[15]=1,t},n.fromScaling=function(t,n){return t[0]=n[0],t[1]=0,t[2]=0,t[3]=0,t[4]=0,t[5]=n[1],t[6]=0,t[7]=0,t[8]=0,t[9]=0,t[10]=n[2],t[11]=0,t[12]=0,t[13]=0,t[14]=0,t[15]=1,t},n.fromRotation=function(t,n,e){let a,o,u,c=e[0],i=e[1],s=e[2],f=Math.sqrt(c*c+i*i+s*s);if(Math.abs(f)<r.b)return null;return c*=f=1/f,i*=f,s*=f,a=Math.sin(n),o=Math.cos(n),u=1-o,t[0]=c*c*u+o,t[1]=i*c*u+s*a,t[2]=s*c*u-i*a,t[3]=0,t[4]=c*i*u-s*a,t[5]=i*i*u+o,t[6]=s*i*u+c*a,t[7]=0,t[8]=c*s*u+i*a,t[9]=i*s*u-c*a,t[10]=s*s*u+o,t[11]=0,t[12]=0,t[13]=0,t[14]=0,t[15]=1,t},n.fromXRotation=function(t,n){let e=Math.sin(n),r=Math.cos(n);return t[0]=1,t[1]=0,t[2]=0,t[3]=0,t[4]=0,t[5]=r,t[6]=e,t[7]=0,t[8]=0,t[9]=-e,t[10]=r,t[11]=0,t[12]=0,t[13]=0,t[14]=0,t[15]=1,t},n.fromYRotation=function(t,n){let e=Math.sin(n),r=Math.cos(n);return t[0]=r,t[1]=0,t[2]=-e,t[3]=0,t[4]=0,t[5]=1,t[6]=0,t[7]=0,t[8]=e,t[9]=0,t[10]=r,t[11]=0,t[12]=0,t[13]=0,t[14]=0,t[15]=1,t},n.fromZRotation=function(t,n){let e=Math.sin(n),r=Math.cos(n);return t[0]=r,t[1]=e,t[2]=0,t[3]=0,t[4]=-e,t[5]=r,t[6]=0,t[7]=0,t[8]=0,t[9]=0,t[10]=1,t[11]=0,t[12]=0,t[13]=0,t[14]=0,t[15]=1,t},n.fromRotationTranslation=function(t,n,e){let r=n[0],a=n[1],o=n[2],u=n[3],c=r+r,i=a+a,s=o+o,f=r*c,l=r*i,h=r*s,M=a*i,m=a*s,d=o*s,b=u*c,p=u*i,_=u*s;return t[0]=1-(M+d),t[1]=l+_,t[2]=h-p,t[3]=0,t[4]=l-_,t[5]=1-(f+d),t[6]=m+b,t[7]=0,t[8]=h+p,t[9]=m-b,t[10]=1-(f+M),t[11]=0,t[12]=e[0],t[13]=e[1],t[14]=e[2],t[15]=1,t},n.getTranslation=function(t,n){return t[0]=n[12],t[1]=n[13],t[2]=n[14],t},n.getScaling=function(t,n){let e=n[0],r=n[1],a=n[2],o=n[4],u=n[5],c=n[6],i=n[8],s=n[9],f=n[10];return t[0]=Math.sqrt(e*e+r*r+a*a),t[1]=Math.sqrt(o*o+u*u+c*c),t[2]=Math.sqrt(i*i+s*s+f*f),t},n.getRotation=function(t,n){let e=n[0]+n[5]+n[10],r=0;e>0?(r=2*Math.sqrt(e+1),t[3]=.25*r,t[0]=(n[6]-n[9])/r,t[1]=(n[8]-n[2])/r,t[2]=(n[1]-n[4])/r):n[0]>n[5]&n[0]>n[10]?(r=2*Math.sqrt(1+n[0]-n[5]-n[10]),t[3]=(n[6]-n[9])/r,t[0]=.25*r,t[1]=(n[1]+n[4])/r,t[2]=(n[8]+n[2])/r):n[5]>n[10]?(r=2*Math.sqrt(1+n[5]-n[0]-n[10]),t[3]=(n[8]-n[2])/r,t[0]=(n[1]+n[4])/r,t[1]=.25*r,t[2]=(n[6]+n[9])/r):(r=2*Math.sqrt(1+n[10]-n[0]-n[5]),t[3]=(n[1]-n[4])/r,t[0]=(n[8]+n[2])/r,t[1]=(n[6]+n[9])/r,t[2]=.25*r);return t},n.fromRotationTranslationScale=function(t,n,e,r){let a=n[0],o=n[1],u=n[2],c=n[3],i=a+a,s=o+o,f=u+u,l=a*i,h=a*s,M=a*f,m=o*s,d=o*f,b=u*f,p=c*i,_=c*s,v=c*f,g=r[0],x=r[1],w=r[2];return t[0]=(1-(m+b))*g,t[1]=(h+v)*g,t[2]=(M-_)*g,t[3]=0,t[4]=(h-v)*x,t[5]=(1-(l+b))*x,t[6]=(d+p)*x,t[7]=0,t[8]=(M+_)*w,t[9]=(d-p)*w,t[10]=(1-(l+m))*w,t[11]=0,t[12]=e[0],t[13]=e[1],t[14]=e[2],t[15]=1,t},n.fromRotationTranslationScaleOrigin=function(t,n,e,r,a){let o=n[0],u=n[1],c=n[2],i=n[3],s=o+o,f=u+u,l=c+c,h=o*s,M=o*f,m=o*l,d=u*f,b=u*l,p=c*l,_=i*s,v=i*f,g=i*l,x=r[0],w=r[1],A=r[2],E=a[0],y=a[1],q=a[2];return t[0]=(1-(d+p))*x,t[1]=(M+g)*x,t[2]=(m-v)*x,t[3]=0,t[4]=(M-g)*w,t[5]=(1-(h+p))*w,t[6]=(b+_)*w,t[7]=0,t[8]=(m+v)*A,t[9]=(b-_)*A,t[10]=(1-(h+d))*A,t[11]=0,t[12]=e[0]+E-(t[0]*E+t[4]*y+t[8]*q),t[13]=e[1]+y-(t[1]*E+t[5]*y+t[9]*q),t[14]=e[2]+q-(t[2]*E+t[6]*y+t[10]*q),t[15]=1,t},n.fromQuat=function(t,n){let e=n[0],r=n[1],a=n[2],o=n[3],u=e+e,c=r+r,i=a+a,s=e*u,f=r*u,l=r*c,h=a*u,M=a*c,m=a*i,d=o*u,b=o*c,p=o*i;return t[0]=1-l-m,t[1]=f+p,t[2]=h-b,t[3]=0,t[4]=f-p,t[5]=1-s-m,t[6]=M+d,t[7]=0,t[8]=h+b,t[9]=M-d,t[10]=1-s-l,t[11]=0,t[12]=0,t[13]=0,t[14]=0,t[15]=1,t},n.frustum=function(t,n,e,r,a,o,u){let c=1/(e-n),i=1/(a-r),s=1/(o-u);return t[0]=2*o*c,t[1]=0,t[2]=0,t[3]=0,t[4]=0,t[5]=2*o*i,t[6]=0,t[7]=0,t[8]=(e+n)*c,t[9]=(a+r)*i,t[10]=(u+o)*s,t[11]=-1,t[12]=0,t[13]=0,t[14]=u*o*2*s,t[15]=0,t},n.perspective=function(t,n,e,r,a){let o=1/Math.tan(n/2),u=1/(r-a);return t[0]=o/e,t[1]=0,t[2]=0,t[3]=0,t[4]=0,t[5]=o,t[6]=0,t[7]=0,t[8]=0,t[9]=0,t[10]=(a+r)*u,t[11]=-1,t[12]=0,t[13]=0,t[14]=2*a*r*u,t[15]=0,t},n.perspectiveFromFieldOfView=function(t,n,e,r){let a=Math.tan(n.upDegrees*Math.PI/180),o=Math.tan(n.downDegrees*Math.PI/180),u=Math.tan(n.leftDegrees*Math.PI/180),c=Math.tan(n.rightDegrees*Math.PI/180),i=2/(u+c),s=2/(a+o);return t[0]=i,t[1]=0,t[2]=0,t[3]=0,t[4]=0,t[5]=s,t[6]=0,t[7]=0,t[8]=-(u-c)*i*.5,t[9]=(a-o)*s*.5,t[10]=r/(e-r),t[11]=-1,t[12]=0,t[13]=0,t[14]=r*e/(e-r),t[15]=0,t},n.ortho=function(t,n,e,r,a,o,u){let c=1/(n-e),i=1/(r-a),s=1/(o-u);return t[0]=-2*c,t[1]=0,t[2]=0,t[3]=0,t[4]=0,t[5]=-2*i,t[6]=0,t[7]=0,t[8]=0,t[9]=0,t[10]=2*s,t[11]=0,t[12]=(n+e)*c,t[13]=(a+r)*i,t[14]=(u+o)*s,t[15]=1,t},n.lookAt=function(t,n,e,o){let u,c,i,s,f,l,h,M,m,d,b=n[0],p=n[1],_=n[2],v=o[0],g=o[1],x=o[2],w=e[0],A=e[1],E=e[2];if(Math.abs(b-w)<r.b&&Math.abs(p-A)<r.b&&Math.abs(_-E)<r.b)return a(t);h=b-w,M=p-A,m=_-E,d=1/Math.sqrt(h*h+M*M+m*m),u=g*(m*=d)-x*(M*=d),c=x*(h*=d)-v*m,i=v*M-g*h,(d=Math.sqrt(u*u+c*c+i*i))?(u*=d=1/d,c*=d,i*=d):(u=0,c=0,i=0);s=M*i-m*c,f=m*u-h*i,l=h*c-M*u,(d=Math.sqrt(s*s+f*f+l*l))?(s*=d=1/d,f*=d,l*=d):(s=0,f=0,l=0);return t[0]=u,t[1]=s,t[2]=h,t[3]=0,t[4]=c,t[5]=f,t[6]=M,t[7]=0,t[8]=i,t[9]=l,t[10]=m,t[11]=0,t[12]=-(u*b+c*p+i*_),t[13]=-(s*b+f*p+l*_),t[14]=-(h*b+M*p+m*_),t[15]=1,t},n.targetTo=function(t,n,e,r){let a=n[0],o=n[1],u=n[2],c=r[0],i=r[1],s=r[2],f=a-e[0],l=o-e[1],h=u-e[2],M=f*f+l*l+h*h;M>0&&(M=1/Math.sqrt(M),f*=M,l*=M,h*=M);let m=i*h-s*l,d=s*f-c*h,b=c*l-i*f;return t[0]=m,t[1]=d,t[2]=b,t[3]=0,t[4]=l*b-h*d,t[5]=h*m-f*b,t[6]=f*d-l*m,t[7]=0,t[8]=f,t[9]=l,t[10]=h,t[11]=0,t[12]=a,t[13]=o,t[14]=u,t[15]=1,t},n.str=function(t){return"mat4("+t[0]+", "+t[1]+", "+t[2]+", "+t[3]+", "+t[4]+", "+t[5]+", "+t[6]+", "+t[7]+", "+t[8]+", "+t[9]+", "+t[10]+", "+t[11]+", "+t[12]+", "+t[13]+", "+t[14]+", "+t[15]+")"},n.frob=function(t){return Math.sqrt(Math.pow(t[0],2)+Math.pow(t[1],2)+Math.pow(t[2],2)+Math.pow(t[3],2)+Math.pow(t[4],2)+Math.pow(t[5],2)+Math.pow(t[6],2)+Math.pow(t[7],2)+Math.pow(t[8],2)+Math.pow(t[9],2)+Math.pow(t[10],2)+Math.pow(t[11],2)+Math.pow(t[12],2)+Math.pow(t[13],2)+Math.pow(t[14],2)+Math.pow(t[15],2))},n.add=function(t,n,e){return t[0]=n[0]+e[0],t[1]=n[1]+e[1],t[2]=n[2]+e[2],t[3]=n[3]+e[3],t[4]=n[4]+e[4],t[5]=n[5]+e[5],t[6]=n[6]+e[6],t[7]=n[7]+e[7],t[8]=n[8]+e[8],t[9]=n[9]+e[9],t[10]=n[10]+e[10],t[11]=n[11]+e[11],t[12]=n[12]+e[12],t[13]=n[13]+e[13],t[14]=n[14]+e[14],t[15]=n[15]+e[15],t},n.subtract=u,n.multiplyScalar=function(t,n,e){return t[0]=n[0]*e,t[1]=n[1]*e,t[2]=n[2]*e,t[3]=n[3]*e,t[4]=n[4]*e,t[5]=n[5]*e,t[6]=n[6]*e,t[7]=n[7]*e,t[8]=n[8]*e,t[9]=n[9]*e,t[10]=n[10]*e,t[11]=n[11]*e,t[12]=n[12]*e,t[13]=n[13]*e,t[14]=n[14]*e,t[15]=n[15]*e,t},n.multiplyScalarAndAdd=function(t,n,e,r){return t[0]=n[0]+e[0]*r,t[1]=n[1]+e[1]*r,t[2]=n[2]+e[2]*r,t[3]=n[3]+e[3]*r,t[4]=n[4]+e[4]*r,t[5]=n[5]+e[5]*r,t[6]=n[6]+e[6]*r,t[7]=n[7]+e[7]*r,t[8]=n[8]+e[8]*r,t[9]=n[9]+e[9]*r,t[10]=n[10]+e[10]*r,t[11]=n[11]+e[11]*r,t[12]=n[12]+e[12]*r,t[13]=n[13]+e[13]*r,t[14]=n[14]+e[14]*r,t[15]=n[15]+e[15]*r,t},n.exactEquals=function(t,n){return t[0]===n[0]&&t[1]===n[1]&&t[2]===n[2]&&t[3]===n[3]&&t[4]===n[4]&&t[5]===n[5]&&t[6]===n[6]&&t[7]===n[7]&&t[8]===n[8]&&t[9]===n[9]&&t[10]===n[10]&&t[11]===n[11]&&t[12]===n[12]&&t[13]===n[13]&&t[14]===n[14]&&t[15]===n[15]},n.equals=function(t,n){let e=t[0],a=t[1],o=t[2],u=t[3],c=t[4],i=t[5],s=t[6],f=t[7],l=t[8],h=t[9],M=t[10],m=t[11],d=t[12],b=t[13],p=t[14],_=t[15],v=n[0],g=n[1],x=n[2],w=n[3],A=n[4],E=n[5],y=n[6],q=n[7],P=n[8],R=n[9],T=n[10],S=n[11],I=n[12],j=n[13],B=n[14],F=n[15];return Math.abs(e-v)<=r.b*Math.max(1,Math.abs(e),Math.abs(v))&&Math.abs(a-g)<=r.b*Math.max(1,Math.abs(a),Math.abs(g))&&Math.abs(o-x)<=r.b*Math.max(1,Math.abs(o),Math.abs(x))&&Math.abs(u-w)<=r.b*Math.max(1,Math.abs(u),Math.abs(w))&&Math.abs(c-A)<=r.b*Math.max(1,Math.abs(c),Math.abs(A))&&Math.abs(i-E)<=r.b*Math.max(1,Math.abs(i),Math.abs(E))&&Math.abs(s-y)<=r.b*Math.max(1,Math.abs(s),Math.abs(y))&&Math.abs(f-q)<=r.b*Math.max(1,Math.abs(f),Math.abs(q))&&Math.abs(l-P)<=r.b*Math.max(1,Math.abs(l),Math.abs(P))&&Math.abs(h-R)<=r.b*Math.max(1,Math.abs(h),Math.abs(R))&&Math.abs(M-T)<=r.b*Math.max(1,Math.abs(M),Math.abs(T))&&Math.abs(m-S)<=r.b*Math.max(1,Math.abs(m),Math.abs(S))&&Math.abs(d-I)<=r.b*Math.max(1,Math.abs(d),Math.abs(I))&&Math.abs(b-j)<=r.b*Math.max(1,Math.abs(b),Math.abs(j))&&Math.abs(p-B)<=r.b*Math.max(1,Math.abs(p),Math.abs(B))&&Math.abs(_-F)<=r.b*Math.max(1,Math.abs(_),Math.abs(F))};var r=e(0);function a(t){return t[0]=1,t[1]=0,t[2]=0,t[3]=0,t[4]=0,t[5]=1,t[6]=0,t[7]=0,t[8]=0,t[9]=0,t[10]=1,t[11]=0,t[12]=0,t[13]=0,t[14]=0,t[15]=1,t}function o(t,n,e){let r=n[0],a=n[1],o=n[2],u=n[3],c=n[4],i=n[5],s=n[6],f=n[7],l=n[8],h=n[9],M=n[10],m=n[11],d=n[12],b=n[13],p=n[14],_=n[15],v=e[0],g=e[1],x=e[2],w=e[3];return t[0]=v*r+g*c+x*l+w*d,t[1]=v*a+g*i+x*h+w*b,t[2]=v*o+g*s+x*M+w*p,t[3]=v*u+g*f+x*m+w*_,v=e[4],g=e[5],x=e[6],w=e[7],t[4]=v*r+g*c+x*l+w*d,t[5]=v*a+g*i+x*h+w*b,t[6]=v*o+g*s+x*M+w*p,t[7]=v*u+g*f+x*m+w*_,v=e[8],g=e[9],x=e[10],w=e[11],t[8]=v*r+g*c+x*l+w*d,t[9]=v*a+g*i+x*h+w*b,t[10]=v*o+g*s+x*M+w*p,t[11]=v*u+g*f+x*m+w*_,v=e[12],g=e[13],x=e[14],w=e[15],t[12]=v*r+g*c+x*l+w*d,t[13]=v*a+g*i+x*h+w*b,t[14]=v*o+g*s+x*M+w*p,t[15]=v*u+g*f+x*m+w*_,t}function u(t,n,e){return t[0]=n[0]-e[0],t[1]=n[1]-e[1],t[2]=n[2]-e[2],t[3]=n[3]-e[3],t[4]=n[4]-e[4],t[5]=n[5]-e[5],t[6]=n[6]-e[6],t[7]=n[7]-e[7],t[8]=n[8]-e[8],t[9]=n[9]-e[9],t[10]=n[10]-e[10],t[11]=n[11]-e[11],t[12]=n[12]-e[12],t[13]=n[13]-e[13],t[14]=n[14]-e[14],t[15]=n[15]-e[15],t}const c=o;n.mul=c;const i=u;n.sub=i},function(t,n,e){"use strict";Object.defineProperty(n,"__esModule",{value:!0}),n.create=c,n.identity=function(t){return t[0]=0,t[1]=0,t[2]=0,t[3]=1,t},n.setAxisAngle=i,n.getAxisAngle=function(t,n){let e=2*Math.acos(n[3]),r=Math.sin(e/2);0!=r?(t[0]=n[0]/r,t[1]=n[1]/r,t[2]=n[2]/r):(t[0]=1,t[1]=0,t[2]=0);return e},n.multiply=s,n.rotateX=function(t,n,e){e*=.5;let r=n[0],a=n[1],o=n[2],u=n[3],c=Math.sin(e),i=Math.cos(e);return t[0]=r*i+u*c,t[1]=a*i+o*c,t[2]=o*i-a*c,t[3]=u*i-r*c,t},n.rotateY=function(t,n,e){e*=.5;let r=n[0],a=n[1],o=n[2],u=n[3],c=Math.sin(e),i=Math.cos(e);return t[0]=r*i-o*c,t[1]=a*i+u*c,t[2]=o*i+r*c,t[3]=u*i-a*c,t},n.rotateZ=function(t,n,e){e*=.5;let r=n[0],a=n[1],o=n[2],u=n[3],c=Math.sin(e),i=Math.cos(e);return t[0]=r*i+a*c,t[1]=a*i-r*c,t[2]=o*i+u*c,t[3]=u*i-o*c,t},n.calculateW=function(t,n){let e=n[0],r=n[1],a=n[2];return t[0]=e,t[1]=r,t[2]=a,t[3]=Math.sqrt(Math.abs(1-e*e-r*r-a*a)),t},n.slerp=f,n.invert=function(t,n){let e=n[0],r=n[1],a=n[2],o=n[3],u=e*e+r*r+a*a+o*o,c=u?1/u:0;return t[0]=-e*c,t[1]=-r*c,t[2]=-a*c,t[3]=o*c,t},n.conjugate=function(t,n){return t[0]=-n[0],t[1]=-n[1],t[2]=-n[2],t[3]=n[3],t},n.fromMat3=l,n.fromEuler=function(t,n,e,r){let a=.5*Math.PI/180;n*=a,e*=a,r*=a;let o=Math.sin(n),u=Math.cos(n),c=Math.sin(e),i=Math.cos(e),s=Math.sin(r),f=Math.cos(r);return t[0]=o*i*f-u*c*s,t[1]=u*c*f+o*i*s,t[2]=u*i*s-o*c*f,t[3]=u*i*f+o*c*s,t},n.str=function(t){return"quat("+t[0]+", "+t[1]+", "+t[2]+", "+t[3]+")"};var r=e(0),a=e(4),o=e(5),u=e(6);function c(){let t=new r.a(4);return t[0]=0,t[1]=0,t[2]=0,t[3]=1,t}function i(t,n,e){e*=.5;let r=Math.sin(e);return t[0]=r*n[0],t[1]=r*n[1],t[2]=r*n[2],t[3]=Math.cos(e),t}function s(t,n,e){let r=n[0],a=n[1],o=n[2],u=n[3],c=e[0],i=e[1],s=e[2],f=e[3];return t[0]=r*f+u*c+a*s-o*i,t[1]=a*f+u*i+o*c-r*s,t[2]=o*f+u*s+r*i-a*c,t[3]=u*f-r*c-a*i-o*s,t}function f(t,n,e,r){let a,o,u,c,i,s=n[0],f=n[1],l=n[2],h=n[3],M=e[0],m=e[1],d=e[2],b=e[3];return(o=s*M+f*m+l*d+h*b)<0&&(o=-o,M=-M,m=-m,d=-d,b=-b),1-o>1e-6?(a=Math.acos(o),u=Math.sin(a),c=Math.sin((1-r)*a)/u,i=Math.sin(r*a)/u):(c=1-r,i=r),t[0]=c*s+i*M,t[1]=c*f+i*m,t[2]=c*l+i*d,t[3]=c*h+i*b,t}function l(t,n){let e,r=n[0]+n[4]+n[8];if(r>0)e=Math.sqrt(r+1),t[3]=.5*e,e=.5/e,t[0]=(n[5]-n[7])*e,t[1]=(n[6]-n[2])*e,t[2]=(n[1]-n[3])*e;else{let r=0;n[4]>n[0]&&(r=1),n[8]>n[3*r+r]&&(r=2);let a=(r+1)%3,o=(r+2)%3;e=Math.sqrt(n[3*r+r]-n[3*a+a]-n[3*o+o]+1),t[r]=.5*e,e=.5/e,t[3]=(n[3*a+o]-n[3*o+a])*e,t[a]=(n[3*a+r]+n[3*r+a])*e,t[o]=(n[3*o+r]+n[3*r+o])*e}return t}const h=u.b;n.clone=h;const M=u.g;n.fromValues=M;const m=u.c;n.copy=m;const d=u.l;n.set=d;const b=u.a;n.add=b;const p=s;n.mul=p;const _=u.k;n.scale=_;const v=u.d;n.dot=v;const g=u.i;n.lerp=g;const x=u.h;n.length=x;const w=x;n.len=w;const A=u.m;n.squaredLength=A;const E=A;n.sqrLen=E;const y=u.j;n.normalize=y;const q=u.f;n.exactEquals=q;const P=u.e;n.equals=P;const R=function(){let t=o.create(),n=o.fromValues(1,0,0),e=o.fromValues(0,1,0);return function(r,a,u){let c=o.dot(a,u);return c<-.999999?(o.cross(t,n,a),o.len(t)<1e-6&&o.cross(t,e,a),o.normalize(t,t),i(r,t,Math.PI),r):c>.999999?(r[0]=0,r[1]=0,r[2]=0,r[3]=1,r):(o.cross(t,a,u),r[0]=t[0],r[1]=t[1],r[2]=t[2],r[3]=1+c,y(r,r))}}();n.rotationTo=R;const T=function(){let t=c(),n=c();return function(e,r,a,o,u,c){return f(t,r,u,c),f(n,a,o,c),f(e,t,n,2*c*(1-c)),e}}();n.sqlerp=T;const S=function(){let t=a.a();return function(n,e,r,a){return t[0]=r[0],t[3]=r[1],t[6]=r[2],t[1]=a[0],t[4]=a[1],t[7]=a[2],t[2]=-e[0],t[5]=-e[1],t[8]=-e[2],y(n,l(n,t))}}();n.setAxes=S},function(t,n,e){"use strict";Object.defineProperty(n,"__esModule",{value:!0}),n.create=a,n.clone=function(t){let n=new r.a(2);return n[0]=t[0],n[1]=t[1],n},n.fromValues=function(t,n){let e=new r.a(2);return e[0]=t,e[1]=n,e},n.copy=function(t,n){return t[0]=n[0],t[1]=n[1],t},n.set=function(t,n,e){return t[0]=n,t[1]=e,t},n.add=function(t,n,e){return t[0]=n[0]+e[0],t[1]=n[1]+e[1],t},n.subtract=o,n.multiply=u,n.divide=c,n.ceil=function(t,n){return t[0]=Math.ceil(n[0]),t[1]=Math.ceil(n[1]),t},n.floor=function(t,n){return t[0]=Math.floor(n[0]),t[1]=Math.floor(n[1]),t},n.min=function(t,n,e){return t[0]=Math.min(n[0],e[0]),t[1]=Math.min(n[1],e[1]),t},n.max=function(t,n,e){return t[0]=Math.max(n[0],e[0]),t[1]=Math.max(n[1],e[1]),t},n.round=function(t,n){return t[0]=Math.round(n[0]),t[1]=Math.round(n[1]),t},n.scale=function(t,n,e){return t[0]=n[0]*e,t[1]=n[1]*e,t},n.scaleAndAdd=function(t,n,e,r){return t[0]=n[0]+e[0]*r,t[1]=n[1]+e[1]*r,t},n.distance=i,n.squaredDistance=s,n.length=f,n.squaredLength=l,n.negate=function(t,n){return t[0]=-n[0],t[1]=-n[1],t},n.inverse=function(t,n){return t[0]=1/n[0],t[1]=1/n[1],t},n.normalize=function(t,n){var e=n[0],r=n[1],a=e*e+r*r;a>0&&(a=1/Math.sqrt(a),t[0]=n[0]*a,t[1]=n[1]*a);return t},n.dot=function(t,n){return t[0]*n[0]+t[1]*n[1]},n.cross=function(t,n,e){var r=n[0]*e[1]-n[1]*e[0];return t[0]=t[1]=0,t[2]=r,t},n.lerp=function(t,n,e,r){var a=n[0],o=n[1];return t[0]=a+r*(e[0]-a),t[1]=o+r*(e[1]-o),t},n.random=function(t,n){n=n||1;var e=2*r.c()*Math.PI;return t[0]=Math.cos(e)*n,t[1]=Math.sin(e)*n,t},n.transformMat2=function(t,n,e){var r=n[0],a=n[1];return t[0]=e[0]*r+e[2]*a,t[1]=e[1]*r+e[3]*a,t},n.transformMat2d=function(t,n,e){var r=n[0],a=n[1];return t[0]=e[0]*r+e[2]*a+e[4],t[1]=e[1]*r+e[3]*a+e[5],t},n.transformMat3=function(t,n,e){var r=n[0],a=n[1];return t[0]=e[0]*r+e[3]*a+e[6],t[1]=e[1]*r+e[4]*a+e[7],t},n.transformMat4=function(t,n,e){let r=n[0],a=n[1];return t[0]=e[0]*r+e[4]*a+e[12],t[1]=e[1]*r+e[5]*a+e[13],t},n.str=function(t){return"vec2("+t[0]+", "+t[1]+")"},n.exactEquals=function(t,n){return t[0]===n[0]&&t[1]===n[1]},n.equals=function(t,n){let e=t[0],a=t[1],o=n[0],u=n[1];return Math.abs(e-o)<=r.b*Math.max(1,Math.abs(e),Math.abs(o))&&Math.abs(a-u)<=r.b*Math.max(1,Math.abs(a),Math.abs(u))};var r=e(0);function a(){let t=new r.a(2);return t[0]=0,t[1]=0,t}function o(t,n,e){return t[0]=n[0]-e[0],t[1]=n[1]-e[1],t}function u(t,n,e){return t[0]=n[0]*e[0],t[1]=n[1]*e[1],t}function c(t,n,e){return t[0]=n[0]/e[0],t[1]=n[1]/e[1],t}function i(t,n){var e=n[0]-t[0],r=n[1]-t[1];return Math.sqrt(e*e+r*r)}function s(t,n){var e=n[0]-t[0],r=n[1]-t[1];return e*e+r*r}function f(t){var n=t[0],e=t[1];return Math.sqrt(n*n+e*e)}function l(t){var n=t[0],e=t[1];return n*n+e*e}const h=f;n.len=h;const M=o;n.sub=M;const m=u;n.mul=m;const d=c;n.div=d;const b=i;n.dist=b;const p=s;n.sqrDist=p;const _=l;n.sqrLen=_;const v=function(){let t=a();return function(n,e,r,a,o,u){let c,i;for(e||(e=2),r||(r=0),i=a?Math.min(a*e+r,n.length):n.length,c=r;c<i;c+=e)t[0]=n[c],t[1]=n[c+1],o(t,t,u),n[c]=t[0],n[c+1]=t[1];return n}}();n.forEach=v},function(t,n,e){"use strict";e.d(n,"a",function(){return o});var r=e(1),a=e(2);class o{constructor(t,n){this.matrix_=r.a.create(),this.mat4Position_=t,this.quatRotate_=n}move(t){r.d.scaleAndAdd(this.mat4Position_,this.mat4Position_,t,a.c)}rotate(t){let n=r.d.length(t)*a.b,e=r.b.setAxisAngle(r.b.create(),t,n);r.b.multiply(this.quatRotate_,this.quatRotate_,e)}getMatrix(){return r.a.fromRotationTranslation(r.a.create(),this.quatRotate_,this.mat4Position_)}}},function(t,n,e){"use strict";e.d(n,"a",function(){return a});var r=e(1);class a{constructor(t,n,e,r,a){this.vertexBuffer_=t,this.indicesBuffer_=n,this.shaderProgram_=e,this.vec3Position_=r,this.quatRotation_=a}getMatrix(){let t=r.a.create();return r.a.fromRotationTranslation(t,this.quatRotation_,this.vec3Position_),t}}},function(t,n,e){"use strict";e.d(n,"c",function(){return r}),e.d(n,"e",function(){return a}),e.d(n,"d",function(){return o}),e.d(n,"a",function(){return u}),e.d(n,"b",function(){return c});var r=[.5,1,0,255],a=.1,o=50,u=[0,0,0],c=[0,0,0]},function(t,n,e){"use strict";n.a=function(t,n){r=n;var e=document.getElementById(t);addEventListener("keydown",i),e.onmousedown=function(t){a=o.c.fromValues(t.pageX,t.pageY),e.onmousemove=c},e.onmouseup=function(t){e.onmousemove=void 0}};var r,a,o=e(1),u=(e(3),e(2));function c(t){let n=o.c.create();document.all?(n[0]=event.x+document.body.scrollLeft,n[1]=event.y+document.body.scrollTop):(n[0]=t.pageX,n[1]=t.pageY);let e=o.c.subtract(o.c.create(),n,a);r.camera_.rotate([e[1],e[0],0]),a=n,r.rend()}function i(t){switch(t.keyCode){case u.a.zoomUp:r.camera_.move(o.d.fromValues(0,0,1));break;case u.a.zoomDown:r.camera_.move(o.d.fromValues(0,0,-1));break;case u.a.left:r.camera_.move(o.d.fromValues(1,0,0));break;case u.a.forward:r.camera_.move(o.d.fromValues(0,-1,0));break;case u.a.right:r.camera_.move(o.d.fromValues(-1,0,0));break;case u.a.back:r.camera_.move(o.d.fromValues(0,1,0))}r.rend()}},function(t,n,e){"use strict";var r;function a(t){void 0===r&&(void 0!==r&&r.close(1e3,""),(r=new WebSocket("ws://localhost:3000/setConnect")).onopen=function(t){console.log("WebSocket is connected.")},r.onclose=function(t){console.log("Close connection."),r=void 0}),r.send(t)}e.d(n,"a",function(){return a})},function(t,n,e){"use strict";e.d(n,"a",function(){return a});const r={ply:e(19).a};function a(t,n){let e=r[t];if(void 0!==e)return e(n);alert("Unsupport format.")}},function(t,n,e){"use strict";e.d(n,"a",function(){return u});const r=/element +(\w+) +([0-9]+)/g,a=/ply([\w\W]+)end_header/g,o=/format (\w+)/g;function u(t,n){let e=new TextDecoder("utf-8").decode(new Uint8Array(t)).match(a);if(null===e)return void alert("Don`t find file head");let u=(e=e[0]).match(o);if(null===u)return void alert("Don`t find data format");let c=e.split(r);c.shift();let i=function(t){let n=new Array;for(let e=0;e<t.length;e+=3){let r=new Object;r.nameElem=t[e],r.count=Number.parseInt(t[e+1]),r.struct=new Object,t[e+2].split("\n").forEach(function(t){if(0!==t.length){let n=t.match(/\w+/g);if(n.length>1){let t=n.pop();r.struct[t]=new Object,r.struct[t].valType=n[1],"list"===r.struct[t].valType&&(r.struct[t].listSizeType=n[2],r.struct[t].listValType=n[3])}}}),n.push(r)}return n}(c);return n={vertex:["x","y","z"],face:["vertex_indices"]},"format ascii"===u[0]?function(t,n,e,r){let a=new Object,o=new TextDecoder("utf-8").decode(new Uint8Array(r,e)).match(/[0-9. -]+/g),u=0;return t.forEach(function(t){let e=u;if(u+=t.count,void 0!==n[t.nameElem])for(a[t.nameElem]=new Array;e<u;e++){let r=o[e].match(/[0-9.-]+/g),u=0;for(let n in t.struct){const e=t.struct[n];switch(e.valType){case"float":e.data=parseFloat(r[u]);break;case"list":let t=parseInt(r[u]);u++,e.data=new Array;for(let n=0;n<t;n++)"float"===e.listValType?e.data.push(parseFloat(r[u])):e.data.push(parseInt(r[u])),u++;u--;break;default:e.data=parseInt(r[u])}u++}n[t.nameElem].forEach(function(n){a[t.nameElem]=a[t.nameElem].concat(t.struct[n].data)})}}),a}(i,n,e.length,t):void alert("Unknown data format")}}]);
//# sourceMappingURL=script.js.map