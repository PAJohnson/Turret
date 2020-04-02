#web interface for turret

import os
import json
import sys
from flask import Flask, render_template, request, url_for, redirect
import trajectoryPlanner as tp

config = str(sys.argv[1])

if config == "rpi":
	import turret as robot
else:
	pass
    
app = Flask(__name__)

@app.route('/index.html')
def homePage():
    return render_template('index.html')
    
@app.route('/')
def return_home():
	return redirect(url_for('homePage'))

@app.route('/move/relative.json', methods = ['GET', 'POST'])
def move_relative():
    content = request.json
    print((content["base"],content["shaft"],content["time"]))
    if config == "rpi":
        robot.moveRelative((content["base"],content["shaft"],content["time"]))
    
    return redirect(url_for('homePage'))
    
@app.route('/test.html')
def testPage():
    return render_template('test.html')
    
@app.route('/program.html')
def program():
    return render_template('program.html')
    
@app.route('/jog.html')
def jog():
    return render_template('jog.html')
    
@app.route('/settings.html')
def settings():
    return render_template('settings.html')
    
@app.route('/graphs.html')
def graphs():
    return render_template('graphs.html')
    
@app.route('/submit', methods=['POST'])
def submit():
    # store the given text in a variable
    text = request.form.get("text")

    # split the text to get each line in a list
    text2 = text.split('\n')
    newWP = tp.wayPoints()
    
    #convert to list of tuples
    for row in text2:
        #strip whitespace
        newRow = [float(val) for val in row.rstrip().split(',')]
        newWP.addWP(tuple(newRow))
        
    if config == "rpi":
        robot.wpToRun(newWP)

    return redirect(url_for('graphs'))

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
