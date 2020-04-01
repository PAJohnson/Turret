import time
import pigpio
import math
import trajectoryPlanner as tp

#mechanical constants
#command degrees

BASE_TEETH = 75
BASE_STEP_TEETH = 10
SHAFT_TEETH = 40
SHAFT_STEP_TEETH = 10
STEP_PER_REV = 3200

BASE_DEG_PER_STEP = (360.0)/(STEP_PER_REV*BASE_TEETH/BASE_STEP_TEETH)
SHAFT_DEG_PER_STEP = (360.0)/(STEP_PER_REV*SHAFT_TEETH/SHAFT_STEP_TEETH)

UP = 1
DOWN = 0

CW = 0
CCW = 1

DIR1 = 6
STEP1 = 13
DIR2 = 5
STEP2 = 18

T_STEP = 0.01

pi = pigpio.pi()

pi.set_mode(DIR1, pigpio.OUTPUT)
pi.set_mode(DIR2, pigpio.OUTPUT)
pi.set_mode(STEP1, pigpio.OUTPUT)
pi.set_mode(STEP2, pigpio.OUTPUT)

def move_shaft(distance, _time):
	# distance in degrees, time in seconds
	if(distance > 0):
		direction = UP
	elif(distance < 0):
		direction = DOWN

	speed = int(abs(distance/SHAFT_DEG_PER_STEP/_time))
	pi.write(DIR2, direction)
	pi.hardware_PWM(STEP2, speed, 500000)
	time.sleep(_time)
	pi.hardware_PWM(STEP2, 0, 500000)

def move_base(distance, _time):
	if(distance > 0):
		direction = CW
	elif(distance < 0):
		direction = CCW
	speed = int(abs(distance/BASE_DEG_PER_STEP/_time))
	print(speed)
	pi.write(DIR1, direction)
	pi.hardware_PWM(STEP1, speed, 500000)
	time.sleep(_time)
	pi.hardware_PWM(STEP1, 0, 500000)

def shaft_speed_set(speed):
	if(speed > 0):
		direction = UP
	elif(speed < 0):
		direction = DOWN
	else:
		direction = DOWN
	pi.write(DIR2, direction)
	pi.hardware_PWM(STEP2, int(abs(speed)), 500000)

def base_speed_set(speed):
	if(speed > 0):
		direction = CW
	elif(speed < 0):
		direction = CCW
	else:
		direction = CW
	pi.write(DIR1, direction)
	pi.hardware_PWM(STEP1, int(abs(speed)), 500000)

def sign(number):
	if number > 0:
		return 1
	elif number < 0:
		return -1
	else:
		return 0

def speed_change(waypoints,multiplier):
	#even change in speed for waypoints
	newPoints = []
	for i in range(len(waypoints)):
		newPoints.append((waypoints[i][0],waypoints[i][1],waypoints[i][2]*multiplier))

	return newPoints

def runOutputs(states,Ts):
	for i in range(len(states[0])):
		base_speed_set(states[0][i][1]/BASE_DEG_PER_STEP)
		shaft_speed_set(states[1][i][1]/SHAFT_DEG_PER_STEP)
		#print(states[0][i][1])
		time.sleep(Ts)

def csvToRun(filename, warp = 1.0, method = "cubic", Ts = 0.01):
	newWP = tp.wayPoints(filename)
	bot = tp.trajectoryPlanner(2)
	bot.waypointsParse(speed_change(newWP.waypoints,warp),method)
	bot.calcOutputs(Ts)
	runOutputs(bot.outputs,Ts)

def moveRelative(newPoint, method = "cubic", Ts = 0.01):
	waypoints = [(0,0,0),newPoint]
	bot = tp.trajectoryPlanner(2)
	bot.waypointsParse(waypoints,method)
	bot.calcOutputs(Ts)
	runOutputs(bot.outputs,Ts)

if __name__ == "__main__":
	wp = [(0,0,0), (30,30,1), (0,0,2), (30,30,3), (0,0,4), (30,30,5), (0,0,6),(0,0,6.1)]

	T = 0.01

	bot = tp.trajectoryPlanner(2)
	bot.waypointsParse(speed_change(wp,0.3),"quintic")
	bot.calcOutputs(T)
	runOutputs(bot.states,T)
