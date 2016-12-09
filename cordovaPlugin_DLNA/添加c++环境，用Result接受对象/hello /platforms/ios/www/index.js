 (function(){
  
  var ele = document.getElementById('log')
  var playing = false;
  var inited = false;
  var host = null;
  var length = 0;
  var current = 0;
  var machines = {};
  var delay = false;
  var TIME = 500;
  var urls = [
              'http://zhaoyang.cdn.ottcloud.tv/zhaoyang/videos/src/3669/72126-GLA.mp4',
              'https://media.w3.org/2010/05/sintel/trailer.mp4',
              'http://rishiji.cdn.ottcloud.tv/rishiji/videos/src/448/72430-3-25-ji-dan-juan-bing.mp4',
              'http://tuxiaobei.cdn.ottcloud.tv/tuxiaobei/videos/dst3/song/250/hls0800k/250.m3u8'
              ]
  function log(args){
		ele.innerText = ele.innerText + '\n' + JSON.stringify(args)
  }
  
  (function(){
   var hidden, visibilityChange;
   if (typeof document.hidden !== "undefined") { // Opera 12.10 and Firefox 18 and later support
			hidden = "hidden";
			visibilityChange = "visibilitychange";
   } else if (typeof document.webkitHidden !== "undefined") {
			hidden = "webkitHidden";
			visibilityChange = "webkitvisibilitychange";
   }
   $(document).on(visibilityChange, function(){
                  if (document[hidden]) {
                  dlna && dlna.stopScan()
                  $('#root').empty()
                  machines = {}
                  }else{
                  $('button.btn.sacn').click()
                  }
                  })
   }());
  
  // scan result
  function list(){
		var txt = '<ul class="list-group">';
		for(var key in machines){
  var item = machines[key]
  txt += '<li class="list-group-item" data-type="'+item.type+'" data-host="'+item.hostname+'" data-friendly_name="'+item.friendly_name+'">Cast to '+item.friendly_name+'</li>'
		}
		txt += '</ul>'
		$('#root').html(txt);
  }
  
  function play(host, url){
		dlna.start(host, url, function(res){
                   log(`start result: ${JSON.stringify(res)}`)
                   setTimeout(function(){
                              $('button.btn.getSysVolume').click()
                              $('button.btn.getVideoInfo').click()
                              },TIME)
                   }, function(err){
                   log(`start error: ${JSON.stringify(err)}`)
                   })
		log('------- Start --------')
		log(`host: ${host}, url: ${url}`)
		playing = true
  }
  
  // set
  $('#root').delegate('ul li', 'click', function(e){
                      var self = $(this)
                      host = self.data('host')
                      var type = self.data('type') // [MEDIA_RENDERER,]
                      var url = self.data('url') || $('input[name="url"]').val()
                      self.parent().find('li').removeClass('active')
                      self.addClass('active')
                      $('#machine').text(`Cast to ${self.data('friendly_name')}`)
                      play(host, url)
                      })
  
  $('button.btn.movie').click(function(){
                              var url = urls [Math.floor(Math.random() * urls.length)]
                              $('input[name="url"]').val(url)
                              if(!host){
                              return
                              }
                              dlna.stop(host)
                              play(host, url)
                              }).click();
  
  // scan
  $('button.btn.sacn').click(function(){
                             if(!inited) {return}
                             $('#root').empty();
                             machines = {};
                             log('------- Scan --------')
                             dlna.scan(function(res){
                                       if(typeof(res) != 'object'){
                                       res = JSON.parse(res)
                                       }
                                       $('#buttons button').each(function(i, item){
                                                                 var self = $(this)
                                                                 self.attr('class', (self.attr('class').replace('btn-default', 'btn-primary')))
                                                                 })
                                       res.map(function(item){
                                               if(machines[item.hostname]){
                                               return
                                               }
                                               if(item.is_invalid){
                                               return
                                               }
                                               machines[item.hostname] = item
                                               log(`scan:${JSON.stringify(item)}`)
                                               })
                                       list()
                                       },function(err){
                                       log('------- Error --------')
                                       log(err)
                                       })
                             })
  
  $('button.btn.stopsacn').click(function(){
                                 if(!inited) {return}
                                 dlna.stopScan(function(){},function(){});
                                 });
  
  $('button.btn.play').click(function(){
                             if(!inited || !host) {return}
                             dlna.play(host, function(res){
                                       log(`Play result: ${JSON.stringify(res)}`)
                                       }, function(err){
                                       log(`Play error: ${JSON.stringify(err)}`)
                                       })
                             //log('------- Play --------')
                             //log(`Play: ${host}`)
                             playing = !playing;
                             })
  
  $('button.btn.pause').click(function(){
                              if(!inited || !host) {return}
                              dlna.pause(host, function(res){
                                         log(`pause result: ${JSON.stringify(res)}`)
                                         }, function(err){
                                         log(`pause error: ${JSON.stringify(err)}`)
                                         })
                              //log('------- Pause --------')
                              //log(`Pause: ${host}`)
                              playing = !playing;
                              })
  
  $('button.btn.getVideoInfo').click(function(){
                                     var self = this;
                                     if(!inited || !host) {return}
                                     dlna.getVideoInfo(host, function(res){
                                                       if(res.code === 0){
                                                       length = res.data.TrackDuration;
                                                       current = res.data.RelTime;
                                                       delay = true;
                                                       if(!length){
                                                       setTimeout(function(){self.click()}, TIME)
                                                       }
                                                       var val = (current / length).toFixed(2)
                                                       $('input[name="seek"]').val(val)
                                                       $('output.seek').val(val)
                                                       setTimeout(function(){delay = false}, 500)
                                                       }
                                                       log(`info result: ${JSON.stringify(res)}`)
                                                       }, function(err){
                                                       log(`info error: ${JSON.stringify(err)}`)
                                                       })
                                     playing = !playing;
                                     })
  
  
  // getSysVolume
  $('button.btn.getSysVolume').click(function(){
                                     if(!inited || !host) {return}
                                     dlna.getSysVolume(host, function(res){
                                                       if(res.code === 0){
                                                       var volume = res.data.CurrentVolume;
                                                       delay = true;
                                                       $('input[name="volume"]').val(volume)
                                                       $('output.volume').val(volume)
                                                       setTimeout(function(){delay = false}, 500)
                                                       }
                                                       log(`getVolume result: ${JSON.stringify(res)}`)
                                                       }, function(err){
                                                       log(`getVolume error: ${JSON.stringify(err)}`)
                                                       })
                                     })
  
  $('button.btn.stop').click(function(){
                             if(!inited || !host) {return}
                             dlna.stop(host, function(res){
                                       log(`stop result: ${JSON.stringify(res)}`)
                                       }, function(err){
                                       log(`stop error: ${JSON.stringify(err)}`)
                                       })
                             $('#root li').removeClass('active')
                             $('#machine').empty()
                             log('------- Stop --------')
                             host = null;
                             })
  
  // volume
  $('input[name="volume"]').change(function(){
                                   var val = $(this).val()
                                   //val = parseInt(val, 10)
                                   $('output.volume').val(val)
                                   if(host && !delay){
                                   log('------- set volume --------')
                                   log(`host:${host}, volume: ${val}`)
                                   dlna.setVolume(host, val, function(res){
                                                  log(`setVolume result: ${JSON.stringify(res)}`)
                                                  }, function(err){
                                                  log(`setVolume error: ${JSON.stringify(err)}`)
                                                  })
                                   }
                                   })
  
  $('input[name="seek"]').change(function(){
                                 var val = $(this).val()
                                 val = parseInt(val, 10)
                                 $('output.seek').val(val)
                                 if (host && !delay){
                                 var seek = (val / 100 * length).toFixed(0)
                                 log('------- seek --------')
                                 log(`host:${host}, seek: ${seek}`)
                                 dlna.seek(host, seek, function(res){
                                           log(`seek result: ${JSON.stringify(res)}`)
                                           }, function(err){
                                           log(`seek error: ${JSON.stringify(err)}`)
                                           })
                                 }
                                 })
  
  document.addEventListener('deviceready', (e) => {
                            dlna && dlna.init();
                            log('------- Inited --------')
                            inited = true;
                            $('button.btn.sacn').click()
                            }, false);
  }());
