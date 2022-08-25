from flask import Flask, Response
import cv2
import pyaudio

app = Flask(__name__)
video1 = cv2.VideoCapture(0)
video2 = cv2.VideoCapture(2)

video1.release()
video2.release()

FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 44100
CHUNK = 1024

audio1 = pyaudio.PyAudio()

global stream
stream = None

@app.route('/')
def index():
    return 'Hello world!'

def genHeader(sampleRate, bitsPerSample, channels):
    datasize = 2000*10**6
    o = bytes("RIFF", 'ascii')
    o += (datasize + 36).to_bytes(4, 'little')
    o += bytes("WAVE", 'ascii')
    o += bytes("fmt", 'ascii')
    o += (16).to_bytes(4, 'little')
    o += (1).to_bytes(2, 'little')
    o += (channels).to_bytes(2, 'little')
    o += (sampleRate).to_bytes(4, 'little')
    o += (sampleRate * channels * bitsPerSample // 8).to_bytes(4, 'little')
    o += (channels + bitsPerSample // 8).to_bytes(2, 'little')
    o += (bitsPerSample).to_bytes(2, 'little')
    o += bytes("data", 'ascii')
    o += (datasize).to_bytes(4, 'little')
    return o


@app.route('/audio')
def audio():
    def sound():
        global stream
        if stream != None:
            stream.stop_stream()
            stream.close()
        CHUNK = 1024
        sampleRate = 44100
        bitsPerSample = 16
        channels = 2
        wav_header = genHeader(sampleRate, bitsPerSample, channels)
        stream = audio1.open(format=FORMAT, channels=CHANNELS,
                            rate=RATE, input=True, input_device_index=1,
                            frames_per_buffer=CHUNK)
        first_run = True
        while True:
            if first_run:
                data = wav_header + stream.read(CHUNK, exception_on_overflow = False)
                first_run = False
            else:
                data = stream.read(CHUNK, exception_on_overflow = False)
            yield(data)
    return Response(sound())


def gen(video):
    while True:
        success, image = video.read()
        ret, jpeg = cv2.imencode('.jpg', image)
        frame = jpeg.tobytes()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')


@app.route('/video_feed1')
def video_feed1():
    video2.release()
    video1 = cv2.VideoCapture(0)
    video1.set(3, 640)
    video1.set(4, 480)
    global video
    return Response(gen(video1), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/video_feed2')
def video_feed2():
    video1.release()
    video2 = cv2.VideoCapture(2)
    video2.set(3, 640)
    video2.set(4, 480)
    global video
    return Response(gen(video2), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.errorhandler(404)
def nfound(e):
    print(e)
    return "404-"+str(e)

if __name__ == '__main__':
    app.run(debug=False, host="192.168.1.100", port=5000, threaded=True)