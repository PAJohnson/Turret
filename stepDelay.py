# step generator
# given a series of polynomials describing a path, determine the
# timing of step pulses to best follow the path

# two methods
# method 1 - increment through each timestep and determine if the error
# between the path and the current position is large enough to take
# a step.
# e(t) = pd(t) - p(t)
# if e(t) >= resoultion/2 then step positive
# if e(t) <= -resolution/2 then step negative
# else do nothing

# method 2 - solve for the solutions to the polynomials 0 = pd(t) + e
# and 0 = pd(t) - e
# root finding is expensive, but the time between pulses could be large
# enough for this to be faster than calculating each timestep

# grab a representative polynomial from trajectory planner
import trajectoryPlanner as tp
import numpy as np

waypoints = [(0,0),(30,1)]
Ts = 0.01
joint = tp.trajectoryPlanner(1)
joint.waypointsParse(waypoints,"cubic")

coeffs = joint.cubicCoeffs[0][0]
time = coeffs[-1]
coeffs = coeffs[0:4]

ratio = 5.0
resolution = 360.0/3200/ratio #resolution is deg per step
eMax = resolution/2

def evalPoly(coeffs,time):
    return coeffs[0] + coeffs[1]*time + coeffs[2]*time**2 + coeffs[3]*time**3
    
#method 1
def calcStepIterative(coeffs,timeStep,position):
    p = position
    steps = []
    for _t in np.arange(0,time,timeStep):
        pd = evalPoly(coeffs,_t)
        e = pd - p
        if e > eMax:
            direction = 1
            steps.append([_t,direction])
            p += resolution
        elif e < -1.0*eMax:
            direction = -1
            steps.append([_t,direction])
            p -= resolution
        else:
            pass
    
    return steps

def calcStepRoots(coeffs,timeStep,position):
    _t = 0
    steps = []
    p = position
    coeffs = np.flip(coeffs,0)
    roots = np.roots(coeffs)
    eHigh = np.copy(coeffs)
    eLow = np.copy(coeffs)
    rootFound = True
    while _t < time and rootFound == True:
        rootFound = False
        eHigh[-1] = coeffs[-1] + (eMax + position)
        eLow[-1] = coeffs[-1] - (eMax + position)
        rootsBackward = np.roots(eHigh)
        rootsForward = np.roots(eLow)
        #trim results of rootsHigh and rootsLow
        for root in rootsBackward:
            if np.iscomplex(root) == False:
                if root < time and root >= _t:
                    _t = root
                    position -= resolution
                    steps.append([_t,-1])
                    rootFound = True
                    
        for root in rootsForward:
            if np.iscomplex(root) == False:
                if root < time and root >= _t:
                    _t = root
                    position += resolution
                    steps.append([_t,1])
                    rootFound = True
        #need a stop condition
        
    return steps
    
stepsIter = calcStepIterative(coeffs,1/100000,0)
stepsRoots = calcStepRoots(coeffs,1/100000,0)

for i in range(len(stepsIter)):
    print(str(stepsIter[i]) + " " + str(stepsRoots[i]))
