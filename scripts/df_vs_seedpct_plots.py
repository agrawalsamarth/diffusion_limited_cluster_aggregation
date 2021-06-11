#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Jun  6 21:19:54 2021

@author: samarth
"""

import pandas as pd
import os 
from matplotlib import pyplot as plt
from pathlib import Path
import time
from sklearn.linear_model import LinearRegression
import numpy as np

parent_folder  = './results/'
df_vs_seed     = pd.DataFrame(columns=['seed_pct','d_f'])
r_min          = 5
rog_prefactor  = 0.8

model = LinearRegression()

for seed_pct in range(1,101):
    
    seed_folder=parent_folder+str(seed_pct)+'/'
    rog_df  = pd.DataFrame(columns=['rog'])
    pair_df = pd.DataFrame() 
    
    for rng_seed in range(10):
                
        rog_filename=seed_folder+'radius_of_gyration_'+str(seed_pct)+'_'+str(rng_seed)+'.csv'        
        rog_temp_df = pd.read_csv(rog_filename)
        
        rog_df.loc[len(rog_df),'rog'] = rog_temp_df.iloc[0]['Radius_of_Gyration']
        
    rog_mean = rog_df['rog'].mean()
    r_cutoff = rog_prefactor * rog_mean
    
    pair_filename=seed_folder+'pair_correlation_'+str(seed_pct)+'_0.csv'
    pair_temp_df=pd.read_csv(pair_filename)
    pair_temp_df=pair_temp_df[(pair_temp_df['r'] >= r_min) & (pair_temp_df['r'] <= r_cutoff)]
    pair_df['r']=pair_temp_df['r']
    
    
    for rng_seed in range(10):
        
        pair_filename=seed_folder+'pair_correlation_'+str(seed_pct)+'_'+str(rng_seed)+'.csv'
        pair_temp_df=pd.read_csv(pair_filename)
        pair_temp_df=pair_temp_df[(pair_temp_df['r'] >= r_min) & (pair_temp_df['r'] <= r_cutoff)]
        
        colname = 'col_'+str(rng_seed)
        pair_df[colname] = pair_temp_df['F(r)']
        
    pair_df['F(r)'] = pair_df.iloc[:,1:].mean(axis=1)
    pair_df = pair_df.iloc[::5]
    
    x = np.log(pair_df['r'].values)
    x = x.reshape(-1,1)
    y = np.log(pair_df['F(r)'].values)
    
    model.fit(x,y)
    df_vs_seed.loc[len(df_vs_seed)] = [seed_pct,model.coef_[0]]
    
plt.figure(dpi=300)
plt.scatter(df_vs_seed['seed_pct'], df_vs_seed['d_f'])
plt.xlabel(r'$n_s$')
plt.ylabel(r'$d_f$')
#plt.text(15,1.6,'N=2048')
plt.savefig('./plots/df_vs_seedpct.pdf', format='pdf')