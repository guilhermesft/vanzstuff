function getWebGLContext( canvas, debug){
	var ctx = canvas.getContext('webgl');
	if( !ctx ){
		console.log("Failed to initialize WebGL context");
		return null;
	}
	return ctx;
}
