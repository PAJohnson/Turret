import time
import pigpio
import math
import numpy as np

#mechanical constants
#command degrees

BASE_TEETH = 50
BASE_STEP_TEETH = 10
SHAFT_TEETH = 20
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

def p_to_p(start, end, _time):
	#start is (base, shaft) in deg
	#end is (base, shaft) in deg
	#cubic trajectory generator
	#returns polynomial coefficients

	#traj base
	qi_b = start[0]
	qi_dot_b = 0
	qf_b = end[0]
	qf_dot_b = 0
	tf = _time

	A_base = np.array([[1,0,0,0],[0,1,0,0],[1,tf,tf**2,tf**3],[0,1,2*tf,3*tf**2]])
	B_base = np.array([[qi_b],[qi_dot_b],[qf_b],[qf_dot_b]])
	base_coeff = np.linalg.solve(A_base,B_base)

	#traj shaft
	qi_s = start[1]
	qi_dot_s = 0
	qf_s = end[1]
	qf_dot_s = 0

	A_shaft = A_base
	B_shaft = np.array([[qi_s],[qi_dot_s],[qf_s],[qf_dot_s]])
	shaft_coeff = np.linalg.solve(A_shaft, B_shaft)

	return (base_coeff,shaft_coeff)

def traj_gen(coeffs,_time):
	#coeffs is two arrays
	#to get a0 for the base, coeffs[0][0]
	#to get a3 for shaft, coeffs[1][3]
	#velocity polynomial is 3*a_3*t^2 + 2*a_2*t + a_1

	base_speed = []
	shaft_speed = []
	for _t in np.arange(0,_time+T_STEP,T_STEP):
		_base =  3*coeffs[0][3]*_t**2 + 2*coeffs[0][2]*_t + coeffs[0][1]
		_shaft = 3*coeffs[1][3]*_t**2 + 2*coeffs[1][2]*_t + coeffs[1][1]
		base_speed.append(_base/BASE_DEG_PER_STEP)
		shaft_speed.append(_shaft/SHAFT_DEG_PER_STEP)

	for i in range(len(base_speed)):
		base_speed_set(base_speed[i])
		shaft_speed_set(shaft_speed[i])
		time.sleep(T_STEP)

def points_parse(waypoints):
	#waypoints in [(base position, shaft position, time)]
	#return coefficients for each interpolating polynomial between points
	#return [base_coeffs, shaft_coeffs]
	q_dot_s = []
	q_dot_b = []
	q_dot_s.append(0)
	q_dot_b.append(0)
	for i in range(1,len(waypoints)-1):
		#calculate intermediate velocities
		vk_s = (waypoints[i][1]-waypoints[i-1][1]) \
			/(waypoints[i][2]-waypoints[i-1][2])

		vk_1_s = (waypoints[i+1][1]-waypoints[i][1]) \
			/(waypoints[i+1][2]-waypoints[i][2])

		vk_b = (waypoints[i][0]-waypoints[i-1][0]) \
			/(waypoints[i][2]-waypoints[i-1][2])

		vk_1_b = (waypoints[i+1][0]-waypoints[i][0]) \
			/(waypoints[i+1][2]-waypoints[i][2])

		if sign(vk_s) == sign(vk_1_s):
			q_dot_s.append((vk_1_s+vk_s)/2)
		else:
			q_dot_s.append(0)
		if sign(vk_b) == sign(vk_1_b):
			q_dot_b.append((vk_1_b+vk_b)/2)
		else:
			q_dot_b.append(0)

	q_dot_s.append(0)
	q_dot_b.append(0)

	base_coeffs = []
	shaft_coeffs = []

	#now we have positions, velocities, and times for each path point
	#compute coefficients
	for i in range(len(waypoints)-1):

		#calculate a3...a0 for the cubic polynomial for each pair of waypoints
		#base
		qi_b = waypoints[i][0]
		qi_dot_b = q_dot_b[i]
		qf_b = waypoints[i+1][0]
		qf_dot_b = q_dot_b[i+1]
		tf = waypoints[i+1][2]-waypoints[i][2]

		A_base = np.array([[1,0,0,0],[0,1,0,0],[1,tf,tf**2,tf**3],[0,1,2*tf,3*tf**2]])
		B_base = np.array([[qi_b],[qi_dot_b],[qf_b],[qf_dot_b]])
		base_coeffs.append(np.linalg.solve(A_base,B_base))

		#shaft
		qi_s = waypoints[i][1]
		qi_dot_s = q_dot_s[i]
		qf_s = waypoints[i+1][1]
		qf_dot_s = q_dot_s[i+1]
		tf = waypoints[i+1][2]-waypoints[i][2]

		A_shaft = np.array([[1,0,0,0],[0,1,0,0],[1,tf,tf**2,tf**3],[0,1,2*tf,3*tf**2]])
		B_shaft = np.array([[qi_s],[qi_dot_s],[qf_s],[qf_dot_s]])
		shaft_coeffs.append(np.linalg.solve(A_shaft,B_shaft))

	times = []
	for i in range(len(waypoints)-1):
		times.append(waypoints[i+1][2]-waypoints[i][2])
	return [base_coeffs,shaft_coeffs,times]

def compute_speeds(coeffs_times):

	base_speed = []
	shaft_speed = []
	for i in range(len(coeffs_times[0])):
		for _t in np.arange(0,coeffs_times[2][i],T_STEP):
			_base =  3*coeffs_times[0][i][3]*_t**2 + \
				2*coeffs_times[0][i][2]*_t + \
				coeffs_times[0][i][1]
			_shaft = 3*coeffs_times[1][i][3]*_t**2 + \
				2*coeffs_times[1][i][2]*_t + \
				coeffs_times[1][i][1]
			base_speed.append(_base/BASE_DEG_PER_STEP)
			shaft_speed.append(_shaft/SHAFT_DEG_PER_STEP)

	base_speed.append(0)
	shaft_speed.append(0)
	return [base_speed,shaft_speed]

def execute_traj(speeds):
	for i in range(len(speeds[0])):
		shaft_speed_set(speeds[1][i])
		base_speed_set(speeds[0][i])
		time.sleep(T_STEP)

def sign(number):
	if number > 0:
		return 1
	elif number < 0:
		return -1
	else:
		return 0

def test():
	for i in range(1):
		traj_gen(p_to_p((0,0),(30,30),1),1)
		traj_gen(p_to_p((30,30),(-30,30),1),1)
		traj_gen(p_to_p((-30,30),(0,0),1),1)

#test waypoints
waypoints = [(0,0,0),(10,30,0.5),(30,0,1),(5,-30,1.5),(0,0,2)]

polys = points_parse(waypoints)
speeds = compute_speeds(polys)
execute_traj(speeds)
