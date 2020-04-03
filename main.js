const express = require('express')
const app = express()
const exphbs = require('express-handlebars');
var mqtt = require("mqtt");
var http = require("http").Server(app);
var io = require("socket.io")(http);

app.engine('handlebars', exphbs());
app.set('view engine', 'handlebars');

//Allows handlebars to find my stylesheet
app.use(express.static(__dirname + '/public'));

app.get('/', function (req, res) {
    res.render('home');
});

//Setup variables for the MQTT communication
var MQTT_TOPIC = "/test";
//MQTT Token
var MQTT_ADDR = "mqtt://763f6861:1362154b898a179a@broker.shiftr.io";
//Shiftr's port
var MQTT_PORT = 1883;
//Connect the client to the broker's address
var mqttClient = mqtt.connect(MQTT_ADDR);

//When the MQTT client is connected, subscribe to the topic
mqttClient.on("connect", function() {
  console.log("Connected to MQTT client");
  //Subscribe to the MQTT topic
  mqttClient.subscribe(MQTT_TOPIC, { qos: 1 });
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
    console.log(`Our app is running on port ${ PORT }`);
});
