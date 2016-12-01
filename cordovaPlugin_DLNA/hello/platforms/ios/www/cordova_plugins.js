cordova.define('cordova/plugin_list', function(require, exports, module) {
module.exports = [
    {
        "id": "com.xx.example.mediaRenderer",
        "file": "plugins/com.xx.example/www/mediaRenderer.js",
        "pluginId": "com.xx.example",
        "clobbers": [
            "mediaRenderer"
        ]
    }
];
module.exports.metadata = 
// TOP OF METADATA
{
    "cordova-plugin-whitelist": "1.3.0",
    "com.xx.example": "0.0.1"
};
// BOTTOM OF METADATA
});