#!/usr/bin/env python3

# Open the file
file = open("credits.txt","r")

# Make the start of the line
output = "s8 credits[48][40] = {\n"

# Loop over each line of credits.txt
while True:
    # read the line and get rid of the trailing newline
    line = file.readline()
    # If we reached the end of the file, break out of the loop
    if not line: break

    line = line.rstrip();

    line = (" " * (20 - int(len(line)/2))) + line

    # Add the line inside quotes to the line
    output += "    \"" + line + "\",\n"

# finish off the like
output +="};"

# print it to the screen!
print (output);
