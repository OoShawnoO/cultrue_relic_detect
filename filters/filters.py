import os

directory = "filters/"
output_file = "filters.h"
filters = []
with open(directory+output_file, "w") as f:
    f.write("#ifndef FILTERS_H\n#define FILTERS_H\n")
    for filename in os.listdir(directory):
        if filename.endswith(".h") and filename != output_file:
            f.write("#include \"" + filename + "\"\n")
            filters.append(filename.split('.')[0])
    for filter in filters:
        f.write("FILTER("+filter+")\n")
    f.write("#endif")