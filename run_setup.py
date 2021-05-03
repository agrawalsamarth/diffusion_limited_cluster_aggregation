#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon May  3 08:25:46 2021

@author: samarth
"""

import pandas as pd
import os
from postprocessing import postprocessing_module
from matplotlib import pyplot as plt

results_folder = 'results'
if (os.path.isdir(results_folder) == False):
    os.system('mkdir '+results_folder)
    
plots_folder = 'plots/'
if (os.path.isdir(plots_folder) == False):
    os.system('mkdir '+plots_folder)

os.system('g++ -std=c++11 -O3 ./simulation_files/main.cpp -o dlca.out')
seed_pct = [0]

plt.figure(figsize=(6,6), dpi=300)

for seed in seed_pct:
    os.system('./dlca.out '+str(seed))
    filepath   = './config_files/'+str(seed)+'.csv'
    modulename = 'module_2'
    postprocessing_module(filepath, modulename)
    filepath   = './results/unfolded_coords.csv' 
    coords_df  = pd.read_csv(filepath, names=['x','y'])
    plt.scatter(coords_df['x'], coords_df['y'],s=1)
    filepath   = plots_folder+str(seed)+'.jpg'
    plt.savefig(filepath, format='jpg')
    plt.clf()
