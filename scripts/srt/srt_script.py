#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Apr  4 04:21:08 2025

@author: samarth
"""

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

def run_simulation(config_filename, result_filename, num_rays):
    
    command = "../../postprocessing/bin/stochastic_ray_tracing " + config_filename + " " + result_filename + " " + format(num_rays, ".0f")
    os.system(command)


os.system('cd ../../postprocessing/; make stochastic_ray_tracing')

configs = []
results = []
num_rays = 1e6

with open('./srt_filenames.txt', 'r') as file:
      for line in file:
        line = [line.rstrip('\n')]
        configs.append(line)
        
for config in configs:
    config.append(config[0].replace('config_files', 'results'))

    
            

num_processors = 80
Parallel(n_jobs=num_processors)(delayed(run_simulation)(config[0], config[1], num_rays) for config in configs)


os.system('cd ../../postprocessing/; make clean')
