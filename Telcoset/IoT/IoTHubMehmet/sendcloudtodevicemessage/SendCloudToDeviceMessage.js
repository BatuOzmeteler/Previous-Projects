'use strict';

var Client = require('azure-iothub').Client;
var Message = require('azure-iot-common').Message;

var connectionString = 'HostName=IoTHubMehmet.azure-devices.net;SharedAccessKeyName=iothubowner;SharedAccessKey=vneK/269RV3Tk3vhTSl8AbIIc2ijnVBqjcLIZKMhOks=';
var targetDevice = 'myDeviceID97';

var serviceClient = Client.fromConnectionString(connectionString);

//to print operation results to the console
function printResultFor(op) {
  return function printResult(err, res) {
    if (err) console.log(op + ' error: ' + err.toString());
    if (res) console.log(op + ' status: ' + res.constructor.name);
  };
}
//to print delivery feedback messages to the console
function receiveFeedback(err, receiver){
  receiver.on('message', function (msg) {
    console.log('Feedback message:')
    console.log(msg.getData().toString('utf-8'));
  });
}
//to send a message to your device and handle the feedback message when the device acknowledges the cloud-to-device message
serviceClient.open(function (err) {
  if (err) {
    console.error('Could not connect: ' + err.message);
  } else {
    console.log('Service client connected');
    serviceClient.getFeedbackReceiver(receiveFeedback);
    var message = new Message('It is getting hot in here!');
    message.ack = 'full';
    message.messageId = "My Message ID";
    console.log('Sending message: ' + message.getData());
    serviceClient.send(targetDevice, message, printResultFor('send'));
  }
});