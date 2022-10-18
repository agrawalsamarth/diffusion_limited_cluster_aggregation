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

def create_dirs(phi, N, seed_pct, dim):

    global D
    
    D = dim

    global params_dir
    
    phi_str = format(phi, '.5f')
    
    params_dir = './'+str(D)+'d/params/phi='+phi_str+'/seed_pct='+str(seed_pct)+'/N='+str(N)+'/'
    path = Path(params_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass
    
    
    global results_dir
    
    results_dir = './'+str(D)+'d/results/phi='+phi_str+'/seed_pct='+str(seed_pct)+'/N='+str(N)+'/'
    path = Path(results_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass
    
    global config_files_dir
    
    config_files_dir = './'+str(D)+'d/config_files/phi='+phi_str+'/seed_pct='+str(seed_pct)+'/N='+str(N)+'/'
    path = Path(config_files_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass     

def run_simulation(phi, N, seed_pct, rng_seed):
    
    lattice=1
    x0_bc="periodic"
    x1_bc="periodic"
    x2_bc="periodic"
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
    file.write("N="+str(N)+"\n")
    file.write("seed_pct="+str(seed_pct)+"\n")
    file.write("x0_bc="+str(x0_bc)+"\n")
    file.write("x1_bc="+str(x1_bc)+"\n")
    file.write("x2_bc="+str(x2_bc)+"\n")
    file.write("x3_bc="+str(x2_bc)+"\n")
    file.write("phi="+str(phi)+"\n")
    file.write("alpha="+str(alpha)+"\n")
    file.write("seedMass="+str(seedMass)+"\n")
    file.write("rng_seed="+str(rng_seed)+"\n")        
    file.close()
    
    print("seed_pct = {}, phi = {}, N={},rng_seed={} simulation".format(seed_pct,phi,N,rng_seed))

    config_filename=config_files_dir+str(rng_seed)+'.csv'
    command = 'simulation '+params_file+' '+config_filename        
    os.system(command)
    
    """print("seed_pct = {}, phi = {}, N={},rng_seed={} lbp".format(seed_pct,phi,N,rng_seed))
    
    rog_filename=results_dir+'lbp_'+str(rng_seed)+'.csv'
    time_filename=results_dir+'time_'+str(rng_seed)+'.csv'
    command = 'lb_bonds_clusterwise_invA ' + config_filename +' ' + rog_filename + ' ' + time_filename
    os.system(command)
    
    
    command = 'sh -c \'echo 1 >/proc/sys/vm/drop_caches\''
    #print(command)
    os.system(command)
    command = 'sh -c \'echo 2 >/proc/sys/vm/drop_caches\''
    #print(command)
    os.system(command)
    command = 'sh -c \'echo 3 >/proc/sys/vm/drop_caches\''
    #print(command)
    os.system(command)"""
    
def create_dirs_for_mk_table():

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



def run_simulation_for_random_params(phi, N, rng_seed):
    
    lattice=1
    x0_bc="periodic"
    x1_bc="periodic"
    x2_bc="periodic"
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
    file.write("N="+str(N)+"\n")
    file.write("seed_pct="+str(seed_pct)+"\n")
    file.write("x0_bc="+str(x0_bc)+"\n")
    file.write("x1_bc="+str(x1_bc)+"\n")
    file.write("x2_bc="+str(x2_bc)+"\n")
    file.write("phi="+str(phi)+"\n")
    file.write("alpha="+str(alpha)+"\n")
    file.write("seedMass="+str(seedMass)+"\n")
    file.write("rng_seed="+str(rng_seed)+"\n")        
    file.close()
    
    print("phi = {}, N={},rng_seed={} simulation".format(phi,N,rng_seed))

    config_filename=config_files_dir+str(rng_seed)+'.csv'
    command = 'simulation '+params_file+' '+config_filename        
    os.system(command)
    
    """print("phi = {}, N={},rng_seed={} lbp".format(phi,N,rng_seed))
    
    rog_filename=results_dir+'lbp_'+str(rng_seed)+'.csv'
    time_filename=results_dir+'time_'+str(rng_seed)+'.csv'
    command = 'lb_bonds_clusterwise_invA ' + config_filename +' ' + rog_filename + ' ' + time_filename
    os.system(command)"""

    

seed(18)
seeds=50
seed_pct_range=[100]
dim = 3

#phi_range = [0.125]
phi_range = []

for i in range(5,9):
    phi_range.append(0.125 * (1.125**i))

N_range = [64000]

phi_rngs = np.ones(seeds)
N_rngs   = np.ones(seeds)


"""for i in range(seeds):
    
    phi_rngs[i] = phi_range[0] + (phi_range[1] - phi_range[0]) * random()
    N_rngs[i]   = N_range[0] + (N_range[1] - N_range[0]) * random()
    N_rngs[i]   = int(N_rngs[i])
    
    
create_dirs_for_mk_table()
for rng_seed in range(seeds):
    run_simulation_for_random_params(phi_rngs[rng_seed], N_rngs[rng_seed], rng_seed)"""

for seed_pct in seed_pct_range:
    for phi in phi_range:
        for N in N_range:
            create_dirs(phi, N, seed_pct, dim)
            Parallel(n_jobs=10)(delayed(run_simulation)(phi, N, seed_pct, rng_seed) for rng_seed in range(seeds))
