# This is the script you need to run the first time you download raspberry pi video stream server on your raspberry pi,
# it is going to check if you have all the backend stuff you need to run this server, there is no need to mess with this code
echo Checking is python installed...
type -P python >/dev/null 2>&1 && echo Python is installed
echo PIP version:
pip --version
echo Checking PIP packages:
echo Server/Flask:
pip show flask
echo Video:
pip show opencv-python
echo Sound:
pip show pyaudio
echo Checking Linux packages:
echo Portaudio package:



# ↓ Add this ↓
sudo apt install portaudio19-dev