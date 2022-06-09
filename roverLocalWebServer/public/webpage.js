function helloEsp() {
    try {
        fetch('http://localhost:5000/helloEsp');
    } catch (e) {
        console.error(e);
    }
}

function goForward() {
    try {
        fetch('http://localhost:5000/forward').then(response => {
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
        fetch('http://localhost:5000/backwards').then(response => {
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
        fetch('http://localhost:5000/left').then(response => {
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
        fetch('http://localhost:5000/right').then(response => {
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
        fetch('http://localhost:5000/stop').then(response => {
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
        fetch('http://localhost:5000/turretLeft').then(response => {
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
        fetch('http://localhost:5000/turretRight').then(response => {
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
        fetch('http://localhost:5000/buttonA').then(response => {
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
        fetch('http://localhost:5000/buttonB').then(response => {
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
        fetch('http://localhost:5000/buttonC').then(response => {
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
        fetch('http://localhost:5000/buttonD').then(response => {
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
        fetch('http://localhost:5000/buttonE').then(response => {
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
        fetch('http://localhost:5000/buttonF').then(response => {
            if (!response.ok) {
                console.error(response.status);
            }
        });
    } catch (e) {
        console.error(e);
    }
}

function buttonPWR() {
    try {
        fetch('http://localhost:5000/buttonPWR').then(response => {
            if (!response.ok) {
                console.error(response.status);
            }
        });
    } catch (e) {
        console.error(e);
    }
}

function buttonReleased() {
    location.reload();
}

function turretStop() {
    try {
        fetch('http://localhost:5000/turretStop').then(response => {
            if (!response.ok) {
                console.error(response.status);
            }
        });
    } catch (e) {
        console.error(e);
    }
    setTimeout(() => {
        location.reload();
    }, 20);
}