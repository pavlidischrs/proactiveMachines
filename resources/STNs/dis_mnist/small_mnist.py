f1 = open('db/train-images.idx3-ubyte','r')
f2 = open('db/train-labels.idx1-ubyte','r')
f3 = open('db/t10k-images.idx3-ubyte','r')
f4 = open('db/t10k-labels.idx1-ubyte','r')

value1 = f1.read()
value2 = f2.read()
value3 = f3.read()
value4 = f4.read()

out1 = open('small-train-i-idx3-ubyte','w')
out2 = open('small-train-l-idx1-ubyte','w')
out3 = open('small-test-i-idx3-ubyte','w')
out4 = open('small-test-l-idx1-ubyte','w')

buf1 = [0] * (16+(600*28*28))
buf2 = [0] * (8+(600))
buf3 = [0] * (16+(100*28*28))
buf4 = [0] * (8+(100))

for i in range(0,16+(600*28*28)):
	buf1[i] = value1[i] 
		
for i in range(0,8+(600)):
	buf2[i] = value2[i]

for i in range(0,16+(100*28*28)):
	buf3[i] = value3[i] 
		
for i in range(0,8+(100)):
	buf4[i] = value4[i]


buf1[6] = 2
buf1[7] = 88

buf2[6] = 2
buf2[7] = 88

buf3[6] = 0
buf3[7] = 100

buf4[6] = 0
buf4[7] = 100




b1 = bytearray(buf1)
b2 = bytearray(buf2)
b3 = bytearray(buf3)
b4 = bytearray(buf4)

out1.write(b1)
out2.write(b2)
out3.write(b3)
out4.write(b4)


