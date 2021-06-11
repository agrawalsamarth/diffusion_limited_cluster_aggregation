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


lattice=1
D=2
N=4096
x0_periodic=1
x1_periodic=1
x0_L=256
x1_L=256
alpha=-0.5
seedMass=1e12

out_filename = './simulation_files/simulation.out'
params_file     = './simulation_files/params.csv'

if (os.path.isfile(out_filename) == False):
    os.system('g++ -I ./simulation_files/include/ -std=c++11 -O3 ./simulation_files/main.cpp -o '+out_filename)

for seed_pct in range(5,10):
    
    for rng_seed in range(10):
    
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
        #file.write("seed_pct="+str(seed_pct)+"\n")
        file.write("N_s="+str(seed_pct)+"\n")
        file.write("rng_seed="+str(rng_seed)+"\n")        
        file.close()
        
        config_folder   = './config_files/df_vs_N_s_'+str(seed_pct)+'/'
        path = Path(config_folder)
        
        try:
            path.mkdir(parents=True, exist_ok=True) 
        except OSError:
            pass
        else:
            pass
        
        config_filename=config_folder+str(rng_seed)+'.csv'
        command = './simulation_files/simulation.out '+params_file+' '+config_filename        
        os.system(command)
        
        print("seed_pct = {}, rng_seed={}".format(seed_pct,rng_seed))