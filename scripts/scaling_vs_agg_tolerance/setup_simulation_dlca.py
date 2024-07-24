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

def create_dirs(phi, agg_distance, ns):

    global D
    
    D = dim

    global N

    N = 100000

    # global params_dir
    
    phi_str = format(phi, '.5f')
    agg_str = format(agg_distance, '.3f')
    
    params_dir = './high_porosity_scaling/params/phi='+phi_str+'/'+agg_str+'/N='+str(N)+'/'
    path = Path(params_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass
    
    
    # global results_dir
    
    results_dir = './high_porosity_scaling/results/phi='+phi_str+'/'+agg_str+'/N='+str(N)+'/'
    path = Path(results_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass
    
    # global config_files_dir
    
    config_files_dir = './high_porosity_scaling/config_files/phi='+phi_str+'/'+agg_str+'/N='+str(N)+'/'
    path = Path(config_files_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass     

def run_simulation(phi, agg_distance, ns, rng_seed):

    phi_str = format(phi, '.5f')
    agg_str = format(agg_distance, '.3f')

    params_dir = './high_porosity_scaling/params/phi='+phi_str+'/'+agg_str+'/N='+str(N)+'/'
    results_dir = './high_porosity_scaling/results/phi='+phi_str+'/'+agg_str+'/N='+str(N)+'/'
    config_files_dir = './high_porosity_scaling/config_files/phi='+phi_str+'/'+agg_str+'/N='+str(N)+'/'
    
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
    file.write("agg_dist_tolerance="+format(agg_distance,'.3f')+"\n")
    file.write("D="+str(D)+"\n")
    file.write("seed_pct="+str(ns)+"\n")
    for axis in range(D):
        file.write("x"+str(axis)+"_bc=periodic\n")
    file.write("N="+str(N)+"\n")
    file.write("phi="+str(phi)+"\n")
    file.write("alpha="+str(alpha)+"\n")
    file.write("seedMass="+str(seedMass)+"\n")
    file.write("rng_seed="+str(rng_seed)+"\n")        
    file.close()
    
    print("phi = {}, ns = {}, rng_seed={}, simulation".format(phi,ns,rng_seed))

    config_filename=config_files_dir+str(rng_seed)+'.csv'
    command = '../../simulation_files/bin/simulation '+params_file+' '+config_filename        
    os.system(command)
        
    results_filename=results_dir+'lb_'+str(rng_seed)+'.csv'
    command="../../postprocessing/bin/lb_bonds_clusterwise_invA "+config_filename+" "+ results_filename
    os.system(command)
    
    """results_filename=results_dir+'sq_'+str(rng_seed)+'.csv'
    command = "../../postprocessing/bin/long_range_scattering "+config_filename+" 1000 0.01 50 2000 "+ results_filename
    os.system(command)"""    
    

    
os.system('cd ../../simulation_files; make simulation')
os.system('cd ../../postprocessing/; make all')

seed(1)
seeds=100
seed_pct_range=[100]
dim=3

ns_range = [100]
agg_dist_tolerance_range = [0.2,0.25,0.3]

phi_min   = 0.05
phi_max   = 0.15
num_i     = 10
delta_phi = np.log(phi_max/phi_min)/(1.*num_i)

phi_range = []

for i in range(num_i):
    
    phi_temp = phi_min * np.exp(1.*i*delta_phi)
    phi_range.append(phi_temp)

all_params = []

for phi in phi_range:
    for agg_dist_tolerance in agg_dist_tolerance_range:
        for ns in ns_range:
            all_params.append([phi, agg_dist_tolerance, ns])
        
for param in all_params:
    create_dirs(param[0], param[1], param[2])

all_params.clear()


for phi in phi_range:
    for agg_dist_tolerance in agg_dist_tolerance_range:
        for ns in ns_range:
            for i in range(seeds):
                all_params.append([phi, agg_dist_tolerance, ns, i])
            

num_processors = 80
Parallel(n_jobs=num_processors)(delayed(run_simulation)(param[0], param[1], param[2], param[3]) for param in all_params)

os.system('cd ../../simulation_files/; make clean')
os.system('cd ../../postprocessing/; make clean')
