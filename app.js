// Load dependencies
const express = require("express");
const app = express();
const handlebars = require("express-handlebars");
const port = process.env.PORT || 3000;
var mqtt = require("mqtt");
var http = require("http").Server(app);
var io = require("socket.io")(http);

// Set view engine to Handlebars
app.set("view engine", "hbs");

// Set view engine options
app.engine(
  "hbs",
  handlebars({
    layoutsDir: __dirname + "/views/layouts",
    extname: "hbs", // Change the file extension from .handlebars to .hbs
    defaultLayout: "index", // Set default layout should nothing specific be targeted during render
    partialsDir: __dirname + "/views/partials"
  })
);

app.use(express.static("public"));
app.use("/", require("./routes"));


/* ARDUINO CODE--------------------------------------------------------------------------*/

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

// Initialise server side socketio
io.on("connection", function(client) {
  console.log("Socket Client connected...");
  // Triggered from submit message button
  client.on("sendMsg", function(data) {
    console.log(data);
    // Publish custom message to MQTT
    mqttClient.publish("test", data);
  });
});
/* END OF ARDUINO -----------------------------------------------------------------------*/

http.listen(port, () => {
  console.log(`App listening on port ${port}`);
  // console.log(`Attempting to connect to DB...`);
});
