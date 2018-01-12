`use strict`
var zmq = require('zmq')
  , sock = zmq.socket('sub');

sock.connect('tcp://127.0.0.1:10000');
sock.subscribe('user2');
console.log('Subscriber connected to port 10000');

sock.on('message', function() {
  console.log('message incoming');
  msg = []
  Array.prototype.slice.call(arguments).forEach(function(arg) {
    msg.push(arg.toString());
  });
  console.log(msg);
});
