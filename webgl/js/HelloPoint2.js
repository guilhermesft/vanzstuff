//Vertex shader
var VSHADER_SOURCE =
	'attribute vec4 a_Position;\n' +
	'attribute float a_PSize;\n' +
	'void main() {\n' +
	'  gl_Position = a_Position;\n' +
	'  gl_PointSize = a_PSize;\n' +
	'}\n';

var FSHADER_SOURCE =
	'void main() {\n' +
	'  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n' +
	'}\n';

function main(){
	var canvas = document.getElementById('webgl');
	gl = getWebGLContext(canvas);
	if( !gl ){
		console.log('Failed to get the rendering context for WebGK');
		return;
	}
	if( !initShaders(gl, VSHADER_SOURCE, FSHADER_SOURCE)){
		console.log('Failed to initialize shaders');
		return;
	}
	var a_Position = gl.getAttribLocation(gl.program, 'a_Position');
	if( a_Position < 0){
		console.log('Failed to get the storage location of a a_Position');
		return;
	}
	var a_PSize = gl.getAttribLocation(gl.program, 'a_PSize');
	if( a_PSize < 0){
		console.log('Failed to get the storage location of a a_PSize');
		return;
	}

	gl.vertexAttrib3f(a_Position, 0.0, 0.0, 0.0);
	gl.vertexAttrib1f(a_PSize, 5.0);
	gl.clearColor(0.0, 0.0, 0.0, 1.0);
	gl.clear(gl.COLOR_BUFFER_BIT);
	gl.drawArrays(gl.POINTS, 0, 1);
}

