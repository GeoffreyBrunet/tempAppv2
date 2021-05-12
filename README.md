# tempAppv2

## Description
This app is a real-time communication bewteen an Arduino (with DHT-22) and a web app.  
In counter folder, there is arduino source code.  
In server folder, there is a Node.js software who communicate with Arduino UNO by serial port, parse data received and send it in real-time to a web app. All components are in the same javascript app, who can be deployed on a Raspberry, and don't need big performances for work well.

## Libraries used
Library for communicate with serial port: [SerialPort](https://serialport.io/)

Web server for deserve HTML page and few routes: [Express.js](https://expressjs.com/)

Real-time communication bewteen SerialPort.js and Express.js: [Socket.io](https://socket.io/)

Chart librarie used for temperature's chart: [Chart.js](https://www.chartjs.org/)

All Libraries are added by npm, except Chart.js who is added by a CDN (hosted on cloudflare).