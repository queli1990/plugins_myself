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

myFunc.prototype.start = function(hostName, url, success, error) {
	exec(success, error, "MediaRenderer", "mediarenderer_start", [hostName, url]);
};

myFunc.prototype.play = function(hostName, success, error) {
	exec(success, error, "MediaRenderer", "mediarenderer_play", [hostName]);
};

myFunc.prototype.pause = function(hostName, success, error) {
	exec(success, error, "MediaRenderer", "mediarenderer_pause", [hostName]);
};

myFunc.prototype.setVolume = function(hostName, volume, success, error) {
	exec(success, error, "MediaRenderer", "mediarenderer_volume", [hostName, volume]);
};

myFunc.prototype.getSysVolume = function(hostName, success, error) {
	exec(success, error, "MediaRenderer", "mediarenderer_getSysVolume", [hostName]);
};

myFunc.prototype.getVideoInfo = function(hostName, success, error) {
	exec(success, error, "MediaRenderer", "mediarenderer_getVideoInfo", [hostName]);
};

myFunc.prototype.stop = function(hostName, success, error) {
	exec(success, error, "MediaRenderer", "mediarenderer_stop", [hostName]);
};

myFunc.prototype.seek = function(hostName, seekTo, success, error) {
	exec(success, error, "MediaRenderer", "mediarenderer_seek", [hostName, seekTo]);
};

var screen = new myFunc();
module.exports = screen;