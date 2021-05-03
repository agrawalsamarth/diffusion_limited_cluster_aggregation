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

    if (os.path.isdir(folder) == False):
        command = 'mkdir '+folder
        os.system(command)
    
    fp = open(filepath, 'r')
    line = fp.readline().strip()    
    txt = line.split('=')
    headers = int(txt[1])
    
    count = 1
    
    while count < headers:
        
        line = fp.readline().strip()
        txt  = line.split('=')
        desc = txt[0]
        val  = float(txt[1])
        
        if desc == 'N':
            N = int(val)
    
        if desc == 'D':
            D = int(val)
        
        if desc == 'maxAttachments':
            maxAttachments = int(val)
    
            
        if desc == 'x0_lo':
            x0_lo = val
        
        if desc == 'x0_hi':
            x0_hi = val
            
        if desc == 'x1_lo':
            x1_lo = val
            
        if desc == 'x1_hi':
            x1_hi = val
            
        if desc == 'x2_lo':
            x2_lo = val
            
        if desc == 'x2_hi':
            x2_hi = val
            
        if desc == 'x0_periodic':
            x0_p = int(val)
            
        if desc == 'x1_periodic':
            x1_p = int(val)
            
        if desc == 'x2_periodic':
            x2_p = int(val)
            
        
        count += 1
    
    fp.close()
    
    filename = folder+'N.csv'
    fp = open(filename, 'w')
    N = (int)(N)
    fp.write(str(N))   
    fp.close()
    
    filename = folder+'D.csv'
    fp = open(filename, 'w')
    D = (int)(D)
    fp.write(str(D))
    fp.close()
    
    filename = folder+'max_attachments.csv'
    fp = open(filename, 'w')
    maxAttachments = (int)(maxAttachments)
    fp.write(str(maxAttachments))    
    fp.close()
    
    L    = np.zeros((D,1))
    L[0] = (x0_hi - x0_lo)
    L[1] = (x1_hi - x1_lo)
    if (D==3):
        L[2] = (x2_hi - x2_lo)
    
    filename = folder+'L.csv'
    fp = open(filename, 'w')
    
    for i in range(D):
        fp.write(str(L[i][0])+'\n')
        
    fp.close()
    
    p    = np.zeros((D,1))
    p[0] = (x0_p)
    p[1] = (x1_p)
    if (D==3):
        p[2] = (x2_p)

    p = p.astype(int)
    
    filename = folder+'periodic.csv'
    fp = open(filename, 'w')
    
    for i in range(D):
        fp.write(str(p[i][0])+'\n')
        
    fp.close()
    
    data_df   = pd.read_csv(filepath, skiprows=headers)

    if (D==3):    
        ext_cols  = ['x', 'y', 'z']
    if (D==2):
        ext_cols  = ['x', 'y']
    
    sample_df = pd.DataFrame(data_df, columns = ext_cols)
    
    for col in ext_cols:
        sample_df[col] = sample_df[col].astype(float)
    
    filename = folder+'config.csv'
    sample_df.to_csv(filename, index=False, header=False)
    
    ext_cols  = ['attachments']
    sample_df = pd.DataFrame(data_df, columns = ext_cols)
    
    for col in ext_cols:
        sample_df[col] = sample_df[col].astype(int)
    
    filename = folder+'num_attachments.csv'
    sample_df.to_csv(filename, index=False, header=False)
    
    columns   = data_df.columns
    att_str   = 'att_'
    
    for col in columns:
        
        if att_str in col:
            ext_cols  = [col]
            sample_df = pd.DataFrame(data_df, columns = ext_cols)
            sample_df[col] = sample_df[col].fillna(N).astype(int)
            filename = folder+col+'.csv'
            sample_df.to_csv(filename, index=False, header=False)
            
