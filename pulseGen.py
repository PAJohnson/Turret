# stepper pulse generator simulator
# experiment with different rates for NCO and compare errors

import trajectoryPlanner as tp

class NCO:
    def __init__(self, size):
        self.accumulator = int(0)
        self.size = size
        self.output = 0
        
    def accumulate(self, tuningWord):
        #accumulate tuningWord
        #if the accumulator overflows, output a 1 for one period
        self.accumulator += int(tuningWord)
        if self.output == True:
            self.output = False
        if self.accumulator >= self.size:
            self.output = True
            self.accumulator -= self.size
            
class stepper:
    def __init__(self):
        self.steps = 3200 #steps per full revolution
        self.position = int(0)
        self.ratio = 5.0
        self.degPerStep = 360.0/self.steps/self.ratio
        
    def step(self, direction):
        if direction == 1:
            self.position += int(1)
        else:
            self.position -= int(1)
            
def calcTuningWord(freq, size, rate):
    #eq is freq = TW/size * rate
    #TW = (freq*size/rate)
    word = int(freq*size/rate)
    return word

def calcError(stepper, sequence, Ts, rate, size, time):
    #take sequence of [(position, velocity),..] and execute the sequence
    #calculate absolute position error at the end of a sequence
    simStep = 1/rate
    osc = NCO(size)
    for i in range(int(time*rate)):
        simTime = i*simStep
        cmdIndex = int(simTime/Ts)
        velocity = sequence[cmdIndex][1] + Ts * sequence[cmdIndex][2]
        accel = Ts * sequence[cmdIndex][2]
        #print(accel)
        #print(velocity)
        tw = calcTuningWord(abs(velocity/stepper.degPerStep),size,rate)
        #run NCO with tuning word
        osc.accumulate(tw)
        if osc.output == True:
            if velocity > 0:
                stepper.step(1)
            else:
                stepper.step(0)

    error = abs(stepper.position*stepper.degPerStep - sequence[-1][0])
    return error
            
if __name__ == "__main__":
    #test here
    pulseGen = NCO(2**24)
    #do some error testing
    #scan through sample rate and size
    rates = [1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000]
    sizes = [2**16, 2**20, 2**24, 2**28, 2**32]
    Ts = 0.001
    joint = tp.trajectoryPlanner(1)
    waypoints = [(0,0),(30,1)]
    joint.waypointsParse(waypoints,"cubic")
    joint.calcOutputs(Ts)
    seq = joint.outputs[0]
    for vector in seq:
        print(vector)
    errorDict = {}
    for rate in rates:
        for size in sizes:
            motor = stepper()
            errorDict[(rate,size)] = calcError(motor,seq,Ts,rate,size,1)
            
    print(errorDict.values())
    #is the error swamped by the linear interpolation of velocities?
    #yes! running in speed mode is causing the error here.
    #Error will always go to zero when returning to a certain position
    #otherwise, error accumulates at all points!!!
    #need smarter algorithm or position control.
