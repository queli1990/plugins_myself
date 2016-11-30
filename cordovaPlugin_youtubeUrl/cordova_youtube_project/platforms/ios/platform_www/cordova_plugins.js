cordova.define('cordova/plugin_list', function(require, exports, module) {
module.exports = [
    {
        "id": "com.cordova.youtube.getYoutubeUrl",
        "file": "plugins/com.cordova.youtube/www/getYoutubeUrl.js",
        "pluginId": "com.cordova.youtube",
        "clobbers": [
            "getYoutubeUrl"
        ]
    }
];
module.exports.metadata = 
// TOP OF METADATA
{
    "cordova-plugin-whitelist": "1.3.0",
    "com.cordova.youtube": "0.0.1"
};
// BOTTOM OF METADATA
});