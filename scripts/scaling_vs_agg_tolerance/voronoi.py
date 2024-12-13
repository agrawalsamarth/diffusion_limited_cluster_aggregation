#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Dec 13 15:25:01 2024

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

def voronoi(config_filename, result_filename):

    command="../../postprocessing/bin/voronoi_volumes "+config_filename+" "+ result_filename
    os.system(command)   

    
all_folders = []

with open("list-samarth-configs.txt", "r") as file:
    for line in file:
        all_folders.append(line)
        
foi = ['high_porosity_scaling', 'scaling_ns']
config_filenames = []

for name in foi:
    for folder in all_folders:
        
        if name in folder:
            config_filenames.append(folder.strip())

result_filenames = []

for filename in config_filenames:
    
    filename = filename.replace('config_files', 'results')
    idx = filename.rfind('/')
    filename = filename[:idx+1]+'vv_'+filename[idx+1:]
    result_filenames.append(filename.strip())


size = len(result_filenames)

os.system('cd ../../simulation_files; make simulation')
os.system('cd ../../postprocessing/; make all')

num_processors = 80
Parallel(n_jobs=num_processors)(delayed(voronoi)(config_filenames[i], result_filenames[i]) for i in range(size))

#os.system('cd ../../simulation_files/; make clean')
#os.system('cd ../../postprocessing/; make clean')

