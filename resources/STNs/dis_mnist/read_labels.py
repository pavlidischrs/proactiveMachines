import struct

f = open('distorted-t10k-labels.idx1-ubyte','r')
value = f.read()
for i in range (100,108):
	print struct.unpack('B',value[i])[0]


