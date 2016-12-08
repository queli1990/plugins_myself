cordova.define("dlna_media_renderer.dlna", function(require, exports, module) {
var exec = require('cordova/exec');
var platform = require('cordova/platform');
 
var myFunc = function() {};
 
myFunc.prototype.init = function() {
	exec(null, null, "DLNA", "init", []);
};
 
myFunc.prototype.scan = function(success, error) {
	exec(success, error, "DLNA", "scan", []);
};
 
myFunc.prototype.stopScan = function() {
	exec(null, null, "DLNA", "stopScan", []);
};
 
 
myFunc.prototype.start = function(hostName, url, success, error) {
	exec(success, error, "DLNA", "mediarenderer_start", [hostName, url]);
};
 
myFunc.prototype.play = function(hostName, success, error) {
	exec(success, error, "DLNA", "mediarenderer_play", [hostName]);
};
 
myFunc.prototype.pause = function(hostName, success, error) {
	exec(success, error, "DLNA", "mediarenderer_pause", [hostName]);
};
 
myFunc.prototype.setVolume = function(hostName, volume, success, error) {
	exec(success, error, "DLNA", "mediarenderer_volume", [hostName, volume]);
};
 
myFunc.prototype.getSysVolume = function(hostName, success, error) {
	exec(success, error, "DLNA", "mediarenderer_getSysVolume", [hostName]);
};
 
myFunc.prototype.getVideoInfo = function(hostName, success, error) {
	exec(success, error, "DLNA", "mediarenderer_getVideoInfo", [hostName]);
};
 
myFunc.prototype.stop = function(hostName, success, error) {
	exec(success, error, "DLNA", "mediarenderer_stop", [hostName]);
};
 
myFunc.prototype.seek = function(hostName, seekTo, success, error) {
	exec(success, error, "DLNA", "mediarenderer_seek", [hostName, seekTo]);
};
 
myFunc.prototype.launchApp = function(hostName, appName, params, success, error) {
	exec(success, error, "DLNA", "dial_launch", [hostName, appName, params]);
};
 
myFunc.prototype.hideApp = function(hostName, appName, success, error) {
	exec(success, error, "DLNA", "dial_hide", [hostName, appName]);
};
 
myFunc.prototype.stopApp = function(hostName, appName, success, error) {
	exec(success, error, "DLNA", "dial_stop", [hostName, appName]);
};
 
myFunc.prototype.getAppStatus = function(hostName, appName, success, error) {
	exec(success, error, "DLNA", "dial_status", [hostName, appName]);
};
 
 
var screen = new myFunc();
module.exports = screen;
});
