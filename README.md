# esp32-TCPclient-to-WebSocket

In Arduino IDE, the esp32 streams a small json from a Web socket and parses it. The values will be used to program a motor in a robot.<br><br>

An issue I had was the node express Web socket was stringifying the json twice. I didn't realize that it "automatically" stringifies any json. So the deserialize function in the client still left me with a stringified json and did not throw an error, and would not yield the values. The stack and json questions are here:<br>
https://github.com/bblanchon/ArduinoJson/issues/1507
https://stackoverflow.com/questions/66283877/esp32-arduino-ide-web-socket-stream-json-no-errors-but-no-values

I recommend the Arduino Json Assistant and utilities for debugging<br>
https://arduinojson.org/v6/assistant/

Stream utils<br>
https://github.com/bblanchon/ArduinoStreamUtils<br>
https://arduinojson.org/v6/how-to/debug-data-coming-from-a-stream/

Here's my working socket code from the server.js file of my node express Web server<br><br>
```
// socket for esp32 to fetch projet data
var net = require('net');
var serverS = net.createServer(function(socket) {
	fs.readFile("./data/projet.json", 'utf-8', (err, data) => {
		if (err) {
			throw err;
		}
		socket.write(data); //<-- this used to be socket.write(JSON.stringify(data)) and didn't work
		socket.pipe(socket);
	});
	socket.on('end', () => {
		console.log('client disconnected');
	  });
});
serverS.on('error', (err) => {
	throw err;
  });
//console.log('socket listening on port...',1337);
serverS.listen(1337, () => {
	console.log('listening on port 1337...');
  });
```
