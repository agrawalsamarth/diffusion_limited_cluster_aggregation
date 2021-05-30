#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon May  3 15:11:49 2021

@author: samarth
"""

import pandas as pd
import os
import sys

argsize  = len(sys.argv)

if argsize < 3:
    print("Too few arguments try again.")
    
else:
    
    filepath   = sys.argv[1]
    modulename = sys.argv[2]
    
    if (os.path.isfile(filepath)):
        
        out_filename = './modules/'+modulename+'.out'
        
        if (os.path.isfile(out_filename) == False):
            os.system('g++ -I ./include/ -std=c++11 -O3 ./modules/'+modulename+'.cpp -o ./modules/'+modulename+'.out')
            
        command     = out_filename +' '+ filepath
        
        for i in range(3,argsize):
            command = command + ' ' + sys.argv[i]
        
        os.system(command)
        

"""else:

    filepath   = sys.argv[1]
    modulename = sys.argv[2]
    
    config_filename = filepath.split('/')
    config_filename = config_filename[-1].replace('.csv','')
    foldername      = config_filename
    parse_test      = parse_config_file(filepath)
    
    if parse_test:
        
        out_filename = './modules/'+modulename+'.out'
        if (os.path.isfile(out_filename) == False):
            os.system('g++ -I ./include/ -std=c++11 -O3 ./modules/'+modulename+'.cpp -o ./modules/'+modulename+'.out')
        
        command     = out_filename +' '+ foldername
        
        for i in range(3,argsize):
            command = command + ' ' + sys.argv[i]
        
        os.system(command)"""
        
