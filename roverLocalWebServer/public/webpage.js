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

function buttonPressed(buttonID) {
    try {
        fetch(('http://192.168.1.18:5000/button' + buttonID)).then(response => {
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