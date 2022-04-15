# -*- coding: utf-8 -*-
"""
# Problem Description

# States
x_1 = psi   => yaw angle of the kite
x_2 = theta => zenith angle of the kite
x_3 = phi   => azimuth angle of the kite

# Inputs
u_1 = delta => steering deflection

@authors: Mehmet Batu Özmeteler, 230306
          Rajesh Pushparaj, 229981
          Ashwin Nedungadi, 230397
"""

import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
from casadi import *
from casadi.tools import *
from wind_model import Wind

def Lagrange(tau_col, tau, j):
    l = 1
    for k in range(len(tau_col)):
        if k!=j:
            l *= (tau-tau_col[k])/(tau_col[j]-tau_col[k]) 
    return l

def plotOpenLoop(opt_x_k):
    X_k = horzcat(*opt_x_k['x',:,0,:])
    
    U_k = np.array(horzcat(*opt_x_k['u',:]))
    U_k = np.append(U_k, U_k[0,-1])
    
    E_k = np.array(horzcat(*opt_x_k['epsilon',:]))
    E_k = np.append(E_k, E_k[0,-1])
    
    SIZE = X_k.T.shape[0]
    
    height = [h_fcn(X_k.T[i,:]) for i in range(SIZE)]
    tether_force = [-stage_cost_fcn(X_k.T[i,:], U_k.T[i], E_k.T[i]) for i in range(SIZE)]
    
    height = np.array(height).reshape(-1,)
    tether_force = np.array(tether_force).reshape(-1,)
    
    fig, ax = plt.subplots(4, 1, figsize=(10,14))

    ax[0].plot(X_k.T[:,2], X_k.T[:,1])
    
    ax[1].plot(U_k.T)
    ax[2].plot(height)
    ax[3].plot(tether_force)
    
    ax[0].set_ylabel('theta')
    ax[0].set_xlabel('phi')
    
    ax[1].set_ylabel('input')
    ax[2].set_ylabel('height')
    ax[3].set_ylabel('tether force')
    
    ax[3].set_xlabel('discretization steps')

    plt.show()

def plotClosedLoop(res_x_mpc, res_u_mpc, res_e_mpc):
    res_x_mpc = np.concatenate(res_x_mpc,axis=1)
    res_u_mpc = np.concatenate(res_u_mpc, axis=1)
    res_e_mpc = np.concatenate(res_e_mpc, axis=1)
    
    SIZE = len(res_x_mpc.T)
    
    height = [h_fcn(res_x_mpc.T[i,:]) for i in range(SIZE)]
    tether_force = [-stage_cost_fcn(res_x_mpc.T[i,:],res_u_mpc.T[i],res_e_mpc.T[i]) for i in range(SIZE)]
    
    #convert to np array for plotting
    height = np.array(height).reshape(-1,)
    tether_force = np.array(tether_force).reshape(-1,)
    
    fig, ax = plt.subplots(4,1, figsize=(10,14))
    
    #ax[0].plot(res_x_mpc.T[:,0], label='psi')
    #ax[0].plot(res_x_mpc.T[:,1], label='theta')
    #ax[0].plot(res_x_mpc.T[:,2], label='phi')
    #ax[0].legend()
    
    ax[0].plot(res_x_mpc.T[:,2], res_x_mpc.T[:,1])
    
    ax[1].plot(res_u_mpc.T)
    ax[2].plot(height)
    ax[3].plot(tether_force)
    
    ax[0].set_ylabel('theta')
    ax[0].set_xlabel('phi')
    
    ax[1].set_ylabel('input')
    ax[2].set_ylabel('height')
    ax[3].set_ylabel('tether force')
    
    ax[3].set_xlabel('discretization steps')
    
    plt.show()
    
# Initialize parameters
dt = 0.2                        #timestep should be between 0.05 - 0.5 s

N = 80                          #prediction horizon of 16 s

L = 400                         #tether length
A = 300                         #kite surface area
c = 0.028

h_min = 100                     #minimum height above the ground
h_max = np.Inf                  #maximum height above the ground

E0 = 6                          #reference glide ratio
v0 = 10                         #constant wind speed

rho = 1                         #value retrieved from the reference paper

nx = 3                          #dimension of the state vector
nu = 1                          #dimension of the input vector

# Create the CasADi function to get xdot
x = SX.sym('x', nx, 1)
u = SX.sym('u', nu, 1)

# System Dynamics
E = E0 - (c*u**2)

v_a = v0*E*cos(x[1])

xdot = vertcat( (v_a/L)*( u - (cos(x[1])/sin(x[1]))*sin(x[0]) ) ,
                (v_a/L)*( cos(x[0]) - (tan(x[1])/E) ),
                -( v_a / (L*sin(x[1])) )*sin(x[0])
               )
system = Function("sys", [x,u], [xdot])

# Create the ODE and an IDAS integrator for system simulation
ode = {'x': x, 'ode': xdot, 'p': u}
opts = {'tf': dt}

ode_solver = integrator('F', 'idas', ode, opts)

# Generate radau/legendre collocation points
K = 2 #number of collocation points

tau_col = collocation_points(K,'radau') #exludes 0
tau_col = [0] + tau_col

# Calculate orthogonal collocation coefficients
tau = SX.sym('tau')
L_jk = np.zeros([K+1,K+1])

for j in range(K+1):
    dLj = gradient( Lagrange(tau_col, tau, j), tau )
    dLj_fcn = Function('dLj_fcn', [tau], [dLj])
    for k in range(K+1):
        L_jk[j,k] = dLj_fcn(tau_col[k])

# Calculate continuity coefficients
D = np.zeros((K+1,1))  

for j in range(K+1):
    Lj = Lagrange(tau_col, tau, j)
    Lj_fcn = Function('Lj_fcn', [tau], [Lj])
    D[j] = Lj_fcn(1)

# MPC with Orthogonal Collocation----------------------------------------------

# Slack Variable for the Height Constraint (as a soft constraint)
epsilon = SX.sym('epsilon', 1)

# Stage Cost
# Tf is the tether force 
# Wf is the weight term for the tension
# Wu is the weight term for the control input (for a smooth control trajectory) 
Tf = ((rho*v0**2)/2)*A*(cos(x[1])**2)*(E+1)*sqrt( ((E**2) + 1) )*cos(x[1])
Wf = 10E-4
Wu = 0.5
Ws = 10**3

stage_cost = -Wf*Tf + Ws*(epsilon**2)
stage_cost_fcn = Function('stage_cost_fcn',[x,u,epsilon],[stage_cost])

# Height Constraint
h = L*sin(x[1])*cos(x[2])
h_fcn = Function('h_fcn',[x],[h])

# State Constraints
lb_x = np.array([-np.pi, 0, -np.pi/2])
ub_x = np.array([np.pi, np.pi/2, np.pi/2])

# Input Constraints
lb_u = -10
ub_u = 10

# Bounds for the Slack Variable 
lb_epsilon = 0
ub_epsilon = 5

# Structured Optimization Variable
# Collocation points are included in the opt. problem hence the opt. variable
# vector includes N+1 intervals with K+1 col. points in each 
opt_x = struct_symSX([
    entry('x', shape=nx, repeat=[N+1, K+1]),
    entry('u', shape=nu, repeat=[N]),
    entry('epsilon', shape=1, repeat=[N])
])

# Populating the structured optimization variable
lb_opt_x = opt_x(0)
ub_opt_x = opt_x(0)

# Setting up the constraints
lb_opt_x['x'] = lb_x
ub_opt_x['x'] = ub_x

lb_opt_x['u'] = lb_u
ub_opt_x['u'] = ub_u

lb_opt_x['epsilon'] = lb_epsilon
ub_opt_x['epsilon'] = ub_epsilon

# Initializing a new problem with cost function and constraints vector
J = 0
g = [] 
lb_g = []
ub_g = []  

# Defining x_init as a symbolic variable such that you can pass the values of the
# changing initial condition to the solver at every MPC iteration
x_init = SX.sym('x_init', nx)

x0 = opt_x['x', 0, 0]

# Setting up the equality constraint for the initial condition of the states
g.append(x0-x_init)
lb_g.append(np.zeros((nx,1)))
ub_g.append(np.zeros((nx,1)))

for i in range(N):
    
    # Adding the stage cost for the current iteration of the prediction horizon
    J += stage_cost_fcn(opt_x['x',i,0], opt_x['u',i], opt_x['epsilon',i])
    
    if i > 0:
        # Penalizing large input changes
        J += Wu*(opt_x['u',i] - opt_x['u',i-1])**2 
    
    # Adding the discretized system constraint using orthogonal collocation
    for k in range(1,K+1):
        gk = -dt*system(opt_x['x',i,k], opt_x['u',i])
        for j in range(K+1):
            gk += L_jk[j,k]*opt_x['x',i,j]
        
        g.append(gk)
        lb_g.append(np.zeros((nx,1)))
        ub_g.append(np.zeros((nx,1)))
    
    # Integrating the system for a step and multiplying by D to get the state value
    # at the beginning of the next interval
    x_next = horzcat(*opt_x['x',i])@D
    g.append(x_next - opt_x['x', i+1, 0])
    lb_g.append(np.zeros((nx,1)))
    ub_g.append(np.zeros((nx,1)))
    
    # Add the height constraint
    g.append(h_fcn(opt_x['x', i, 0]) + opt_x['epsilon', i, 0])
    lb_g.append(h_min)
    ub_g.append(h_max)

# Vertically concatenating the constraint vectors for CasADi
g = vertcat(*g)
lb_g = vertcat(*lb_g)
ub_g = vertcat(*ub_g)

# Creating the problem structure as a dictionary
prob = {'f':J, 'x':vertcat(opt_x), 'g':g, 'p':x_init}
mpc_solver = nlpsol('solver', 'ipopt', prob)

# Setting up the initial condition
x_0 = np.array([2.5, 0.5, 0]).reshape(nx,1)

# Give initial guesses to the solver (non-zero guesses due to trigonometric functions)
opt_x_k = opt_x(1)

# Solving once for open-loop predictions
mpc_res = mpc_solver(p=x_0, x0=opt_x_k, lbg=lb_g, ubg=ub_g, lbx = lb_opt_x, ubx = ub_opt_x)
opt_x_k = opt_x(mpc_res['x'])

# Plot the open-loop trajectories
plotOpenLoop(opt_x_k)

# Initialize trajectories
res_x_mpc = [x_0]
res_u_mpc = []
res_e_mpc = []
wind_speeds = []

# Simulation time of 50 seconds
t_sim = 50
N_sim = int(t_sim/dt)

for i in range(N_sim):
    
    # Solve the optimization problem with a receding horizon scheme
    
    # If condition exists just to have the next initial state equal to the 
    # calculated optimal states after the first iteration 
    mpc_res = mpc_solver(p=x_0, x0=opt_x_k, lbg=lb_g, ubg=ub_g, lbx = lb_opt_x, ubx = ub_opt_x)

    opt_x_k = opt_x(mpc_res['x'])
    u_k = opt_x_k['u', 0]
    e_k = opt_x_k['epsilon',0]
    
    # Integrate the system
    res_integrator = ode_solver(x0=x_0, p=u_k)
    x_next = res_integrator['xf']

    # Update the initial state
    x_0 = x_next
  
    # Store the results
    res_x_mpc.append(x_next)
    res_u_mpc.append(u_k)
    res_e_mpc.append(e_k)
    
    text = '\nIteration: {}'.format(i)
    print(text)
    
# Repeat last entries for an easier plot
res_u_mpc.append(u_k)
res_e_mpc.append(e_k)

# Plot the closed-loop trajectories
plotClosedLoop(res_x_mpc, res_u_mpc, res_e_mpc)
