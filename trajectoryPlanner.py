#Patrick Johnson 2020
#Joint Trajectory Planner

#given a list of waypoints, this will output position/velocity/acceleration
#values for each joint at a desired sampling interval.
import numpy as np
import csv

class wayPoints:
    def __init__(self,filename = None):
        self.waypoints = []
        if filename != None:
            self.readFromFile(filename)
        
    def addWP(self,wp):
        #wp is (j1,j2,jn,t) tuple
        self.waypoints.append(wp)
        
    def readFromFile(self,filename):
        #assuming CSV file
        with open(filename,newline='') as wpFile:
            wpReader = csv.reader(wpFile, delimiter=',', quotechar='|')
            for wp in wpReader:
                newRow = [float(val) for val in wp]
                self.addWP(tuple(newRow))

class trajectoryPlanner:
    def __init__(self, jointNum):
        self.jointNum = jointNum
        self.method = None
        
    def waypointsParse(self,wayP,method):
        #interpret a list of waypoints
        #list of tuples, [(joint1_i,joint2_i,...,t_i)...]
        #interpolate between points with method
        self.method = method
        
        if len(wayP) < 2:
            print("not enough waypoints!")
            return
        
        #check for positive time increments
        tLast = 0
        for waypoint in wayP:
            tDiff = waypoint[-1]-tLast
            tLast = waypoint[-1]
            if tDiff < 0:
                print("not a positive time sequence!")
                return
        
        if len(wayP[0])-1 != self.jointNum:
            print("waypoints/joint mismatch!")
            return
        
        self.joints = []
        for i in range(self.jointNum):
            self.joints.append([])
        
        #split waypoints into individual [(position,time)] per joint
        for waypoint in wayP:
            for i in range(self.jointNum):
                self.joints[i].append([waypoint[i],waypoint[-1]])

        #joints[] is now a list of points,time per joint
        if method == "cubic":
            #cubic interpolation
            self.cubicInterpolate()
        elif method == "quintic":
            #quintic interpolation
            self.quinticInterpolate()
        elif method == "trapVel":
            self.trapVelInterpolate()
        else:
            pass
            
    def trapVelInterpolate(self):
        #interpolate between points with trapezoidal velocity
        #later - respect max velocity and acceleration constraints
        #no blending - stop at waypoints.
        pass
        
            
    def cubicInterpolate(self):
        #for each joint, calculate the coefficients for the cubic
        #interpolating polynomials
        
        #modify joints from [position,time] to [position,velocity,time]
        self.cubicCoeffs = []
        for j in self.joints:
            self.cubicCoeffs.append([])
        
        #null starting and ending velocity
        for j in self.joints:
            for i in range(len(j)):
                j[i].insert(1,0)
            
        #compute intermediate velocities
        #if the last and next velocities are of different sign, make 
        #velocity = 0 at that point.    
        for j in self.joints:
            for i in range(1,len(j)-1):
                #(pos-lastPos)/(tdiffLast)
                velBefore = (j[i][0]-j[i-1][0])/(j[i][-1]-j[i-1][-1])
                #(nextPos-pos)/(tdiffNext)
                velAfter = (j[i+1][0]-j[i][0])/(j[i+1][-1]-j[i][-1])
                if sign(velBefore) == sign(velAfter):
                    intermediateVelocity = (1/2)*(velBefore+velAfter)
                else:
                    intermediateVelocity = 0
                j[i][1] = intermediateVelocity
        
        #now, do the interpolation
        #treat each points starting time as 0, simpler math
        for num,j in enumerate(self.joints):
            for i in range(len(j)-1):
                #calculate a3...a0 for the cubic polynomial between
                #each pair of points
                
                qi = j[i][0]
                qi_dot = j[i][1]
                qf = j[i+1][0]
                qf_dot = j[i+1][1]
                tf = j[i+1][-1]-j[i][-1]
                
                A = np.array([[1,0,0,0],[0,1,0,0],[1,tf,tf**2,tf**3],[0,1,2*tf,3*tf**2]])
                B = np.array([[qi],[qi_dot],[qf],[qf_dot]])
                coeffs = np.linalg.solve(A,B)
                coeffs = np.append(coeffs,tf)
                self.cubicCoeffs[num].append(coeffs)
                
                
        
    def quinticInterpolate(self):
        #same as cubic but with continuous accelerations
        
        #modify joints from [position,time] to [position,velocity,accel,time]
        self.quinticCoeffs = []
        for j in self.joints:
            self.quinticCoeffs.append([])
        
        #null starting and ending velocity and acceleration
        for j in self.joints:
            for i in range(len(j)):
                j[i].insert(1,0)
                j[i].insert(1,0)
            
        #compute intermediate velocities
        #if the last and next velocities are of different sign, make 
        #velocity = 0 at that point.    
        for j in self.joints:
            for i in range(1,len(j)-1):
                #(pos-lastPos)/(tdiffLast)
                velBefore = (j[i][0]-j[i-1][0])/(j[i][-1]-j[i-1][-1])
                #(nextPos-pos)/(tdiffNext)
                velAfter = (j[i+1][0]-j[i][0])/(j[i+1][-1]-j[i][-1])
                if sign(velBefore) == sign(velAfter):
                    intermediateVelocity = (1/2)*(velBefore+velAfter)
                else:
                    intermediateVelocity = 0
                j[i][1] = intermediateVelocity
                
        #compute intermediate accelerations
        #if the last an next accel are diff sign, make accel = 0
        #at midpoint
        for j in self.joints:
            for i in range(1,len(j)-1):
                #(vel-lastVel)/(tdiffLast)
                accelBefore = (j[i][1]-j[i-1][1])/(j[i][-1]-j[i-1][-1])
                #(nextVel-vel)/(tdiffNext)
                accelAfter = (j[i+1][1]-j[i][1])/(j[i+1][-1]-j[i][-1])
                if sign(accelBefore) == sign(accelAfter):
                    intermediateAccel = (1/2)*(accelBefore+accelAfter)
                else:
                    intermediateAccel = 0
                j[i][2] = intermediateAccel
        
        #now, do the interpolation
        #treat each points starting time as 0, simpler math
        for num,j in enumerate(self.joints):
            for i in range(len(j)-1):
                #calculate a3...a0 for the cubic polynomial between
                #each pair of points
                
                qi = j[i][0]
                qi_dot = j[i][1]
                qi_ddot = j[i][2]
                qf = j[i+1][0]
                qf_dot = j[i+1][1]
                qf_ddot = j[i+1][2]
                tf = j[i+1][-1]-j[i][-1]
                
                A = np.array([[1,0,0,0,0,0],[0,1,0,0,0,0],[0,0,2,0,0,0],[1,tf,tf**2,tf**3,tf**4,tf**5],[0,1,2*tf,3*tf**2,4*tf**3,5*tf**4],[0,0,2,6*tf,12*tf**2,20*tf**3]])
                B = np.array([[qi],[qi_dot],[qi_ddot],[qf],[qf_dot],[qf_ddot]])
                coeffs = np.linalg.solve(A,B)
                coeffs = np.append(coeffs,tf)
                self.quinticCoeffs[num].append(coeffs)
        
    def calcOutputs(self,Ts):
        
        #calculate p,v,a outputs for each joint
        #sample polynomials with time interval Ts
        
        #self.outputs is a list, [[[j1_pi,j1_vi,j1_ai],...]...]
        self.outputs = []
        for j in self.joints:
            self.outputs.append([])
            
        if self.method == None:
            print("Paths have not been interpolated")
            return
        elif self.method == "cubic":
            i = 0
            for joint in self.cubicCoeffs:
                for j, c in enumerate(joint):
                    if j == 0:
                        #create 1st value
                        _t = 0
                        p = c[0] + c[1]*_t + c[2]*_t**2 + c[3]*_t**3
                        v = c[1] + 2*c[2]*_t + 3*c[3]*_t**2
                        a = 2*c[2] + 6*c[3]*_t
                        self.outputs[i].append([p,v,a])
                        
                    for _t in np.arange(Ts,c[-1]+Ts,Ts):
                        p = c[0] + c[1]*_t + c[2]*_t**2 + c[3]*_t**3
                        v = c[1] + 2*c[2]*_t + 3*c[3]*_t**2
                        a = 2*c[2] + 6*c[3]*_t
                        self.outputs[i].append([p,v,a])
                        
                i = i + 1
        elif self.method == "quintic":
            i = 0
            for joint in self.quinticCoeffs:
                for j, c in enumerate(joint):
                    if j == 0:
                        #create 1st value
                        _t = 0
                        p = c[0] + c[1]*_t + c[2]*_t**2 + c[3]*_t**3 + c[4]*_t**4 + c[5]*_t**5
                        v = c[1] + 2*c[2]*_t + 3*c[3]*_t**2 + 4*c[4]*_t**3 + 5*c[5]*_t**4
                        a = 2*c[2] + 6*c[3]*_t + 12*c[4]*_t**2 + 20*c[5]*_t**3
                        self.outputs[i].append([p,v,a])
                        
                    for _t in np.arange(Ts,c[-1]+Ts,Ts):
                        p = c[0] + c[1]*_t + c[2]*_t**2 + c[3]*_t**3 + c[4]*_t**4 + c[5]*_t**5
                        v = c[1] + 2*c[2]*_t + 3*c[3]*_t**2 + 4*c[4]*_t**3 + 5*c[5]*_t**4
                        a = 2*c[2] + 6*c[3]*_t + 12*c[4]*_t**2 + 20*c[5]*_t**3
                        self.outputs[i].append([p,v,a])
                        
                i = i + 1
        elif self.method == "trapVel":
            pass
        else:
            return
        
def sign(num):
    if num > 0:
        return 1
    elif num < 0:
        return -1
    else:
        return 0

def speedChange(waypoints, multiplier):
    newPoints = []
    for i in range(len(waypoints)):
        newPoints.append((waypoints[i][0],waypoints[i][1],waypoints[i][2]*multiplier))
        
    return newPoints

if __name__ == "__main__":
    
    #example usage
    #create a wayPoints class and parse a file of waypoints
    newWP = wayPoints()
    newWP.readFromFile('circle.csv')
    print("Waypoints: " + str(newWP.waypoints))
    
    #instantiate a trajectoryPlanner with the correct number of joints
    tp = trajectoryPlanner(2)
    
    #parse the waypoints and generate the polynomials to interpolate
    #between them
    tp.waypointsParse(newWP.waypoints, "cubic")
    
    #sample the polynomial sequence at the desired period
    tp.calcOutputs(0.01)
    
    #tp.outputs[0] is the sequence of P,V,A values for joint 0
    #tp.outputs[1] is the same for joint 1
    print("Outputs")
    print(tp.outputs[1])
    
