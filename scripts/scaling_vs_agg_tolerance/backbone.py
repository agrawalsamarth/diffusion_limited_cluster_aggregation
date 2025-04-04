#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Dec 13 23:52:59 2024

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

def skeletonize(config_filename, skeleton_filename):

    command="../../postprocessing/bin/create_dangling_free_config "+config_filename+" "+ skeleton_filename
    os.system(command)   

def coord_number(config_filename, result_filename):
    
    df = pd.read_csv(config_filename, skiprows=19)
    df[['attachments']].to_csv(result_filename, header=True, index=False)


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
            
skeleton_filenames = []

for filename in config_filenames:
    
    idx = filename.rfind('/')
    filename = filename[:idx+1]+'skeleton_'+filename[idx+1:]
    skeleton_filenames.append(filename)
    
os.system('cd ../../simulation_files; make create_dangling_free_config')
os.system('cd ../../postprocessing/; make all')

size=len(skeleton_filenames)
num_processors = 80
Parallel(n_jobs=num_processors)(delayed(skeletonize)(config_filenames[i], skeleton_filenames[i]) for i in range(size))

result_filenames = []

for filename in config_filenames:
    
    filename = filename.replace('config_files', 'results')
    idx = filename.rfind('/')
    filename = filename[:idx+1]+'full_Zdist_'+filename[idx+1:]
    result_filenames.append(filename.strip())

num_processors = 80
Parallel(n_jobs=num_processors)(delayed(coord_number)(config_filenames[i], result_filenames[i]) for i in range(size))

result_filenames = []

for filename in skeleton_filenames:
    
    filename = filename.replace('config_files', 'results')
    idx = filename.rfind('/')
    filename = filename[:idx+1]+'skeleton_Zdist_'+filename[idx+1:]
    result_filenames.append(filename.strip())

num_processors = 80
Parallel(n_jobs=num_processors)(delayed(coord_number)(skeleton_filenames[i], result_filenames[i]) for i in range(size))


#os.system('cd ../../simulation_files/; make clean')
#os.system('cd ../../postprocessing/; make clean')

