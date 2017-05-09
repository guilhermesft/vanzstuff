"use strict";

var pam = require("authenticate-pam");
var http = require("http");
var url = require("url");

http.createServer(function(req, res) {
	var path = url.parse(req.url);
	console.log(`Request ${path.pathname}`);
	pam.authenticate("user", "password",
		function(err, sid) {
			if (err){
				res.writeHead(500);
				res.end("FAILED");
			} else {
				res.writeHead(200);
				res.end(`Request: ${path.pathname}`);
			}
		});
}).listen(8989, function(){
	console.log("Listening...");
});
