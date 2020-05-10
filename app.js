// Load dependencies
const express = require("express");
const app = express();
const handlebars = require("express-handlebars");
const port = process.env.PORT || 3000;
// const session = require("express-session");
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


/* ARDUINO --------------------------------------------------------------------------*/

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

/* Socket I/O send messages to control collar */
io.on("connection", function(client) {
  console.log("Socket Client connected...");
  client.on("sendRed", function(data) {
    mqttClient.publish("test");
  });
});

// Called after form input is processed
function startConnect() {
    // Generate a random client ID
    // clientID = "clientID-" + parseInt(Math.random() * 100);

    // Fetch the hostname/IP address and port number from the form
    host = document.getElementById("customMsg").value;
    // port = document.getElementById("port").value;

    // Print output for the user in the messages div
    // document.getElementById("messages").innerHTML += '<span>Sending Message...'</span><br/>';
    // document.getElementById("messages").innerHTML += '<span>Using the following client value: ' + clientID + '</span><br/>';



}

    // Initialize new Paho client connection
//     client = new Paho.MQTT.Client(host, Number(port), clientID);
//
//     // Set callback handlers
//     client.onConnectionLost = onConnectionLost;
//     client.onMessageArrived = onMessageArrived;
//
//     // Connect the client, if successful, call onConnect function
//     client.connect({
//         onSuccess: onConnect,
//     });
// }

// Called when the client connects
// function onConnect() {
//     // Fetch the MQTT topic from the form
//     topic = document.getElementById("topic").value;
//
//     // Print output for the user in the messages div
//     document.getElementById("messages").innerHTML += '<span>Subscribing to: ' + topic + '</span><br/>';
//
//     // Subscribe to the requested topic
//     client.subscribe(topic);
// }

// Called when the client loses its connection
// function onConnectionLost(responseObject) {
//     document.getElementById("messages").innerHTML += '<span>ERROR: Connection lost</span><br/>';
//     if (responseObject.errorCode !== 0) {
//         document.getElementById("messages").innerHTML += '<span>ERROR: ' + + responseObject.errorMessage + '</span><br/>';
//     }
// }

// // Called when a message arrives
// function onMessageArrived(message) {
//     console.log("onMessageArrived: " + message.payloadString);
//     document.getElementById("messages").innerHTML += '<span>Topic: ' + message.destinationName + '  | ' + message.payloadString + '</span><br/>';
// }

// // Called when the disconnection button is pressed
// function startDisconnect() {
//     client.disconnect();
//     document.getElementById("messages").innerHTML += '<span>Disconnected</span><br/>';
// }

/* END OF ARDUINO -----------------------------------------------------------------------*/

http.listen(port, () => {
  console.log(`App listening on port ${port}`);
  // console.log(`Attempting to connect to DB...`);
});
