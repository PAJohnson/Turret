#web interface for turret

import os
import json
import sys
from flask import Flask, render_template, request, url_for, redirect
#import turret as robot

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
    #robot.moveRelative((content["base"],content["shaft"],content["time"]))
    return redirect(url_for('homePage'))

if __name__ == '__main__':
    app.run()
