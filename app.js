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

  // Set default file route
  app.use(express.static("public"));
  app.use("/", require("./routes"));


  /* ARDUINO CODE--------------------------------------------------------------------------*/

  //Setup variables for the MQTT communication
  // Topic where location data will be coming in
  var MQTT_TOPIC = "/location";
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
      // console.log(data); //Uncomment to see device message in console
      // Publish custom message to MQTT
      mqttClient.publish("messages", data);
    });

  // Publish device customisations to mqtt
    client.on("redFont", function(data) {
     mqttClient.publish("custom/font/red");
   });
   client.on("orangeFont", function(data) {
    mqttClient.publish("custom/font/orange");
  });
  client.on("yellowFont", function(data) {
   mqttClient.publish("custom/font/yellow");
  });
  client.on("greenFont", function(data) {
   mqttClient.publish("custom/font/green");
  });
  client.on("blueFont", function(data) {
   mqttClient.publish("custom/font/blue");
  });
  client.on("pinkFont", function(data) {
   mqttClient.publish("custom/font/pink");
  });
  client.on("purpleFont", function(data) {
   mqttClient.publish("custom/font/purple");
  });
  client.on("blackFont", function(data) {
   mqttClient.publish("custom/font/black");
  });
  client.on("whiteFont", function(data) {
   mqttClient.publish("custom/font/white");
  });

  client.on("redBg", function(data) {
   mqttClient.publish("custom/background/red");
  });
  client.on("orangeBg", function(data) {
  mqttClient.publish("custom/background/orange");
  });
  client.on("yellowBg", function(data) {
  mqttClient.publish("custom/background/yellow");
  });
  client.on("greenBg", function(data) {
  mqttClient.publish("custom/background/green");
  });
  client.on("blueBg", function(data) {
  mqttClient.publish("custom/background/blue");
  });
  client.on("pinkBg", function(data) {
  mqttClient.publish("custom/background/pink");
  });
  client.on("purpleBg", function(data) {
  mqttClient.publish("custom/background/purple");
  });
  client.on("blackBg", function(data) {
  mqttClient.publish("custom/background/black");
  });
  client.on("whiteBg", function(data) {
  mqttClient.publish("custom/background/white");
  });

  });

  // On message from location topic, convert the JSON data recieved into variables
  mqttClient.on("message", function(topic, message) {
    var jsonObj = JSON.parse(message);
    latVal = jsonObj.Latitude;
    lonVal = jsonObj.Longitude;
    // console.log("Latitude:" + latVal + "Longitude:" + lonVal); //Uncomment to see location data in console
    // Send to client side to be input to google maps api
        io.emit("updateLocation", latVal, lonVal);
  });


  /* END OF ARDUINO -----------------------------------------------------------------------*/

  http.listen(port, () => {
    console.log(`App listening on port ${port}`);
    // console.log(`Attempting to connect to DB...`);
  });
