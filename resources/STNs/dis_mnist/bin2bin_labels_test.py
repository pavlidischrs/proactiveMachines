import struct


f = open('db/t10k-labels.idx1-ubyte','r')
value = f.read()

arrayint= [0, 0, 8, 1, 0, 0, 0, 100];

df = open ('distorted-t10k-labels-idx1-ubyte','w')

df.write ( bytearray(arrayint) )
#print len(value)
for i in range (8, 108):
	df.write(value[i])

