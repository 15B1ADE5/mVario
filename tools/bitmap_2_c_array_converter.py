#!/usr/bin/env python

import sys, argparse, os, math, matplotlib.image

arg_parser = argparse.ArgumentParser()

arg_parser.add_argument('--input', '-i')
arg_parser.add_argument('--output', '-o')
arg_parser.add_argument('--threshold', '-t')
arg_parser.add_argument('--group', '-g')
arg_parser.add_argument('--skip', '-s')

args = arg_parser.parse_args()

image_path = ""
if args.input:
	image_path = args.input
else:
	sys.exit()

output = os.path.splitext( os.path.basename(image_path) )[0] + ".h"
if args.output:
	output = args.output

threshold = 0.5
if args.threshold:
	threshold = args.threshold

group = 4
if args.group:
	group = args.group

skip_v_line = None
if args.skip:
	skip_v_line = int(args.skip)

image = matplotlib.image.imread(image_path)

y_len = math.ceil(len(image)/8)
x_len = len(image[0])

out_list = []

for y in range(y_len):
	lines = 8
	if ( (y + 1) * 8) > len(image):
		lines = len(image) - (y * 8)

	skip_x_count = 0
	for x in range(x_len):
		skip = False
		if(skip_v_line is not None):
			if( (x % (skip_v_line + 1) ) == skip_v_line):
				skip = True
				skip_x_count += 1

		if(not skip):
			vbit = 0

			for line in range(lines):
				if(image[(y * 8) + line][x][0] > threshold):
					vbit |= (1 << line)
			
			out_list.append(vbit)



image_name = os.path.splitext( os.path.basename(output) )[0].upper()
out_file = open(output, "w")

# header
out_file.write("#ifndef " + image_name + "_H\n")
out_file.write("#define " + image_name + "_H\n")
out_file.write("\n")
out_file.write("#include <stdint.h>\n")
out_file.write("\n")
out_file.write("#define " + image_name + "_Y_LEN      " + str(y_len) + "\n")
out_file.write("#define " + image_name + "_X_LEN      " + str(x_len - skip_x_count) + "\n")
out_file.write("#define " + image_name + "_ARRAY_LEN  " + str(len(out_list)) + "\n")
out_file.write("\n")
out_file.write("const uint8_t " + image_name.lower() + "[" + str(len(out_list)) + "] = {\n")

array_lines = math.ceil(len(out_list) / group)

for line in range(array_lines):
	array_bytes = group
	if ( (line + 1) * group) > len(out_list):
		array_bytes = len(out_list) - (line * group)

	out_line = "	"
	for array_byte in range(array_bytes):
		out_line += str('0x{0:0{1}X}'.format(out_list[(line * group) + array_byte], 2)) + ", "
	
	out_file.write(out_line + "\n")


out_file.write("};\n")
out_file.write("\n")

out_file.write("#endif // " + image_name + "_H\n")
out_file.close()