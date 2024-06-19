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

def create_dirs(phi, agg_distance):

    global D
    
    D = dim

    # global params_dir
    
    phi_str = format(phi, '.5f')
    agg_str = format(agg_distance, '.3f')
    
    params_dir = './scaling/params/phi='+phi_str+'/'+agg_str+'/'
    path = Path(params_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass
    
    
    # global results_dir
    
    results_dir = './scaling/results/phi='+phi_str+'/'+agg_str+'/'
    path = Path(results_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass
    
    # global config_files_dir
    
    config_files_dir = './scaling/config_files/phi='+phi_str+'/'+agg_str+'/'
    path = Path(config_files_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass     

def run_simulation(phi, agg_distance, rng_seed):

    phi_str = format(phi, '.5f')
    agg_str = format(agg_distance, '.3f')

    params_dir = './scaling/params/phi='+phi_str+'/'+agg_str+'/'
    results_dir = './scaling/results/phi='+phi_str+'/'+agg_str+'/'
    config_files_dir = './scaling/config_files/phi='+phi_str+'/'+agg_str+'/'
    
    lattice=0
    alpha=0.5
    seedMass=1e12
    N=25000
    
    params_file  = params_dir+'params_'+str(rng_seed)+'.csv'
        
    file = open(params_file, "w")
    
    file.write("system=dlma\n")
    file.write("lattice="+str(lattice)+"\n")
    file.write("aggregation_type=normal\n")
    file.write("aggregation_condition=mass\n")
    file.write("bind=normal\n")
    file.write("movement=brownian\n")
    file.write("agg_dist_tolerance="+format(agg_distance,'.2f')+"\n")
    file.write("D="+str(D)+"\n")
    file.write("seed_pct=100\n")
    for axis in range(D):
        file.write("x"+str(axis)+"_bc=periodic\n")
    file.write("N="+str(N)+"\n")
    file.write("phi="+str(phi)+"\n")
    file.write("alpha="+str(alpha)+"\n")
    file.write("seedMass="+str(seedMass)+"\n")
    file.write("rng_seed="+str(rng_seed)+"\n")        
    file.close()
    
    print("phi = {}, agg_dist = {}, rng_seed={}, simulation".format(phi,agg_distance,rng_seed))

    config_filename=config_files_dir+str(rng_seed)+'.csv'
    command = '../../simulation_files/bin/simulation '+params_file+' '+config_filename        
    os.system(command)
        
    results_filename=results_dir+'lb_'+str(rng_seed)+'.csv'
    command="../../postprocessing/bin/lb_bonds_clusterwise_invA "+config_filename+" "+ results_filename
    os.system(command)
    

    
os.system('cd ../../simulation_files; make simulation')
os.system('cd ../../postprocessing/; make all')

seed(1)
seeds=100
seed_pct_range=[100]
dim=3

agg_dist_tolerance_range = [0.15, 0.2, 0.25, 0.175, 0.225]

phi_min   = 0.1
phi_max   = 0.3
num_i     = 10
delta_phi = np.log(phi_max/phi_min)/(1.*num_i)

phi_range = []

for i in range(num_i):
    
    phi_temp = phi_min * np.exp(1.*i*delta_phi)
    phi_range.append(phi_temp)

all_params = []

for phi in phi_range:
    for agg_dist_tolerance in agg_dist_tolerance_range:
        all_params.append([phi, agg_dist_tolerance])
        
for param in all_params:
    create_dirs(param[0], param[1])

all_params.clear()
agg_dist_tolerance_range_short = [0.15, 0.2, 0.25]

# 50 x 3 * 10 = 1500

for phi in phi_range:
    for agg_dist_tolerance in agg_dist_tolerance_range_short:
        for i in range(50, seeds):
            all_params.append([phi, agg_dist_tolerance, i])

agg_dist_tolerance_range_ext = [0.175, 0.225]

for phi in phi_range:
    for agg_dist_tolerance in agg_dist_tolerance_range_ext:
        for i in range(seeds):
            all_params.append([phi, agg_dist_tolerance, i])
            
for i in range(50):
    all_params.append([phi_range[-1], agg_dist_tolerance_range_short[-1], i])

num_processors = 80
Parallel(n_jobs=num_processors)(delayed(run_simulation)(param[0], param[1], param[2]) for param in all_params)

os.system('cd ../../simulation_files/; make clean')
os.system('cd ../../postprocessing/; make clean')
