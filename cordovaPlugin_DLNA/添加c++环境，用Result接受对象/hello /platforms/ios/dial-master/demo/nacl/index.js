
var dial = new DialModule('pnacl/Release', function() {
	document.getElementById('panel').style.display = 'block';
});

var is_scan = false;
var device = document.getElementById('device');
var button = document.getElementById('button');

button.onclick = function() {

	if ( is_scan ) return;

	device.innerHTML = ' <li> Scan device ... </li> ';

	is_scan = true;

	dial.scan(function (err, data) {
		is_scan = false;

		if (err) {
			console.error(err);
			device.innerHTML = ' <li> ' + err + ' </li> ';
		} else {
			if ( data.length == 0 ) {
				device.innerHTML = ' <li> No device </li> ';
			} else {
				device.innerHTML = '';
			}

			data.forEach(function(item) {
				var li = document.createElement('li');

				li.innerHTML = '[' + item.friendly_name + ']' + 
				' <a href="#"> Launch Netflix </a> | <a href="#"> Stop Netflix </a> | ' +
				' <a href="#"> Launch YouTube </a> | <a href="#"> Stop YouTube </a> ' ;

				device.appendChild(li);

				var ls = li.querySelectorAll('a');

				ls[0].onclick = function () { // Launch Netflix
					dial.launch(item.host, 'Netflix', '');
				}
				ls[1].onclick = function () { // Stop Netflix
					dial.stop(item.host, 'Netflix');
				}
				ls[2].onclick = function () { // Launch YouTube
					dial.launch(item.host, 'YouTube', '');
				}
				ls[3].onclick = function () { // Stop YouTube
					dial.stop(item.host, 'YouTube');
				}

			});
		}
	});
}
