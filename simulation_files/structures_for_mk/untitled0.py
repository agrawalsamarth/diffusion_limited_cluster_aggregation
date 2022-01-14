# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import pandas as pd
from matplotlib import pyplot as plt

df = pd.read_csv('mt_1.csv', skiprows=16)
plt.scatter(df['x0'], df['x1'],s=1)