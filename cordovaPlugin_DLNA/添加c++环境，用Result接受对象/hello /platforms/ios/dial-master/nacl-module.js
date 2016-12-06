/* ***** BEGIN LICENSE BLOCK *****
 * Distributed under the BSD license:
 *
 * Copyright (c) 2015-2016, Gochinatv
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Gochinatv nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Gochinatv BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * ***** END LICENSE BLOCK ***** */

 (function () {

 	if ( window.DialModule ) {
 		return;
 	}

 	function moduleDidLoad(self, evt) { 
 		// console.debug('Dial moduleDidLoad');
 		self._onload();
 	}
 	function handleMessage(self, evt) { 

 		var tag = evt.data.tag;
 		var data = evt.data.data;

 		if ( tag == 'CONSOLE_LOG' ) {
 			console.log(data);
 		} else if ( tag == 'CONSOLE_ERR' ) {
 			console.error(data);
 		} else {
 			var err = null;
 			if ( data.indexOf('ERR:') == 0 ) {
				err = data.substr(4);
				data = null;
 			}
 			var cb = self._cb[tag].shift();
 			if ( cb ) {
 				cb(err, data);
 			}
 		}
 	}
 	function handleError(self, evt) { 
		console.error('Nacl DialModule error');
 	}
 	function handleCrash(self, evt) { 
 		console.error('Nacl DialModule crash');
 	}

 	function post_message(self, cmd, args, cb) {
 		cb = cb || function() { };

 		args.unshift(cmd);

 		var cbs = self._cb[cmd];
 		if ( ! cbs ) {
 			self._cb[cmd] = cbs = [ ];
 		}
		cbs.push( cb );

 		self._module.postMessage(args);
 	}

 	function DialModule(nacl_path, onload) {

 		if ( typeof nacl_path == 'function' ) {
 			onload = nacl_path;
 			nacl_path = 'pnacl/Release';
 		} else {
 			nacl_path = nacl_path || 'pnacl/Release';
 			if ( typeof onload != 'function' ) {
 				onload = function () { };
 			}
 		}

 		var self = this;
 		
 		var embed = document.createElement('embed');
 		embed.width = 0;
 		embed.height = 0;
 		embed.style.width = 0;
 		embed.style.height = 0;
 		embed.setAttribute('type', 'application/x-pnacl'); 
 		embed.setAttribute('path', nacl_path); 
 		embed.setAttribute('src', nacl_path + '/dialclient.nmf');

 		embed.addEventListener('load', function (evt) { moduleDidLoad(self, evt) }, true);
    embed.addEventListener('message', function (evt) { handleMessage(self, evt) }, true);
    embed.addEventListener('error', function (evt) { handleError(self, evt) }, true);
    embed.addEventListener('crash', function (evt) { handleCrash(self, evt) }, true);

   	if (typeof window.attachListeners !== 'undefined') {
      window.attachListeners();
    }

 		var body = document.body;
 		if ( ! body ) {
 			document.write( '<div style="display:none"></div>' );
 		}
 		body.appendChild(embed);
 		this._module = embed;
 		this._onload = onload;
 		this._cb = { };
 	}

	DialModule.prototype.scan = function (cb) {
		post_message(this, 'scan', [], cb);
	}

	DialModule.prototype.host_list = function (cb) {
		post_message(this, 'host_list', [], cb);	
	}

	DialModule.prototype.launch = function (host, app, payload, cb) {
		post_message(this, 'launch', [host, app, payload], cb);
	}

	DialModule.prototype.hide = function (host, app, cb) {
		post_message(this, 'hide', [host, app], cb);
	}

	DialModule.prototype.stop = function (host, app, cb) {
		post_message(this, 'stop', [host, app], cb);
	}

	DialModule.prototype.status = function (host, app, cb) {
		post_message(this, 'status', [host, app], cb);
	}

	DialModule.prototype.release = function () {
		// this._module.parentNode.removeChild(this._module);
	}

	window.DialModule = DialModule;
})();