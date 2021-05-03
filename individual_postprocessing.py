#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon May  3 15:11:49 2021

@author: samarth
"""

import pandas as pd
import os
import sys
from postprocessing import postprocessing_module

argsize  = len(sys.argv)

if argsize < 3:
    print("Too few arguments try again.")

else:
    filepath   = sys.argv[1]
    modulename = sys.argv[2]
    postprocessing_module(filepath, modulename)
    