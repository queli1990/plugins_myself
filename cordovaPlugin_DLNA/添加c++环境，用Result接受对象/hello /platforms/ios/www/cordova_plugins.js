cordova.define('cordova/plugin_list', function(require, exports, module) {
module.exports = [
    {
        "id": "com.cordovaDlnaDemo.example.mediaRenderer",
        "file": "plugins/com.cordovaDlnaDemo.example/www/mediaRenderer.js",
        "pluginId": "com.cordovaDlnaDemo.example",
        "clobbers": [
            "mediaRenderer"
        ]
    }
];
module.exports.metadata = 
// TOP OF METADATA
{
    "cordova-plugin-whitelist": "1.3.0",
    "com.cordovaDlnaDemo.example": "0.0.1"
};
// BOTTOM OF METADATA
});