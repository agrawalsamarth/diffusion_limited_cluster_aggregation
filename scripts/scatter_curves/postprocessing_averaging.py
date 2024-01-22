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

    phi_str = format(phi, '.4f')
    results_dir = './act_values/results/phi='+phi_str+'/'    
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


seeds=50
phi_range = [(100-96.21)/100, (100-94.83)/100, (100-93.01)/100, (100-91.08)/100, (100-88.20)/100]

"""plt.figure(figsize=(6,6), dpi=300)
plt.xscale('log')
plt.yscale('log')
plt.xlabel(r'$q$')
plt.ylabel(r'$I(q)$')"""

for phi in phi_range:
    
    df = calc_ave(phi, seeds)
    #plt.plot(df['q'], df['Sq'], label='phi='+format(phi,'.3f'))
    filename = './act_values/'+format(phi, '.4f') 
    df.to_csv(filename)
    
#plt.legend()