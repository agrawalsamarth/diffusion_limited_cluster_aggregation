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

def create_dirs(phi, dim, agg_distance, N):

    global D
    
    D = dim

    global params_dir
    
    phi_str = format(phi, '.4f')
    agg_str = format(agg_distance, '.3f')
    
    params_dir = './gpsd_results/lattice=0/params/phi='+phi_str+'/'+agg_str+'/N='+str(N)+'/'
    path = Path(params_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass
    
    
    global results_dir
    
    results_dir = './gpsd_results/lattice=0/results/phi='+phi_str+'/'+agg_str+'/N='+str(N)+'/'
    path = Path(results_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass
    
    global config_files_dir
    
    config_files_dir = './gpsd_results/lattice=0/config_files/phi='+phi_str+'/'+agg_str+'/N='+str(N)+'/'
    path = Path(config_files_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass     

def run_simulation(phi, agg_distance, N, rng_seed):
    
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
    file.write("agg_dist_tolerance="+format(agg_distance,'.2f')+"\n")
    file.write("D="+str(D)+"\n")
    file.write("seed_pct=100\n")
    for axis in range(D):
        file.write("x"+str(axis)+"_bc=periodic\n")
    #for axis in range(D):
        #file.write("x"+str(axis)+"_L="+str(L)+"\n")
    file.write("N="+str(N)+"\n")
    file.write("phi="+str(phi)+"\n")
    file.write("alpha="+str(alpha)+"\n")
    file.write("seedMass="+str(seedMass)+"\n")
    file.write("rng_seed="+str(rng_seed)+"\n")        
    file.close()
    
    print("phi = {}, rng_seed={}, agg_dist = {}, N = {}, simulation".format(phi,rng_seed,agg_distance,N))

    config_filename=config_files_dir+str(rng_seed)+'.csv'
    command = '../../simulation_files/bin/simulation '+params_file+' '+config_filename        
    os.system(command)
        
    results_filename=results_dir+'lb_'+str(rng_seed)+'.csv'
    command="../../postprocessing/bin/lb_bonds_clusterwise_invA "+config_filename+" "+ results_filename
    os.system(command)
    

    
os.system('../../simulation_files/make simulation')
os.system('../../postprocessing/make all')

seed(1)
seeds=10
seed_pct_range=[100]
dim = 3
#agg_dist_tolerance_range = [0.1,0.15,0.2]
#phi_range = [0.1/2.2, 0.5/2.2, 1.0/2.2]
#N_range = [20000, 40000]

agg_dist_tolerance_range = [0.025, 0.05, 0.1, 0.2]
phi_range = [0.1/2.2]
N_range   = [100000]

for agg_dist in agg_dist_tolerance_range:
    for phi in phi_range:
        for N in N_range:
            create_dirs(phi, dim, agg_dist, N)
            Parallel(n_jobs=10)(delayed(run_simulation)(phi, agg_dist, N, rng_seed) for rng_seed in range(seeds))
