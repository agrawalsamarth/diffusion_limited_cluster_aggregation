#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Aug  5 15:29:33 2024

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
#def create_dirs(phi):
    

    # global params_dir
    phi_str = format(phi,'.3f')
    ns_str  = format(ns,'.1f')
    agg_str = format(agg_distance,'.3f')
    
    
    params_dir = './thesis_figs_data/params/ns='+ns_str+'/phi='+phi_str+'/'+agg_str+'/'
    path = Path(params_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass
    
    
    # global results_dir
    
    results_dir = './thesis_figs_data/results/ns='+ns_str+'/phi='+phi_str+'/'+agg_str+'/'
    path = Path(results_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass
    
    # global config_files_dir
    
    config_files_dir = './thesis_figs_data/config_files/ns='+ns_str+'/phi='+phi_str+'/'+agg_str+'/'
    path = Path(config_files_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass     

def run_simulation(phi, agg_distance, ns, rng_seed):
#def run_simulation(phi, rng_seed):
    
    phi_str = format(phi,'.3f')
    ns_str  = format(ns,'.1f')
    agg_str = format(agg_distance,'.3f')

    params_dir = './thesis_figs_data/params/ns='+ns_str+'/phi='+phi_str+'/'+agg_str+'/'
    results_dir = './thesis_figs_data/results/ns='+ns_str+'/phi='+phi_str+'/'+agg_str+'/'
    config_files_dir = './thesis_figs_data/config_files/ns='+ns_str+'/phi='+phi_str+'/'+agg_str+'/'
    
    lattice=0
    alpha=0.5
    seedMass=1e12
    D=3
    N=100000
    L = ((N/phi))**(1/D)
    steps = str(int(L*10)+1)
    
    
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
    #file.write("seed_pct=100\n")
    for axis in range(D):
        file.write("x"+str(axis)+"_bc=periodic\n")
    file.write("N="+str(N)+"\n")
    file.write("phi="+str(phi)+"\n")
    #for axis in range(D):
     #   file.write("x"+str(axis)+"_L=57.7\n")
    file.write("alpha="+str(alpha)+"\n")
    file.write("seedMass="+str(seedMass)+"\n")
    file.write("rng_seed="+str(rng_seed)+"\n")        
    file.close()
    
    #print("phi = {}, agg = {}, rng_seed={}, simulation".format(phi,agg_distance,rng_seed))
    print("rng_seed={}, ns={}, agg_dist={}, phi={}".format(rng_seed, ns, agg_distance, phi))

    config_filename=config_files_dir+str(rng_seed)+'.csv'
    command = '../../simulation_files/bin/simulation '+params_file+' '+config_filename        
    os.system(command)
    
    
    results_filename=results_dir+'sq_'+str(rng_seed)+'_fixed.csv'
    command="../../postprocessing/bin/long_range_scattering "+config_filename+" "+steps+" 0.1 10 1000 "+ results_filename
    os.system(command)

os.system('cd ../../simulation_files; make simulation')
os.system('cd ../../postprocessing/; make all')

seed(1)
seeds=10
dim=3

ns_range = [0.1]
agg_dist_tolerance_range = [0.0,0.15,0.3]
phi_range = [0.05, 0.075, 0.1]

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
            

num_processors = 10
Parallel(n_jobs=num_processors)(delayed(run_simulation)(param[0], param[1], param[2], param[3]) for param in all_params)


os.system('cd ../../simulation_files/; make clean')
os.system('cd ../../postprocessing/; make clean')
