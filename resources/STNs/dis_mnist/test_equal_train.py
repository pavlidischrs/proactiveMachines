import random


f1 = open('distorted-train-images-idx3-ubyte','r')
value1 = f1.read()

offset1 = 16;
count = 0;

output1 = open('distorted-tt10k-images-idx3-ubyte','w')

size1 = 16+(100*60*60)
arrayint1 = [0] * size1
arrayint1[0] = 0;
arrayint1[1] = 0;
arrayint1[2] = 8;
arrayint1[3] = 3;
arrayint1[4] = 0;
arrayint1[5] = 0;
arrayint1[6] = 0;	#00000010 -> 2
arrayint1[7] = 100;	#01011000 -> 88
arrayint1[8] = 0;
arrayint1[9] = 0;
arrayint1[10] = 0;
arrayint1[11] = 60;
arrayint1[12] = 0;
arrayint1[13] = 0;
arrayint1[14] = 0;
arrayint1[15] = 60;


count = 0
for i in range(0,360000):
	arrayint1[ offset1 + i ] = value1[ offset1 + i ]


binar1 = bytearray(arrayint1)
output1.write(binar1)



f2 = open('distorted-train-labels-idx1-ubyte','r')
value2 = f2.read()
output2 = open('distorted-tt10k-labels-idx1-ubyte','w')
offset2 = 8	
		
size2 = 8+(100)
arrayint2 = [0] * size2
arrayint2[0] = 0;
arrayint2[1] = 0;
arrayint2[2] = 8;
arrayint2[3] = 1;
arrayint2[4] = 0;
arrayint2[5] = 0;
arrayint2[6] = 0;	#00000010 -> 2
arrayint2[7] = 100;	#01011000 -> 88
arrayint2[8] = 0;

count = 0
for i in range(0,100):
	arrayint2[offset2+i]=value2[offset2+i]

binar2 = bytearray(arrayint2)
output2.write(binar2)
