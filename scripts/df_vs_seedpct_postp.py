#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Jun  6 11:28:31 2021

@author: samarth
"""

import pandas as pd
import os 
from matplotlib import pyplot as plt
from pathlib import Path
import time

rog_module  = 'radius_of_gyration'
pair_module = 'pair_correlation'

rog_out_filename  = './modules/'+rog_module+'.out'
pair_out_filename = './modules/'+pair_module+'.out'


if (os.path.isfile(rog_out_filename) == False):
    os.system('g++ -I ./include/ -std=c++11 -O3 ./modules/'+rog_module+'.cpp -o ./modules/'+rog_module+'.out')

if (os.path.isfile(pair_out_filename) == False):
    os.system('g++ -I ./include/ -std=c++11 -O3 ./modules/'+pair_module+'.cpp -o ./modules/'+pair_module+'.out')
    
bin_size      = 0.1
parent_folder  = './results/'



for seed_pct in range(1,2):
    
    seed_folder=parent_folder+str(seed_pct)+'/'
    path = Path(seed_folder)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        print("1")
    else:
        print("2")
        
    #time.sleep(1)
    
    for rng_seed in range(10):
        
        config_folder   = './config_files/df_vs_seedpct_'+str(seed_pct)+'/'
        config_filename=config_folder+str(rng_seed)+'.csv'
        
        rog_filename=seed_folder+'radius_of_gyration_'+str(seed_pct)+'_'+str(rng_seed)+'.csv'
        rog_command = rog_out_filename+' '+config_filename+' '+rog_filename
        os.system(rog_command)
        
        pair_filename=seed_folder+'pair_correlation_'+str(seed_pct)+'_'+str(rng_seed)+'.csv'
        pair_command = pair_out_filename+' '+config_filename+' '+str(bin_size)+' '+pair_filename
        os.system(pair_command)
        
        print("seed_pct={} rng_seed={}".format(seed_pct, rng_seed))