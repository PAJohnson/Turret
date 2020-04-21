#plot file
import trajectoryPlanner as tp
import matplotlib.pyplot as plt
import matplotlib as mpl
from pylatex import Document, Section, Subsection, Tabular, Math, TikZ, Axis, \
    Plot, Figure, Matrix, Alignat
from pylatex.utils import italic
import numpy as np

def pathPlot(waypoints, joints):
    #parse the waypoints and generate the polynomials to interpolate
    #between them
    joints.waypointsParse(newWP.waypoints, "cubic")
    
    #sample the polynomial sequence at the desired period
    joints.calcOutputs(0.01)
    
    #tp.outputs[0] is the sequence of P,V,A values for joint 0
    #tp.outputs[1] is the same for joint 1
    
    #plot cubic path
    tWP = [row[2] for row in newWP.waypoints]
    j1WP = [row[0] for row in newWP.waypoints]
    j2WP = [row[1] for row in newWP.waypoints]
    j1PC = [row[0] for row in joints.outputs[0]]
    j2PC = [row[0] for row in joints.outputs[1]]
    
    joints.waypointsParse(newWP.waypoints, "quintic")
    
    #sample the polynomial sequence at the desired period
    joints.calcOutputs(0.01)
    j1PQ = [row[0] for row in joints.outputs[0]]
    j2PQ = [row[0] for row in joints.outputs[1]]
    
    plt.plot(j1PC,j2PC,'g--')
    plt.plot(j1PQ,j2PQ,'y--')
    plt.plot(j1WP,j2WP,'rD')
    plt.xlabel('Joint 1 Position')
    plt.ylabel('Joint 2 Position')
    plt.title("Interpolation Comparison")
    
    SMALL_SIZE = 8
    MEDIUM_SIZE = 10
    BIGGER_SIZE = 12
    
    plt.rc('font', size=SMALL_SIZE)          # controls default text sizes
    plt.rc('axes', titlesize=SMALL_SIZE)     # fontsize of the axes title
    plt.rc('axes', labelsize=MEDIUM_SIZE)    # fontsize of the x and y labels
    plt.rc('xtick', labelsize=SMALL_SIZE)    # fontsize of the tick labels
    plt.rc('ytick', labelsize=SMALL_SIZE)    # fontsize of the tick labels
    plt.rc('legend', fontsize=SMALL_SIZE)    # legend fontsize
    plt.rc('figure', titlesize=BIGGER_SIZE)  # fontsize of the figure title
    
    plt.show()

def statesPlot(waypoints, joints):
    #waypoint data
    tWP = [row[2] for row in newWP.waypoints]
    j1WP = [row[0] for row in newWP.waypoints]
    j2WP = [row[1] for row in newWP.waypoints]
    
    #joint data
    j1P = [row[0] for row in joints.outputs[0]]
    j1V = [row[1] for row in joints.outputs[0]]
    j1A = [row[2] for row in joints.outputs[0]]
    
    j2P = [row[0] for row in joints.outputs[1]]
    j2V = [row[1] for row in joints.outputs[1]]
    j2A = [row[2] for row in joints.outputs[1]]
    t = [0.01*i for i in range(len(j1P))]
    
    #have data, now plot
    plt.subplot(2,3,1)
    plt.plot(t,j1P)
    plt.plot(tWP,j1WP,'rx')
    plt.ylabel('Joint 1 Position')
    
    plt.subplot(2,3,2)
    plt.plot(t,j1V,'g-')
    plt.ylabel('Joint 1 Velocity')
    plt.title('Joint States (Quintic Interpolation)')
    
    plt.subplot(2,3,3)
    plt.plot(t,j1A,'y-')
    plt.ylabel('Joint 1 Acceleration')
    
    plt.subplot(2,3,4)
    plt.plot(t,j2P)
    plt.plot(tWP,j2WP,'rx')
    plt.xlabel('time')
    plt.ylabel('Joint 2 Position')
    
    plt.subplot(2,3,5)
    plt.plot(t,j2V,'g-')
    plt.xlabel('time')
    plt.ylabel('Joint 2 Velocity')
    
    plt.subplot(2,3,6)
    plt.plot(t,j2A,'y-')
    plt.ylabel('Joint 2 Accel')
    plt.xlabel('time')
    
    SMALL_SIZE = 8
    MEDIUM_SIZE = 10
    BIGGER_SIZE = 12
    
    plt.rc('font', size=SMALL_SIZE)          # controls default text sizes
    plt.rc('axes', titlesize=SMALL_SIZE)     # fontsize of the axes title
    plt.rc('axes', labelsize=MEDIUM_SIZE)    # fontsize of the x and y labels
    plt.rc('xtick', labelsize=SMALL_SIZE)    # fontsize of the tick labels
    plt.rc('ytick', labelsize=SMALL_SIZE)    # fontsize of the tick labels
    plt.rc('legend', fontsize=SMALL_SIZE)    # legend fontsize
    plt.rc('figure', titlesize=BIGGER_SIZE)  # fontsize of the figure title
    
    plt.show()
    
def plot_equation(eq, fontsize=50, outfile=None, padding=0.1, **kwargs):
    """Plot an equation as a matplotlib figure.
    Parameters
    ----------
    eq : string
        The equation that you wish to plot. Should be plottable with
        latex. If `$` is included, they will be stripped.
    fontsize : number
        The fontsize passed to plt.text()
    outfile : string
        Name of the file to save the figure to.
    padding : float
        Amount of padding around the equation in inches.
    Returns
    -------
    ax : matplotlib axis
        The axis with your equation.
    """
    # clean equation string
    eq = eq.strip('$').replace(' ', '')
    
    # set up figure
    f = plt.figure()
    ax = plt.axes([0,0,1,1])    
    r = f.canvas.get_renderer()

    # display equation
    t = ax.text(0.5, 0.5, '${}$'.format(eq), fontsize=fontsize,
        horizontalalignment='center',verticalalignment='center')
    
    # resize figure to fit equation
    bb = t.get_window_extent(renderer=r)
    w,h = bb.width/f.dpi,np.ceil(bb.height/f.dpi)
    f.set_size_inches((padding+w,padding+h))

    # set axis limits so equation is centered
    plt.xlim([0,1])
    plt.ylim([0,1])
    ax.grid(False)
    ax.set_axis_off()

    if outfile is not None:
        plt.savefig(outfile, **kwargs)

    return ax

if __name__ == "__main__":
    
    #example usage
    #create a wayPoints class and parse a file of waypoints
    newWP = tp.wayPoints()
    newWP.readFromFile('circle.csv')
    print("Waypoints: " + str(newWP.waypoints))
    
    #instantiate a trajectoryPlanner with the correct number of joints
    joints = tp.trajectoryPlanner(2)
    
    #pathPlot(newWP,joints)
    
    #make some mathematical figures
    plot_equation(r'$ \begin{array} 1 & ti  & ti^2  & ti^3 \\ 0 & 1 & 2*ti & 3*ti^2 \\ 1 & tf & tf^2 & tf^3 \\  0 & 1 & 2*tf & 3*tf^2 \end{array}$',outfile='test',padding=0.1)
