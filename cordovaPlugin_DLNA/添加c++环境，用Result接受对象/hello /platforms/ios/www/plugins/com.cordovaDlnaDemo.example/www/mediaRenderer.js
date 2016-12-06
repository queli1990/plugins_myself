cordova.define("com.cordovaDlnaDemo.example.mediaRenderer", function(require, exports, module) {
var exec = require('cordova/exec');
var myFunc = function(){};

myFunc.prototype.init = function(){
  exec(null,null,"MediaRenderer","mediarender_init",[]);
};

myFunc.prototype.scan = function(success,error){
  exec(success,error,"MediaRenderer","mediarender_scan",[]);
};

myFunc.prototype.stopScan = function(){
  exec(null,null,"MediaRenderer","mediarender_stopScan",[]);
};

myFunc.prototype.start = function (successCallback,errorCallback,hostName,url) {
  exec(successCallback,errorCallback,"MediaRenderer","mediarender_start",[hostName,url]);
};

myFunc.prototype.play = function (successCallback,errorCallback,hostName) {
  exec(successCallback,errorCallback,"MediaRenderer","mediarender_play",[hostName]);
};

myFunc.prototype.pause = function (successCallback,errorCallback,hostName) {
  exec (successCallback,errorCallback,"MediaRenderer","mediarender_pause",[hostName]);
};

myFunc.prototype.setVolume = function(successCallback,errorCallback,hostName, volume) {
  exec(successCallback, errorCallback, "MediaRenderer", "mediarender_setVolume", [hostName, volume]);
};

myFunc.prototype.getSysVolume = function(successCallback,errorCallback,hostName) {
  exec(successCallback, errorCallback, "MediaRenderer", "mediarender_getSytVolume", [hostName]);
};

myFunc.prototype.getVideoInfo = function(success, error,hostName) {
  exec(success, error, "MediaRenderer", "mediarender_getInfo", [hostName]);
};

myFunc.prototype.stop = function(successCallback,errorCallback,hostName){
  exec(successCallback,errorCallback,"MediaRenderer","mediarender_stop",[hostName]);
};

myFunc.prototype.seek = function (successCallback,errorCallback,hostName,seekTime) {
  exec (successCallback,errorCallback,"MediaRenderer","mediarender_seek",[hostName,seekTime]);
};


myFunc.prototype.test = function(successCallback,errorCallback,param){

if (errorCallback == null) {
  errorCallback = function(){}
}

if (typeof errorCallback != "function") {
  alert('failure parameter not a function');
  return ;
}

if (typeof successCallback != "function") {
  alert('success callback parameter must be a function');
  return 
}
  
  //调用原生代码
  exec(successCallback,errorCallback,"MediaRenderer","test",[param]);
}


var screen = new myFunc();
module.exports = screen;

});
