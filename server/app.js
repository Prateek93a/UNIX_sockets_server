const express = require('express');
const app = express();
const net = require('net');

const socketConnections = [];
const server = net.createConnection('',(connection) => {
    socketConnections.push(connection);
    connection.write('Hello world');
});

server.listen({
    host: 'localhost',
    port: 3000,
    exclusive: true
  }, () => {
    console.log('Socket Port Started');
})

app.listen(5555, () => console.log('server started'));

app.get('/', (req,res) => {
    console.log('Request came!');
    res.send("Hello World!!");
})