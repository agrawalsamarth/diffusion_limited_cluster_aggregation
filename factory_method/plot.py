#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Aug 19 19:32:22 2021

@author: samarth
"""

import pandas as pd
from matplotlib import pyplot as plt

df = pd.read_csv('unfolded_test.csv', skiprows=15)
plt.scatter(df['x0'], df['x1'],s=1)