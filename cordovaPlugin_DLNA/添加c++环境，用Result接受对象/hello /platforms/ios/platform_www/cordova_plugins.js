cordova.define('cordova/plugin_list', function(require, exports, module) {
module.exports = [
    {
        "id": "dlna_media_renderer.dlna",
        "file": "plugins/dlna_media_renderer/www/dlna.js",
        "pluginId": "dlna_media_renderer",
        "clobbers": [
            "dlna"
        ]
    }
];
module.exports.metadata = 
// TOP OF METADATA
{
    "cordova-plugin-whitelist": "1.3.0",
    "dlna_media_renderer": "0.0.1"
};
// BOTTOM OF METADATA
});