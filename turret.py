import time
import pigpio
import math
import trajectoryPlanner as tp
import queue

#mechanical constants
#command degrees
class Turret:
    def __init__(self):
        self.BASE_TEETH = 75
        self.BASE_STEP_TEETH = 10
        self.SHAFT_TEETH = 40
        self.SHAFT_STEP_TEETH = 10
        self.STEP_PER_REV = 3200

        self.BASE_DEG_PER_STEP = (360.0)/(self.STEP_PER_REV*self.BASE_TEETH/self.BASE_STEP_TEETH)
        self.SHAFT_DEG_PER_STEP = (360.0)/(self.STEP_PER_REV*self.SHAFT_TEETH/self.SHAFT_STEP_TEETH)

        self.UP = 1
        self.DOWN = 0

        self.CW = 0
        self.CCW = 1

        self.DIR1 = 6
        self.STEP1 = 13
        self.DIR2 = 5
        self.STEP2 = 18

        self.pi = pigpio.pi()

        self.pi.set_mode(self.DIR1, pigpio.OUTPUT)
        self.pi.set_mode(self.DIR2, pigpio.OUTPUT)
        self.pi.set_mode(self.STEP1, pigpio.OUTPUT)
        self.pi.set_mode(self.STEP2, pigpio.OUTPUT)
        
        self.bot = tp.trajectoryPlanner(2)
        self.moveRelQ = queue.Queue()
        self.runLock = False
        self.programs = {}

    def shaft_speed_set(self, speed):
        if(speed > 0):
            direction = self.UP
        elif(speed < 0):
            direction = self.DOWN
        else:
            direction = self.DOWN
        self.pi.write(self.DIR2, direction)
        self.pi.hardware_PWM(self.STEP2, int(abs(speed)), 500000)

    def base_speed_set(self, speed):
        if(speed > 0):
            direction = self.CW
        elif(speed < 0):
            direction = self.CCW
        else:
            direction = self.CW
        self.pi.write(self.DIR1, direction)
        self.pi.hardware_PWM(self.STEP1, int(abs(speed)), 500000)

    def sign(self, number):
        if number > 0:
            return 1
        elif number < 0:
            return -1
        else:
            return 0

    def speed_change(self, waypoints, multiplier):
        #even change in speed for waypoints
        newPoints = []
        for i in range(len(waypoints)):
            newPoints.append((waypoints[i][0],waypoints[i][1],waypoints[i][2]*multiplier))

        return newPoints

    def runOutputs(self, states, Ts):
        self.runLock = True
        for i in range(len(states[0])):
            self.base_speed_set(states[0][i][1]/self.BASE_DEG_PER_STEP)
            self.shaft_speed_set(states[1][i][1]/self.SHAFT_DEG_PER_STEP)
            #print(states[0][i][1])
            time.sleep(Ts)
        self.runLock = False

    def csvToRun(self, filename, warp = 1.0, method = "cubic", Ts = 0.01):
        newWP = tp.wayPoints(filename)
        self.wpToRun(newWP,warp,method,Ts)
        
    def wpToRun(self, wp, warp = 1.0, method = "cubic", Ts = 0.01):
        self.bot.waypointsParse(self.speed_change(wp.waypoints,warp),method)
        self.bot.calcOutputs(Ts)
        self.runOutputs(self.bot.outputs,Ts)

    def moveRelative(self, newPoint, method = "cubic", Ts = 0.01):
        self.moveRelQ.put([(0,0,0),newPoint])
        if self.runLock == False:
            while not self.moveRelQ.empty():
                self.bot.waypointsParse(self.moveRelQ.get(),method)
                self.bot.calcOutputs(Ts)
                self.runOutputs(self.bot.outputs,Ts)

if __name__ == "__main__":
    
    turret = Turret()
    turret.csvToRun('circle.csv')
