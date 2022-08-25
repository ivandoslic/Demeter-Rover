const express = require('express');
const app = express();
const appWs = require('express-ws')(app);
const path = require('path');

let esp;
let rasp;
let pc;

app.use(express.static(path.join(__dirname, '/public')));

app.ws('/device', ws => {
    ws.on('message', msg => {
        if (msg == 'init-esp') {
            esp = ws;
            console.log("ESP connected!");
        } else if (msg == 'init-rasp') {
            rasp = ws;
        } else {
            console.log(msg);
        }
    });
});

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public/newInterface.html'));
});

app.get('/forward', (req, res) => {
    esp.send("f");
    res.send("Going forward");
});

app.get('/backwards', (req, res) => {
    esp.send("b");
    res.send("Going backwards");
});

app.get('/left', (req, res) => {
    esp.send("l");
    res.send("Going left");
});

app.get('/right', (req, res) => {
    esp.send("r");
    res.send("Going right");
});

app.get('/stop', (req, res) => {
    esp.send("s");
    res.send("Stopping");
});

app.get('/turretLeft', (req, res) => {
    esp.send("tl");
});

app.get('/turretRight', (req, res) => {
    esp.send("tr");
});

app.get('/buttonA', (req, res) => {
    esp.send("ba");
});

app.get('/buttonB', (req, res) => {
    esp.send("bb");
});

app.get('/buttonC', (req, res) => {
    esp.send("bc");
});

app.get('/buttonD', (req, res) => {
    esp.send("bd");
});

app.get('/buttonE', (req, res) => {
    esp.send("be");
});

app.get('/buttonF', (req, res) => {
    esp.send("bf");
});

app.get('/buttonR', (req, res) => {
    esp.send("br");
});

app.get('/buttonPWR', (req, res) => {
    esp.send("pwr");
});

app.get('/turretStop', (req, res) => {
    setTimeout(() => {
        esp.send("ts");
    }, 20);
});

app.listen(5000, () => console.log('Server started on port 5000!'));