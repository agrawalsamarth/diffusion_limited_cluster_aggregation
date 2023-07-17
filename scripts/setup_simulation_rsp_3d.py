#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Jul  3 14:09:31 2023

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
    
    phi_str = format(phi, '.8f')
    
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
    #print(command)
    
    print("phi = {}, L={},rng_seed={} lbp".format(phi,L,rng_seed))
    
    rog_filename=results_dir+'lbp_'+str(rng_seed)+'.csv'
    command = 'lb_bonds_clusterwise_invA ' + config_filename +' ' + rog_filename
    os.system(command)
    
    command = 'rm ' + config_filename
    os.system(command)
    

#phi=0.15    
seeds=100
seed_pct=100
dim = 3


L_range = [10,20,40,60,80,100,150,200]

phi_min = 0.005
phi_max = 0.05
phi_c = 0.311608

phi_min += phi_c
phi_max += phi_c

total_steps = 15
d_phi   = (np.log(phi_max/phi_min))/(total_steps)

phi_range = []

for i in range(total_steps):
    
    temp_phi  = phi_min * np.exp(i * (d_phi))
    phi_range.append(temp_phi)

for L in L_range:
    for i in phi_range:
        create_dirs(i, L, seed_pct, dim)
        Parallel(n_jobs=10)(delayed(run_simulation)(i, L, seed_pct, rng_seed) for rng_seed in range(seeds))
