:: This is a script file that helps you run rover controler server on your PC
@ECHO OFF
ECHO Running Demeter rover server
:: CHECK NODE VERSION:
ECHO Node version:
call node --version
:: CHECK NPM VERSION:
ECHO NPM version:
call npm -v
:: CHECK DEPENDENCIES IF NOT INSTALLED INSTALLS THEM
ECHO Checking dependencies...
call npm list express || call npm install express
call npm list express-ws || call npm install express-ws
:: START SERVER AND LOAD IT IN BROWSER
ECHO Starting server...
start "" http://localhost:5000/
ECHO Use CTRL + C to shut the server down
call node ./index.js
PAUSE