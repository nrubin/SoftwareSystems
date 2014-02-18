"""Modified version of the example code from Janert,
Feedback Control For Computer Systems

This modified version requires pandas, numpy, and matplotlib.

If you use apt:

sudo apt-get install python-pandas python-numpy python-matplotlib
"""

import numpy
import pandas
import random
import matplotlib.pyplot as pyplot

class Buffer:
    def __init__( self, max_wip, max_flow ):
        """Initializes the buffer:

        max_wip: maximum work in progress
        max_flow: maximum work completed per time step
        """
        self.queued = 0
        self.wip = 0             # work-in-progress ("ready pool")

        self.max_wip = max_wip
        self.max_flow = max_flow # avg outflow is max_flow/2

    def work( self, u ):
        # Add to ready pool
        u = max( 0, int(round(u)) )
        u = min( u, self.max_wip )
        self.wip += u

        # Transfer from ready pool to queue
        r = int( round( random.uniform( 0, self.wip ) ) )
        self.wip -= r
        self.queued += r

        # Release from queue to downstream process
        r = int( round( random.uniform( 0, self.max_flow ) ) )
        r = min( r, self.queued )
        self.queued -= r

        return self.queued

class Controller:
    def __init__( self, kp, ki, kd ):
        """Initializes the controller.

        kp: proportional gain
        ki: integral gain
        kd: derivative gain
        """
        self.kp, self.ki, self.kd = kp, ki, kd
        self.i = 0       # Cumulative error ("integral")
        self.d = 0
        self.last_t = 0
        self.last_e = 0

    def work( self, e ,t):
        """Computes the number of jobs to be added to the ready queue.

        e: error

        returns: float number of jobs
        """
        self.i += e
        #derivative term is (current error - prev error) / (current time - previous time)
        self.d = (e - self.last_e) / (t - self.last_t)
        self.last_e = e
        self.last_t = t

        return self.kp*e + self.ki*self.i + self.kd*self.d

# ============================================================

def closed_loop( c, p, tm=5000 ):
    """Simulates a closed loop control system.

    c: Controller object
    p: Buffer object
    tm: number of time steps

    returns: tuple of sequences (times, targets, errors)
    """
    def setpoint( t ):
        """
        with a gradually increasing setpoint, the system displays a higher value of RMS
        the system also consistently undershoots the setpoint curve
        """
        return 0.004*(t**1.7)
        # if t < 100: return 0
        # if t < 300: return 50
        # return 10
    
    y = 0
    res = []
    for t in range(1, tm ):
        r = setpoint(t)
        e = r - y
        u = c.work(e,t)
        y = p.work(u)

        #print t, r, e, u, y
        res.append((t, r, e, u, y))

    return zip(*res)

# ============================================================
"""
Setting ki to 0 increases RMS error by about 0.5

Also, educing the integral term decreases feedback delay
It also dampens the effect of output peaks.
"""

"""
My Controller with a derivative gain of 0.9 is about 25% better
"""
c = Controller( 1.0, 0.0005, 0.9 )
# c = Controller( 1.0, 0.0005, 0 )
p = Buffer( 50, 10 )

# run the simulation
ts, rs, es, us, ys = closed_loop( c, p, 1000 )

rms = numpy.sqrt(numpy.mean(numpy.array(es)**2))
print "RMS = %s" % rms

# generate the smoothed curve using a rolling mean
# (I think the curves in the book use loess)
ys_smooth = pandas.rolling_mean(numpy.array(ys), 20)

# make the plot
pyplot.plot(ts, rs, color='green', label='target')
pyplot.plot(ts, ys, color='red', label='queue length')
pyplot.plot(ts, ys_smooth, color='blue', label='trend')
pyplot.show()



