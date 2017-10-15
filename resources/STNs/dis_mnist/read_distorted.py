import numpy as np
  

import Image
import struct

f = open('../../../../Desktop/databases/distorted_db_part/dis-train-images-idx3-ubyte','r')

value = f.read()
offset=16
count=0;

#a = np.ones((60,60))

for k in range(0,1):

	img = Image.new( 'L', (60,60),'black')
	pixels = img.load() 
	
	for i in range(img.size[1]):    
	    for j in range(img.size[1]):
		#print i , j
		pixels[j,i] = struct.unpack('B',value[offset + count ])[0]
		count = count + 1

#	img.save(str(k)+'.jpg')





	img.save('dis_ts_part/n'+str(k)+'.jpg')




