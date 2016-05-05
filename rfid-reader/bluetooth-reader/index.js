var btSerial = new (require('bluetooth-serial-port')).BluetoothSerialPort();
var exit = require('exit');
var rest = require('restler');
var colors = require('colors');


const BT_ADDRESS = '20-13-07-19-14-98';
const HOST = 'http://192.168.0.2';
const API_GET_REQUEST = '/camerino/rfid';

var global_buffer = "";

btSerial.on('found', function(address,name){
  //console.log("Nome: "+name+" - "+"Indirizzo: "+address);

  if(address == BT_ADDRESS) {
    console.log("Dispositivo trovato!");
    console.log("Connessione in corso...");

    btSerial.findSerialPortChannel(address, function(channel) {
      btSerial.connect(address, channel, function(){
        console.log("Connected!");

        btSerial.on('data', function(buffer) {
          var piece = buffer.toString("utf-8");
          global_buffer += piece.replace(/(\r\n|\n\r|\n|\r)/gm,"");
          if(piece.indexOf(";") > -1) {
            console.log(global_buffer.bold);

            //@TODO inviare la richiesta al server
            rest.get(HOST+API_GET_REQUEST).on('complete', function(result) {
              if (result instanceof Error) {
                console.log('Error:', result.message);
                this.retry(5000);
              } else {
                //var json = JSON.parse(result);
                if (result['true']) {
                  console.log("TRUE".bgGreen);
                } else{
                  console.log("FALSE".bgRed);
                }
              }
            });
            global_buffer = "";
          }
        });

      }, function() {
        console.log("Cannot connect!");
        exit(1);
      });
    });
  }
});
btSerial.inquire();
