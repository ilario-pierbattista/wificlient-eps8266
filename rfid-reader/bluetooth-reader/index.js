var btSerial = new (require('bluetooth-serial-port')).BluetoothSerialPort();
var exit = require('exit');

const BT_ADDRESS = "20-13-07-19-14-98";

var global_buffer = "";

btSerial.on('found', function(address,name){
  console.log("Nome: "+name+" - "+"Indirizzo: "+address);

  if(address == BT_ADDRESS) {
    console.log("Device found! \n");

    btSerial.findSerialPortChannel(address, function(channel) {
      console.log("Serial channel: "+channel);

      btSerial.connect(address, channel, function(){
        console.log("Connected!");

        btSerial.on('data', function(buffer) {
          var piece = buffer.toString("utf-8");
          global_buffer += piece.replace(/(\r\n|\n\r|\n|\r)/gm,"");
          if(piece.indexOf(";") > -1) {
            console.log(global_buffer);

            //@TODO inviare la richiesta al server

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
