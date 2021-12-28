const express = require('express');
const app = express();
const socket = require('socket.io');
const PORT = process.env.PORT || 3000;

const server = app.listen(PORT, () =>{
    console.log('server running..');
})

const io = socket(server);

io.on('connection', (socket) => {
    console.log('Nodemcu Bağlandı' + socket.id);
    socket.on('message', function(message){
        app.get('/', (req,res,message) => {
            res.send(message);
        });
        console.log(message);
    })
})
