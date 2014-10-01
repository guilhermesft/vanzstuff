function getWebGLContext( canvas, debug){
	var ctx = canvas.getContext('webgl');
	if( !ctx ){
		console.log("Failed to initialize WebGL context");
		return null;
	}
	return ctx;
}

function initShaders(gl, vshader, fshader){
	var vertexShader = compile(gl, gl.VERTEX_SHADER, vshader);
	var fragmentShader = compile(gl, gl.FRAGMENT_SHADER, fshader);
	var program = null;
	if( vertexShader && fragmentShader ){
		program = link(gl, vertexShader, fragmentShader);
	}
	if( !program ){
		return false;
	}
	gl.useProgram(program);
	gl.program = program;
	return true;
}

/**
 * This function link and return a program that uses the vertex and fragment
 * shader passed in the parameteres
 */
function link(gl, vertexShader, fragmentShader){
	var program = gl.createProgram();
	gl.attachShader(program, vertexShader);
	gl.attachShader(program, fragmentShader);
	gl.linkProgram(program);

	if( !gl.getProgramParameter(program, gl.LINK_STATUS)){
		throw new Error(gl.getProgramInfoLog(program));
	}
	return program;
};

/**
 * Compile the shader source and return the shader obj
 */
function compile(gl, type, shaderSrc){
	var shader = gl.createShader(type);
	gl.shaderSource(shader, shaderSrc);
	gl.compileShader(shader);

	if( !gl.getShaderParameter(shader, gl.COMPILE_STATUS)){
		throw new Error(gl.getShaderInfoLog(shader));
	}
	return shader;
};


