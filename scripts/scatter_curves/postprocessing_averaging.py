#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Jan 21 20:00:46 2024

@author: samarth
"""

import pandas as pd

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

   
def calc_ave(phi, seeds):

    phi_str = format(phi, '.3f')
    results_dir = './test/results/phi='+phi_str+'/'    
    filename = results_dir+'0.csv'
    
    df = pd.DataFrame()
    df_temp = pd.read_csv(filename)
    
    df['q'] = df_temp['q']
    
    for i in range(seeds):
        df_temp      = pd.read_csv(results_dir+str(i)+'.csv') 
        col_name     = 'Sq_'+str(i)
        df[col_name] = df_temp['Sq']
    
    df = df[df['q'] < 1]
    df['Sq'] = df.iloc[:,1:].mean(axis=1)
    return df[['q', 'Sq']]


seeds=20
phi_range = [0.025, 0.05]

plt.xscale('log')
plt.yscale('log')

for phi in phi_range:
    
    df = calc_ave(phi, seeds)
    plt.plot(df['q'], df['Sq'], label='phi='+format(phi,'.3f'))
    
plt.legend()