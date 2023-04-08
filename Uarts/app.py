# app.py

from flask import Flask, jsonify
from flask_socketio import SocketIO, emit

app = Flask(__name__, static_folder='flask_react/build', static_url_path='/')
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app)


@app.route('/')
def index():
    print('Enter index page')
    return app.send_static_file('index.html')


@socketio.on('connect')
def test_connect():
    print('Client connected')


@socketio.on('disconnect')
def test_disconnect():
    print('Client disconnected')


@app.route('/set')
def cup_set():
    print('set')
    socketio.emit('data', {'message': 'CUP SET'})
    return 'Cup set'

@app.route('/lift')
def cup_lift():
    print('lift')
    socketio.emit('data', {'message': 'CUP LIFT'})
    return 'Cup lifted'
