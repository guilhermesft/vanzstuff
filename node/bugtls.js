var native = require("xmlsecnode");
var restify = require("restify")

console.log(native.hello());

var server = restify.createServer();
