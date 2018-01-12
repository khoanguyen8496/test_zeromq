// pubber.js
var zmq = require('zmq')
  , sock = zmq.socket('pub');

sock.bindSync('tcp://127.0.0.1:10000');
console.log('Publisher bound to port 10000');

setInterval(function(){
  sock.send(['user1', 'hello!']);
}, 500);

setInterval(function() {
  sock.send(['user2', 'hello!', 'bye']);
}, 500);
