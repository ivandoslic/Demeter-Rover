var activeCam = 0;

function helloEsp() {
    try {
        fetch('http://192.168.1.18:5000/helloEsp');
    } catch (e) {
        console.error(e);
    }
}

function goForward() {
    try {
        fetch('http://192.168.1.18:5000/forward').then(response => {
            if (!response.ok) {
                console.error(response.status);
            }
        });
    } catch (e) {
        console.error(e);
    }
}

function goBackwards() {
    try {
        fetch('http://192.168.1.18:5000/backwards').then(response => {
            if (!response.ok) {
                console.error(response.status);
            }
        });
    } catch (e) {
        console.error(e);
    }
}

function turnLeft() {
    try {
        fetch('http://192.168.1.18:5000/left').then(response => {
            if (!response.ok) {
                console.error(response.status);
            }
        });
    } catch (e) {
        console.error(e);
    }
}

function turnRight() {
    try {
        fetch('http://192.168.1.18:5000/right').then(response => {
            if (!response.ok) {
                console.error(response.status);
            }
        });
    } catch (e) {
        console.error(e);
    }
}

function stopMovement() {
    try {
        fetch('http://192.168.1.18:5000/stop').then(response => {
            if (!response.ok) {
                console.error(response.status);
            }
        });
    } catch (e) {
        console.error(e);
    }
}

function turretLeft() {
    try {
        fetch('http://192.168.1.18:5000/turretLeft').then(response => {
            if (!response.ok) {
                console.error(response.status);
            }
        });
    } catch (e) {
        console.error(e);
    }
}

function turretRight() {
    try {
        fetch('http://192.168.1.18:5000/turretRight').then(response => {
            if (!response.ok) {
                console.error(response.status);
            }
        });
    } catch (e) {
        console.error(e);
    }
}

function buttonAPressed(buttonID) {
    try {
        fetch('http://192.168.1.18:5000/buttonA').then(response => {
            if (!response.ok) {
                console.error(response.status);
            }
        });
    } catch (e) {
        console.error(e);
    }
}

function buttonBPressed(buttonID) {
    try {
        fetch('http://192.168.1.18:5000/buttonB').then(response => {
            if (!response.ok) {
                console.error(response.status);
            }
        });
    } catch (e) {
        console.error(e);
    }
}

function buttonCPressed(buttonID) {
    try {
        fetch('http://192.168.1.18:5000/buttonC').then(response => {
            if (!response.ok) {
                console.error(response.status);
            }
        });
    } catch (e) {
        console.error(e);
    }
}

function buttonDPressed(buttonID) {
    try {
        fetch('http://192.168.1.18:5000/buttonD').then(response => {
            if (!response.ok) {
                console.error(response.status);
            }
        });
    } catch (e) {
        console.error(e);
    }
}

function buttonEPressed(buttonID) {
    try {
        fetch('http://192.168.1.18:5000/buttonE').then(response => {
            if (!response.ok) {
                console.error(response.status);
            }
        });
    } catch (e) {
        console.error(e);
    }
}

function buttonFPressed(buttonID) {
    try {
        fetch('http://192.168.1.18:5000/buttonF').then(response => {
            if (!response.ok) {
                console.error(response.status);
            }
        });
    } catch (e) {
        console.error(e);
    }
}

function buttonReleased() {
    try {
        fetch('http://192.168.1.18:5000/buttonR').then(response => {
            if (!response.ok) {
                console.error(response.status);
            }
        });
    } catch (e) {
        console.error(e);
    }
}

function changeCam() {
    var frame = document.getElementById('streamFrame');
    if (activeCam === 0) {
        activeCam = 1;
        frame.src = 'http://127.0.0.1:5000/video_feed2'
    } else {
        activeCam = 0;
        frame.src = 'http://127.0.0.1:5000/video_feed1'
    }
}