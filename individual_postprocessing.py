#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon May  3 15:11:49 2021

@author: samarth
"""

import pandas as pd
import os
import sys
from parsingscript.parser_script import parse_config_file

argsize  = len(sys.argv)

if argsize < 3:
    print("Too few arguments try again.")

else:

    filepath   = sys.argv[1]
    modulename = sys.argv[2]
    
    config_filename = filepath.split('/')
    config_filename = config_filename[-1].split('.')
    foldername      = config_filename[0]
    parse_config_file(filepath)
    
    out_filename = './modules/'+modulename+'.out'
    if (os.path.isfile(out_filename) == False):
        os.system('g++ -I ./include/ -std=c++11 -O3 ./modules/'+modulename+'.cpp -o ./modules/'+modulename+'.out')
    
    command     = out_filename +' '+ foldername
    os.system(command)