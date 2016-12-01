cordova.define("com.xx.example.mediaRenderer", function(require, exports, module) {
var exec = require('cordova/exec');
var myFunc = function(){};

myFunc.prototype.scan = function(success,error,param){
  exec(success,error,"MediaRenderer","dial_scan",[param]);
};

myFunc.prototype.start = function (param) {
  exec(null,null,"MediaRenderer","dial_start",[param]);
};

myFunc.prototype.stop = function(){
  exec(null,null,"MediaRenderer","dial_stop",[]);
};

myFunc.prototype.play = function () {
  exec(null,null,"MediaRenderer","dial_play",[]);
};

myFunc.prototype.pause = function () {
  exec (null,null,"MediaRenderer","dial_pause",[]);
};

myFunc.prototype.seek = function (param) {
  exec (null,null,"MediaRenderer","dial_seek",[param]);
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
  
  //调用原声代码
  exec(successCallback,errorCallback,"MediaRenderer","test",[param]);
}


var screen = new myFunc();
module.exports = screen;

});
