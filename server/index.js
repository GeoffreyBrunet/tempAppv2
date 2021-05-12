// requires
const express = require('express')
const SerialPort = require('serialport')
const http = require('http')
const socketIo = require('socket.io')()

// const for web server
const app = express()
const server = http.createServer(app)
const io = socketIo.listen(server)
const port = 3000

// const for serialport
const Readline = SerialPort.parsers.Readline
const portSerial = new SerialPort('/dev/ttyACM0', {
    baudRate: 9600
})
const parser = portSerial.pipe(new Readline({ delimiter: '\r\n' }))

// var for status of led
let statusLed = ''

// var for status of led
let statusRadiator = ''

io.on('connection', function (socket) {
    console.log('new socket connected')
})

// start parser for serialport
portSerial.on('ready', function () {
    console.log('the ready byte sequence has been received')
})

// parse data from arduino and send it through socket.io
parser.on('data', data => {
    const dataString = data.toString()
    const arr = dataString.split(' ')
    const getHumidity = arr[0].toString()
    const getTemperature = arr[1].toString()
    statusLed = arr[2].toString()
    statusRadiator = arr[3].toString()
    io.emit('arduino:data', {
        humidity: getHumidity,
        temperature: getTemperature,
        ledStatus: statusLed,
        radiatorStatus: statusRadiator
    })
})

portSerial.on('err', function (err) {
    console.log(err.message)
})

// default route for html index page
app.get('/', (req, res, next) => {
    res.sendFile(__dirname + '/index.html')
})

// Route for get radiator status
app.get('/getradiatorstatus', function (req, res) {
    let radiadorResponse = { "radiatorStatus": statusRadiator }
    res.status(200).json(radiadorResponse)
})

// Route for change to auto radiator status
app.get('/changetoauto', function (req, res) {
    portSerial.write('Auto')
    let autoResponse = { "autoResponse": "Changed to auto mode" }
    res.status(200).json(autoResponse)
})

// Route for change to manual radiator status
app.get('/changetomanual', function (req, res) {
    portSerial.write('Manual')
    let manualResponse = { "manualResponse": "Changed to manual mode" }
    res.status(200).json(manualResponse)
})

// start express.js server
server.listen(port, "0.0.0.0", () => {
    console.log('server on port:', port)
})