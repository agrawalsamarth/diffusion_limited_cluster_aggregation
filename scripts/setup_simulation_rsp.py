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


def create_dirs(phi, N, seed_pct, dim):

    global D
    
    D = dim

    global params_dir
    
    phi_str = format(phi, '.3f')
    
    params_dir = './'+str(D)+'d/params/phi='+phi_str+'/seed_pct='+str(seed_pct)+'/L='+str(N)+'/'
    path = Path(params_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass
    
    
    global results_dir
    
    results_dir = './'+str(D)+'d/results/phi='+phi_str+'/seed_pct='+str(seed_pct)+'/L='+str(N)+'/'
    path = Path(results_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass
    
    global config_files_dir
    
    config_files_dir = './'+str(D)+'d/config_files/phi='+phi_str+'/seed_pct='+str(seed_pct)+'/L='+str(N)+'/'
    path = Path(config_files_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass     

def run_simulation(phi, L, seed_pct, rng_seed):
    
    lattice=1
    
    params_file  = params_dir+'params_'+str(rng_seed)+'.csv'
        
    file = open(params_file, "w")
    
    file.write("system=random_site_percolation\n")
    file.write("lattice="+str(lattice)+"\n")
    file.write("D="+str(D)+"\n")
    file.write("phi="+str(phi)+"\n")
    file.write("seed_pct="+str(seed_pct)+"\n")
    
    for axis in range(D):    
        file.write("x"+str(axis)+"_bc=periodic\n")
        
    for axis in range(D):
        file.write("x"+str(axis)+"_L="+str(L)+"\n")

    file.write("rng_seed="+str(rng_seed)+"\n")        
    file.close()
    
    print("phi = {}, L={},rng_seed={} simulation".format(phi,L,rng_seed))

    config_filename=config_files_dir+str(rng_seed)+'.csv'
    command = 'simulation '+params_file+' '+config_filename        
    os.system(command)
    
    print("phi = {}, L={},rng_seed={} lbp".format(phi,L,rng_seed))
    
    rog_filename=results_dir+'lbp_'+str(rng_seed)+'.csv'
    time_filename=results_dir+'time_'+str(rng_seed)+'.csv'
    command = 'lb_bonds_clusterwise_invA ' + config_filename +' ' + rog_filename + ' ' + time_filename
    os.system(command)
    
def create_dirs_for_mk_table(dim):

    global D
    
    D = dim

    global params_dir
    
    
    params_dir = './mk_table/params/'
    path = Path(params_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass
    
    
    global results_dir
    
    results_dir = './mk_table/results/'
    path = Path(results_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass
    
    global config_files_dir
    
    config_files_dir = './mk_table/config_files/'
    path = Path(config_files_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass     

def run_simulation_for_random_params(phi, L, rng_seed):
    
    lattice=1
    x0_bc="periodic"
    x1_bc="periodic"
    x2_bc="periodic"
    alpha=0.5
    seedMass=1e12
    seed_pct = 100

    
    params_file  = params_dir+'params_'+str(rng_seed)+'.csv'        
    file = open(params_file, "w")
    
    file.write("system=random_site_percolation\n")
    file.write("lattice="+str(lattice)+"\n")
    file.write("aggregation_type=normal\n")
    file.write("aggregation_condition=mass\n")
    file.write("bind=normal\n")
    file.write("movement=brownian\n")
    file.write("agg_dist_tolerance=0.0\n")
    file.write("D="+str(D)+"\n")
    file.write("seed_pct="+str(seed_pct)+"\n")
    file.write("x0_bc="+str(x0_bc)+"\n")
    file.write("x1_bc="+str(x1_bc)+"\n")
    file.write("x2_bc="+str(x2_bc)+"\n")
    file.write("x0_L="+str(L)+"\n")
    file.write("x1_L="+str(L)+"\n")
    file.write("x2_L="+str(L)+"\n")
    file.write("phi="+str(phi)+"\n")
    file.write("alpha="+str(alpha)+"\n")
    file.write("seedMass="+str(seedMass)+"\n")
    file.write("rng_seed="+str(rng_seed)+"\n")        
    file.close()
    
    print("phi = {}, N={},rng_seed={} simulation".format(phi,L,rng_seed))

    config_filename=config_files_dir+str(rng_seed)+'.csv'
    command = 'simulation '+params_file+' '+config_filename        
    os.system(command)
    
    print("phi = {}, N={},rng_seed={} lbp".format(phi,L,rng_seed))
    
    rog_filename=results_dir+'lbp_'+str(rng_seed)+'.csv'
    time_filename=results_dir+'time_'+str(rng_seed)+'.csv'
    command = 'lb_bonds_clusterwise_invA ' + config_filename +' ' + rog_filename + ' ' + time_filename
    os.system(command)    

#phi=0.15    
seeds=100
seed_pct=100
dim = 3

#phi_range = [0.15, 0.175, 0.20, 0.225, 0.25, 0.275, 0.30]
#N_range = [4000, 8000, 10000, 12000, 14000, 16000, 20000, 24000]
#N_range = [8000]

#phi_range = [0.35, 0.375, 0.4, 0.425, 0.45, 0.475, 0.5]
#phi_range = [0.65, 0.675, 0.7, 0.725, 0.75, 0.775, 0.8]
#phi_range = [0.8]
#L_range   = [500, 1000, 2000, 4000, 8000, 16000, 32000, 64000]
#L_range = [200]

phi_range = [0.32, 0.32, 0.33, 0.34, 0.35, 0.36, 0.37, 0.38, 0.39, 0.40]
L_range = [50]

for i in phi_range:
    for L in L_range:
        create_dirs(i, L, seed_pct, dim)
        Parallel(n_jobs=10)(delayed(run_simulation)(i, L, seed_pct, rng_seed) for rng_seed in range(seeds))
        
"""phi_range = [0.35, 0.6]
L_range = [50, 100]

phi_rngs = np.ones(seeds)
L_rngs   = np.ones(seeds)

for i in range(seeds):
    
    phi_rngs[i] = phi_range[0] + (phi_range[1] - phi_range[0]) * random()
    L_rngs[i]   = L_range[0] + (L_range[1] - L_range[0]) * random()
    L_rngs[i]   = int(L_rngs[i])
    
    
create_dirs_for_mk_table(dim)
#for rng_seed in range(seeds):
Parallel(n_jobs=10)(delayed(run_simulation_for_random_params)(phi_rngs[rng_seed], L_rngs[rng_seed], rng_seed) for rng_seed in range(seeds))
"""