import struct


f = open('db/train-labels.idx1-ubyte','r')
value = f.read()

arrayint= [0, 0, 8, 1, 0, 0, 2, 88];

df = open ('distorted-train-labels-idx1-ubyte','w')

df.write ( bytearray(arrayint) )
#print len(value)
for i in range (8, 608):
	df.write(value[i])

