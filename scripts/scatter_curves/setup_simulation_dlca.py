#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Jun  4 13:56:09 2021

@author: samarth
"""

import pandas as pd
import os
import sys
from pathlib import Path
from joblib import Parallel, delayed
from matplotlib import pyplot as plt
import numpy as np
from sklearn.linear_model import LinearRegression
from random import random, seed
import subprocess

def create_dirs(phi, dim):

    global D
    
    D = dim

    global params_dir
    
    phi_str = format(phi, '.4f')
    
    params_dir = './act_values/params/phi='+phi_str+'/'
    path = Path(params_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass
    
    
    global results_dir
    
    results_dir = './act_values/results/phi='+phi_str+'/'
    path = Path(results_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass
    
    global config_files_dir
    
    config_files_dir = './act_values/config_files/phi='+phi_str+'/'
    path = Path(config_files_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass     

def run_simulation(phi, L, rng_seed):
    
    lattice=0
    alpha=0.5
    seedMass=1e12
    
    params_file  = params_dir+'params_'+str(rng_seed)+'.csv'
        
    file = open(params_file, "w")
    
    file.write("system=dlma\n")
    file.write("lattice="+str(lattice)+"\n")
    file.write("aggregation_type=normal\n")
    file.write("aggregation_condition=mass\n")
    file.write("bind=normal\n")
    file.write("movement=brownian\n")
    file.write("agg_dist_tolerance=0.0\n")
    file.write("D="+str(D)+"\n")
    file.write("seed_pct=100\n")
    for axis in range(D):
        file.write("x"+str(axis)+"_bc=periodic\n")
    #for axis in range(D):
        #file.write("x"+str(axis)+"_L="+str(L)+"\n")
    file.write("N=10000\n")
    file.write("phi="+str(phi)+"\n")
    file.write("alpha="+str(alpha)+"\n")
    file.write("seedMass="+str(seedMass)+"\n")
    file.write("rng_seed="+str(rng_seed)+"\n")        
    file.close()
    
    print("phi = {}, rng_seed={} simulation".format(phi,rng_seed))

    config_filename=config_files_dir+str(rng_seed)+'.csv'
    command = '/home/mkroeger/PC/vsandra/C4SCIENCE/dlma/simulation_files/bin/simulation '+params_file+' '+config_filename        
    os.system(command)
    
    results_filename=results_dir+str(rng_seed)+'.csv'
    command = "/home/mkroeger/PC/vsandra/C4SCIENCE/dlma/postprocessing/bin/long_range_scattering "+config_filename+" 1000 0.02 10 1000 "+ results_filename
    os.system(command)
    

    

seed(1)
seeds=50
seed_pct_range=[100]
dim = 3
L=57.7
phi_range = [(100-96.21)/100, (100-94.83)/100, (100-93.01)/100, (100-91.08)/100, (100-88.20)/100, (100-86.80)/100]

for phi in phi_range:
    create_dirs(phi, dim)
    Parallel(n_jobs=50)(delayed(run_simulation)(phi, L, rng_seed) for rng_seed in range(seeds))
