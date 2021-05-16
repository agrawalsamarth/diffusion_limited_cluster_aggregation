#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Apr 19 08:49:28 2021

@author: samarth
"""

import pandas as pd
import os
import numpy as np

def parse_config_file(filepath):
    
    folder = './parsed_file/'
    params_flag = True
    col_flag    = True

    if (os.path.isdir(folder) == False):
        command = 'mkdir '+folder
        os.system(command)
        
    config_filename = filepath.split('/')
    config_filename = config_filename[-1].replace('.csv', '')
    foldername      = config_filename
    
    folder = folder + foldername + "/"
    
    if (os.path.isdir(folder) == False):
        command = 'mkdir '+folder
        os.system(command)
    
    fp = open(filepath, 'r')
    line = fp.readline().strip()    
    txt = line.split('=')
    headers = int(txt[1])
    
    count = 1
    
    N_flag = False
    D_flag = False
    maxAttachments_Flag = False
    lattice_Flag = False
    folded_Flag  = False
    
    while count < headers:
        
        line = fp.readline().strip()
        txt  = line.split('=')
        desc = txt[0]
        val  = float(txt[1])
        
        if desc == 'N':
            N = int(val)
            N_flag = True
    
        if desc == 'D':
            D = int(val)
            D_flag = True
        
        if desc == 'maxAttachments':
            maxAttachments = int(val)
            maxAttachments_Flag = True
            
        if desc == 'lattice':
            lattice = (int)(val)
            lattice_Flag = True
            
        if desc == 'folded':
            folded = (int)(val)
            folded_Flag = True
            
        if desc == 'iters':
            iters = (int)(val)
            
        if desc == 'seedMass':
            seedMass = val
            
        if desc == 'alpha':
            alpha = val
            
        if desc == 'phi':
            phi = val
            
        if desc == 'columns':
            columns = val
            
        count += 1
    
    fp.close()

    if 'N' in locals():
        filename = folder+'N.csv'
        fp = open(filename, 'w')
        #N = (int)(N)
        fp.write(str(N))   
        fp.close()

    if 'D' in locals():    
        filename = folder+'D.csv'
        fp = open(filename, 'w')
        #D = (int)(D)
        fp.write(str(D))
        fp.close()

    if 'maxAttachments' in locals():    
        filename = folder+'max_attachments.csv'
        fp = open(filename, 'w')
        #maxAttachments = (int)(maxAttachments)
        fp.write(str(maxAttachments))    
        fp.close()

    if 'lattice' in locals():    
        filename = folder+'lattice.csv'
        fp = open(filename, 'w')
        fp.write(str(lattice))   
        fp.close()

    if 'iters' in locals():    
        filename = folder+'iters.csv'
        fp = open(filename, 'w')
        fp.write(str(iters))
        fp.close()

    if 'seedMass' in locals():    
        filename = folder+'seedMass.csv'
        fp = open(filename, 'w')
        fp.write(str(seedMass))    
        fp.close()

    if 'alpha' in locals():
        filename = folder+'alpha.csv'
        fp = open(filename, 'w')
        fp.write(str(alpha))    
        fp.close()
    
    if 'folded' in locals():
        filename = folder+'folded.csv'
        fp = open(filename, 'w')
        fp.write(str(folded))    
        fp.close()
        
    if 'phi' in locals():
        filename = folder+'phi.csv'
        fp = open(filename, 'w')
        fp.write(str(phi))    
        fp.close()
        
    if 'columns' in locals():
        filename = folder+'columns.csv'
        fp = open(filename, 'w')
        fp.write(str(columns))    
        fp.close()
        
    if N_flag == False:
        print("Please provide N\n")
        params_flag = False
    if D_flag == False:
        print("Please provide D\n")
        params_flag = False
    if maxAttachments_Flag == False:
        print("Please provide maxAttachments\n")
        params_flag = False
    if lattice_Flag == False:
        print("Please provide lattice\n")
        params_flag = False
    if folded_Flag == False:
        print("Please provide folded\n")
        params_flag = False
        
    if params_flag == False:
        return params_flag
            
    lo_hi    = np.zeros((D,2))
    periodic = np.zeros((D,1))
    
    lo_hi_flag    = np.zeros((D,2))
    periodic_flag = np.zeros((D,1))
    
    fp = open(filepath, 'r')
    
    count = 0
    
    periodic_list = []
    
    for axis in range(D):
        periodic_list.append('x'+str(axis)+'_periodic')
        
    lo_list = []

    for axis in range(D):
        lo_list.append('x'+str(axis)+'_lo')
        
    hi_list = []

    for axis in range(D):
        hi_list.append('x'+str(axis)+'_hi')
        
    while count < headers:
    
        line = fp.readline().strip()
        txt  = line.split('=')
        desc = txt[0]
        val  = float(txt[1])
        
        if desc in periodic_list:
        
            axis = periodic_list.index(desc)
            periodic[axis] = val
            periodic_flag[axis] = 1
            
        if desc in lo_list:
            
            axis = lo_list.index(desc)
            lo_hi[axis][0]      = val
            lo_hi_flag[axis][0] = 1
                
        if desc in hi_list:
            
            axis = hi_list.index(desc)  
            lo_hi[axis][1]      = val
            lo_hi_flag[axis][1] = 1
            
        count += 1
        
    fp.close()
    
    for axis in range(D):
        if periodic_flag[axis] == 0:
            print('Please provide x'+str(axis)+'_periodic\n')
                  
    for axis in range(D):
        if lo_hi_flag[axis][0] == 0:
            print('Please provide x'+str(axis)+'_lo\n')
        if lo_hi_flag[axis][1] == 0:
            print('Please provide x'+str(axis)+'_hi\n')

    
    filename = folder+'lo_hi.csv'
    fp = open(filename, 'w')
    
    for i in range(D):
        fp.write(str(lo_hi[i][0])+','+str(lo_hi[i][1])+'\n')
        
    fp.close()
    
    periodic = periodic.astype(int)
    filename = folder+'periodic.csv'
    fp = open(filename, 'w')
    
    for i in range(D):
        fp.write(str(periodic[i][0])+'\n')
        
    fp.close()
    
    data_df   = pd.read_csv(filepath, skiprows=headers)
    col_names = data_df.columns

    ext_cols  = []
    
    for axis in range(D):
        ext_cols.append('x'+str(axis))
        
    for coord in ext_cols:
        if (coord not in col_names):
            print('Please provide '+coord)
            params_flag = False
    
    sample_df = pd.DataFrame(data_df, columns = ext_cols)
    
    for col in ext_cols:
        sample_df[col] = sample_df[col].astype(float)
    
    filename = folder+'config.csv'
    sample_df.to_csv(filename, index=False, header=False)
    
    ext_cols  = ['attachments']
    sample_df = pd.DataFrame(data_df, columns = ext_cols)
    
    for coord in ext_cols:
        if (coord not in col_names):
            print('Please provide '+coord+'\n')
            params_flag = False
    
    for col in ext_cols:
        sample_df[col] = sample_df[col].astype(int)
    
    filename = folder+'num_attachments.csv'
    sample_df.to_csv(filename, index=False, header=False)
    

    ext_cols  = ['assignedSeedStatus']
    sample_df = pd.DataFrame(data_df, columns = ext_cols)
    
    for seed in ext_cols:
        if (seed not in col_names):
            print('Please provide '+seed+'\n')
            params_flag = False
    
    for col in ext_cols:
        sample_df[col] = sample_df[col].astype(int)
    
    filename = folder+'original_seed.csv'
    sample_df.to_csv(filename, index=False, header=False)
    
    ext_cols  = ['currentSeedStatus']
    sample_df = pd.DataFrame(data_df, columns = ext_cols)
    
    for seed in ext_cols:
        if (seed not in col_names):
            print('Please provide '+seed+'\n')
            params_flag = False
    
    for col in ext_cols:
        sample_df[col] = sample_df[col].astype(int)
    
    filename = folder+'current_seed.csv'
    sample_df.to_csv(filename, index=False, header=False)
    
    ext_cols  = ['diameter']
    sample_df = pd.DataFrame(data_df, columns = ext_cols)
    
    for dia in ext_cols:
        if (seed not in col_names):
            print('Please provide '+dia+'\n')
            params_flag = False
    
    filename = folder+'diameter.csv'
    sample_df.to_csv(filename, index=False, header=False)
    
    for i in range(1,maxAttachments+1):
    
        att_col = 'att_'+str(i)
       
        if (att_col not in col_names):
            print('Please provide '+att_col+'\n')
            params_flag = False
        
        sample_df = pd.DataFrame(data_df, columns = [att_col])
        sample_df[att_col] = sample_df[att_col].fillna(N).astype(int)
        filename = folder+att_col+'.csv'
        sample_df.to_csv(filename, index=False, header=False)
        
    return params_flag
            