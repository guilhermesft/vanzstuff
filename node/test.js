"use strict";

var http = require("http");
var spawn = require("child_process").spawn;

const COUNT = 50;

for (var i = 0; i < COUNT; ++i){
	setTimeout(function(i) {
		spawn("curl", ["127.0.0.1:8989/" + i]).stdout
			.on("data", (data) => {
				console.log(data.toString());
			});
	}, getRandomInt(0,5), i);
}

function getRandomInt(min, max) {
	min = Math.ceil(min);
	max = Math.floor(max);
	return (Math.floor(Math.random() * (max - min)) + min)*1000;
}
