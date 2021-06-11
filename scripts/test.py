#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Jun  3 14:10:11 2021

@author: samarth
"""

import pandas as pd
from matplotlib import pyplot as plt
import os
from pathlib import Path

param  = 'seedpct'
values = [1,3,5,10,20,50,80,100]
N = 2048
total_files = 10

num_att_df = pd.DataFrame(columns=['N_s', 'Z'])

for val in values:
    
    att_df = pd.DataFrame()
    
    for file_number in range(total_files):    
    
        config_filename = './config_files/df_vs_'+param+'_'+str(val)+'/'+str(file_number)+'.csv'
        command = './modules/unfold.out ' + config_filename
        os.system(command)
    
        filename = './postprocessing_results/config_files_df_vs_'+param+'_'+str(val)+'_'+str(file_number)+'/unfolded_config.csv'
        df = pd.read_csv(filename, skiprows=10)
        att_df = att_df.append(df)
        
        """seed_df    = df[df['assignedSeedStatus']==1]
        walkers_df = df[df['assignedSeedStatus']==0]
        
        plt.figure(dpi=300)
        plt.scatter(walkers_df['x0'],walkers_df['x1'],c='blue',s=0.5)
        plt.scatter(seed_df['x0'],seed_df['x1'],c='red',s=10)
        #plt.hist(df['attachments'])
        folder='./plots/individual/'+param+'_'+str(val)+'_hist/'
        
        path = Path(folder)
        
        try:
            path.mkdir(parents=True, exist_ok=True) 
        except OSError:
            pass
        else:
            pass
        
        plt.savefig(folder+str(file_number)+'.pdf', format='pdf')
        plt.clf()"""
        
    """plt.figure(dpi=300)
    plt.hist(att_df['attachments'])
    folder='./plots/individual/'
    filename = folder+param+'_'+str(val)+'_hist.pdf'
    
    
    path = Path(folder)
    
    try:
        path.mkdir(parents=True, exist_ok=True) 
    except OSError:
        pass
    else:
        pass
    
    plt.savefig(filename, format='pdf')
    plt.clf()
    
    num_att_df.loc[len(num_att_df)] = [val, att_df['attachments'].sum()/(N*total_files)]
    
plt.figure(dpi=300)
folder='./plots/individual/'
filename = folder+param+'_Z_vs_seed.pdf'


path = Path(folder)

try:
    path.mkdir(parents=True, exist_ok=True) 
except OSError:
    pass
else:
    pass

plt.plot(num_att_df['N_s'], num_att_df['Z'])
plt.savefig(filename, format='pdf')
plt.clf()"""