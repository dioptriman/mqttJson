const mqtt = require("mqtt");

const clientId = `server_${Math.random().toString(16).slice(3)}`;

const client = mqtt.connect("mqtt://localhost:1883", {
  clientId,
  username: "guest",
  password: "guest",
});

const topic = "esp/test";

client.on("connect", () => {
  console.log("Connected");

  client.subscribe([topic], () => {
    console.log(`Subscribe to topic '${topic}'`);
    client.publish(
      topic,
      "nodejs mqtt test",
      { qos: 0, retain: false },
      (error) => {
        if (error) {
          console.error(error);
        }
      }
    );
  });
});

client.on("message", (topic, payload) => {
  console.log("Received Message:", topic, payload.toString());
});
