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

def create_dirs():

    global params_dir   

    params_dir = './params/'
    path = Path(params_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass
    
    global results_dir
    
    results_dir = './results/'
    path = Path(results_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass
    
    global config_files_dir
    
    config_files_dir = './config_files/'
    path = Path(config_files_dir)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass     

def run_simulation(N, rng_seed):
    
    phi=0.01
    D=2
    L=pow((N/phi),1/D) 

    lattice=1
    N_s=1
    x0_periodic=1
    x1_periodic=1
    x0_L=L
    x1_L=L
    alpha=-0.5
    seedMass=1e12
    
    params_file  = params_dir+'params_'+str(rng_seed)+'.csv'
        
    file = open(params_file, "w")
    
    file.write("lattice="+str(lattice)+"\n")
    file.write("D="+str(D)+"\n")
    file.write("N="+str(N)+"\n")
    file.write("x0_periodic="+str(x0_periodic)+"\n")
    file.write("x1_periodic="+str(x1_periodic)+"\n")
    file.write("x0_L="+str(x0_L)+"\n")
    file.write("x1_L="+str(x1_L)+"\n")
    file.write("alpha="+str(alpha)+"\n")
    file.write("seedMass="+str(seedMass)+"\n")
    file.write("N_s="+str(N_s)+"\n")
    file.write("rng_seed="+str(rng_seed)+"\n")        
    file.close()    

    print("N={}, rng_seed={}".format(N,rng_seed))

    config_filename=config_files_dir+str(rng_seed)+'.csv'
    command = 'simulation.out '+params_file+' '+config_filename        
    os.system(command)
    
    rog_filename=results_dir+'rog_'+str(rng_seed)+'.csv'
    command = 'radius_of_gyration ' + config_filename +' ' + rog_filename
    os.system(command)
    
def calc_mean_rog(seeds):
    
    rog_df = pd.DataFrame(columns=['rog'])
    
    for rng_seed in range(seeds):
        
        rog_filename=results_dir+'rog_'+str(rng_seed)+'.csv'
        temp_df = pd.read_csv(rog_filename)
        
        rog_df.loc[len(rog_df)] = temp_df.iloc[0]['Radius_of_Gyration']
        
    return rog_df['rog'].mean()


create_dirs()
rog_df = pd.DataFrame(columns=(['N', 'Rg']))
seeds=10


for N in range(1000,3501,500):
    
    Parallel(n_jobs=-1)(delayed(run_simulation)(N, rng_seed) for rng_seed in range(seeds))
    rog_mean=calc_mean_rog(seeds)
    rog_df.loc[len(rog_df)] = [N, rog_mean]
    
x = np.log(rog_df['N'].values)
x = x.reshape(-1,1)
y = np.log(rog_df['Rg'].values)

model = LinearRegression()
model.fit(x,y)

print(model.coef_[0])

plt.figure(dpi=300)
plt.scatter(rog_df['N'], rog_df['Rg'], color='red')
plt.plot(np.exp(x), np.exp(y))
plt.xscale('log')
plt.yscale('log')
plt.savefig('rg_dla.pdf', format='pdf')