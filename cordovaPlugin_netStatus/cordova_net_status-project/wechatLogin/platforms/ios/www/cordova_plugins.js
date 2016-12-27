cordova.define('cordova/plugin_list', function(require, exports, module) {
module.exports = [
    {
        "id": "cordova_net_status.netStatusPlugin",
        "file": "plugins/cordova_net_status/www/netStatus.js",
        "pluginId": "cordova_net_status",
        "clobbers": [
            "netStatus"
        ]
    }
];
module.exports.metadata = 
// TOP OF METADATA
{
    "cordova-plugin-whitelist": "1.3.1",
    "cordova_net_status": "0.0.1"
};
// BOTTOM OF METADATA
});