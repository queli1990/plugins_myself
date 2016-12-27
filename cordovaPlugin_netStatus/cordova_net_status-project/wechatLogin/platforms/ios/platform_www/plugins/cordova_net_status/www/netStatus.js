cordova.define("cordova_net_status.netStatusPlugin", function(require, exports, module) {
var exec = require('cordova/exec');
var myFunc = function() {};



myFunc.prototype.getNetStatus = function(success) {
	exec(success, null, "NetStatus", "getNetStatus", []);
};

myFunc.prototype.registerNetStatus = function(success) {
	exec(success, null, "NetStatus", "registerNetStatus", []);
};



var netStatus = new myFunc();
module.exports = netStatus;
});
