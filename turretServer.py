#zmq server test
import time
import zmq
import json
import turret as robot

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:5555")

#message types
#message is [header, data] where header is in message types
#data is payload

headers = {"moveRel" : , 
           "loadProgram" : , 
           "executeProgram" : }

while True:
    #  Wait for next request from client
    message = json.loads(socket.recv_json())
    print("Received request: %s" % message)

    #  Do some 'work'
    if message[0] in headers:
        
    else:
        print("unknown command")

    #  Send reply back to client
    socket.send(b"Ok")
