var native = require("./native/build/Release/xmlsecnode");
var restify = require("restify")

console.log(native.hello());

var server = restify.createServer();
