#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Feb 19 13:36:32 2025

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

#def voronoi(config_filename, result_filename):

    #command="../../postprocessing/bin/voronoi_volumes "+config_filename+" "+ result_filename
    #os.system(command)   

    
"""all_folders = []

with open("list-samarth-configs.txt", "r") as file:
    for line in file:
        all_folders.append(line)
        
foi = ['scaling_ns']
config_filenames = []

for name in foi:
    for folder in all_folders:
        
        if name in folder:
            config_filenames.append(folder.strip())"""

result_filenames = []

phi_vals = ['phi=0.05000/', 'phi=0.10788/']
agg_vals = ['0.200/', '0.250/', '0.300/']
N_val = 'N=100000/'

prefix = '/data/mkroeger/vsandra/2024/dlma/scripts/scaling_vs_agg_tolerance/high_porosity_scaling/config_files/'

for phi in phi_vals:
    for agg in agg_vals:
        for i in range(100):
            
            file = prefix + phi + agg + N_val + str(i) + '.csv'
            result_filenames.append(file)
            

phi_vals = ['phi=0.05000/', 'phi=0.10788/']
agg_vals = ['0.300/']
ns_val   = ['ns=0.100/', 'ns=1.000/', 'ns=0.500/']
N_val = 'N=100000/'

prefix = '/data/mkroeger/vsandra/2024/dlma/scripts/scaling_vs_agg_tolerance/scaling_ns/config_files/'

for phi in phi_vals:
    for agg in agg_vals:
        for ns in ns_val:
            for i in range(100):
            
                file = prefix + phi + agg + N_val + ns + str(i) + '.csv'
                result_filenames.append(file)
                
with open('srt_filenames.txt', 'w') as f:
    for line in result_filenames:
        f.write(f"{line}\n")        