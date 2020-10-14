#!/usr/bin/env python3
import json
import re

# Gd equal to non-gd for these maps

data = {}
targets = ["wbls_1pct", "wbls_3pct", "wbls_1pct_gd", "wbls_3pct_gd"]
coverages = ["10", "20", "40", "80"]
radii = ["4700", "5700", "6700", "7700", "8800"]

for t in targets:
  data[t] = {}
  for c in coverages:
    covnum = "0."+c
    data[t][covnum] = {}
    for r in radii:
      filename=f'{re.sub(r"_gd","",t)}_WM_0820_{r}mm_{c}pct.csv'
      data[t][covnum][r] = filename

with open('response_dictionary.json', 'w') as ff:
  json.dump(data, ff, indent=2)
