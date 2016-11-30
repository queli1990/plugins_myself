var exec = require('cordova/exec');
var myFunc = function(){};

myFunc.prototype.liveUrl = function(successCallback,errorCallback,param){
  alert('start....');
  exec(successCallback,errorCallback,"YouTubeUrl","getYouTubeLiveUrl",[param]);
};

myFunc.prototype.normalUrl = function(successCallback,errorCallback,param){
  alert('start....');

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
  exec(successCallback,errorCallback,"YouTubeUrl","getYouTubeUrl",[param]);
}


var screen = new myFunc();
module.exports = screen;
