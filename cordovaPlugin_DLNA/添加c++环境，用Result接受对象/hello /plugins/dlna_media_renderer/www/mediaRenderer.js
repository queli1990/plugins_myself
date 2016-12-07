var exec = require('cordova/exec');
var platform = require('cordova/platform');

var myFunc = function() {};

myFunc.prototype.init = function() {
	exec(null, null, "MediaRenderer", "init", []);
};

myFunc.prototype.scan = function(success, error) {
	exec(success, error, "MediaRenderer", "scan", []);
};

myFunc.prototype.stopScan = function() {
	exec(null, null, "MediaRenderer", "stopScan", []);
};


myFunc.prototype.start = function(success, error, hostName, url) {
	exec(success, error, "MediaRenderer", "mediarenderer_start", [hostName, url]);
};

myFunc.prototype.play = function(success, error, hostName) {
	exec(success, error, "MediaRenderer", "mediarenderer_play", [hostName]);
};

myFunc.prototype.pause = function(success, error, hostName) {
	exec(success, error, "MediaRenderer", "mediarenderer_pause", [hostName]);
};

myFunc.prototype.setVolume = function(success, error, hostName, volume) {
	exec(success, error, "MediaRenderer", "mediarenderer_volume", [hostName, volume]);
};

myFunc.prototype.getSysVolume = function(success, error, hostName) {
	exec(success, error, "MediaRenderer", "mediarenderer_getSysVolume", [hostName]);
};

myFunc.prototype.getVideoInfo = function(success, error, hostName) {
	exec(success, error, "MediaRenderer", "mediarenderer_getVideoInfo", [hostName]);
};

myFunc.prototype.stop = function(success, error, hostName) {
	exec(success, error, "MediaRenderer", "mediarenderer_stop", [hostName]);
};

myFunc.prototype.seek = function(success, error, hostName, seekTo) {
	exec(success, error, "MediaRenderer", "mediarenderer_seek", [hostName, seekTo]);
};

var screen = new myFunc();
module.exports = screen;