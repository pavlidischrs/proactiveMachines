import random


f = open('db/train-images-idx3-ubyte','r')

value = f.read()

offset = 16;

output = open('dis-train-images-idx3-ubyte','w')

size = 16+(60000*60*60)
arrayint = [0] * size
arrayint[0] = value[0];
arrayint[1] = value[1];
arrayint[2] = value[2];
arrayint[3] = value[3];
arrayint[4] = value[4];
arrayint[5] = value[5];
arrayint[6] = value[6];	#00000010 -> 2
arrayint[7] = value[7];	#01011000 -> 88
arrayint[8] = 0;
arrayint[9] = 0;
arrayint[10] = 0;
arrayint[11] = 60;
arrayint[12] = 0;
arrayint[13] = 0;
arrayint[14] = 0;
arrayint[15] = 60;

for i in range(0,60000):

	x = random.randrange(29)
	y = random.randrange(29)
	for j in range(0,28):
		for l in range(0,28):
			arrayint[offset + (i*3600) + (x+j)*60 + y+l ] = value[offset+ (i*784) + j*28+l]



binar = bytearray(arrayint)
output.write(binar)
			


