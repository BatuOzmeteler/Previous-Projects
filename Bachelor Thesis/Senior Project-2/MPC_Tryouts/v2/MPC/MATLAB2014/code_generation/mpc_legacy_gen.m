
addpath('C:\Program Files\casadi-windows-matlabR2014b-v3.5.1')
import casadi.*

def = legacy_code('initialize')

def.SourceFiles = {'M.c'};
def.HeaderFiles = {'M.h'};
def.SFunctionName = 'mpc_controller';
def.OutputFcnSpec = 'double y1 = doubleIt(double u1)';