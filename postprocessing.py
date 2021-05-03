#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Apr 27 22:02:58 2021

@author: samarth
"""

import os
import pandas as pd
from parsingscript.parser_script import parse_config_file

def postprocessing_module(filepath, modulename):

    parse_config_file(filepath)
    os.system(' g++ -I ./include/ -std=c++11 -O3 ./modules/'+modulename+'.cpp -o ./modules/'+modulename+'.out')
    os.system('./modules/'+modulename+'.out')