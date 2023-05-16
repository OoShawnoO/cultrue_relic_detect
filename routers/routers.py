import os

directory = "routers/"
output_file = "routers.h"

with open(directory+output_file, "w") as f:
    f.write("#ifndef ROUTERS_H\n#define ROUTERS_H\n")
    for filename in os.listdir(directory):
        if filename.endswith(".h") and filename != output_file:
            f.write("#include \"" + filename + "\"\n")
    f.write("#endif")