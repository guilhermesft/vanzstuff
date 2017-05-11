var native = require("./native/build/Release/xmlsec");
var restify = require("restify")

console.log(native.hello());

var server = restify.createServer();
