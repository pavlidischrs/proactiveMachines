import numpy as np
import matplotlib.pyplot as plt
import sys
caffe_root = '/home/pavlidis/libraries/caffe/'  
sys.path.insert(0, caffe_root + 'python')
import caffe

plt.rcParams['figure.figsize'] = (10, 10)       
plt.rcParams['image.interpolation'] = 'nearest'  
plt.rcParams['image.cmap'] = 'gray'  


def vis_square(data):
    """Take an array of shape (n, height, width) or (n, height, width, 3)
       and visualize each (height, width) thing in a grid of size approx. sqrt(n) by sqrt(n)"""
    print data.max(), data.min()
    # normalize data for display
    data = (data - data.min()) / (data.max() - data.min())
    
    # force the number of filters to be square
    n = int(np.ceil(np.sqrt(data.shape[0])))
    padding = (((0, n ** 2 - data.shape[0]),
               (0, 1), (0, 1))                 # add some space between filters
               + ((0, 0),) * (data.ndim - 3))  # don't pad the last dimension (if there is one)
    data = np.pad(data, padding, mode='constant', constant_values=1)  # pad with ones (white)
    
    # tile the filters into an image
    data = data.reshape((n, n) + data.shape[1:]).transpose((0, 2, 1, 3) + tuple(range(4, data.ndim + 1)))
    data = data.reshape((n * data.shape[1], n * data.shape[3]) + data.shape[4:])
    
    plt.imshow(data)
    plt.show()
    plt.axis('off')



#Solver and Net
caffe.set_mode_cpu()
model_def='lenet_train_test.prototxt'
model_weights='partial__iter_2000_accuracy_095.caffemodel'
net = caffe.Net(model_def, model_weights, caffe.TEST) 

#Blobs Load 
transformer = caffe.io.Transformer({'data': net.blobs['data'].data.shape})
transformer.set_transpose('data', (2,0,1))  



#Signle Pass
#img = caffe.io.load_image('../../../Desktop/s4_grey_.png',color=False)
img = caffe.io.load_image('images/'+str(sys.argv[1])+'.png',color=False)
net.blobs['data'].data[...] = transformer.preprocess('data', img )
output = net.forward() 

output_prob = output['ip2'][0]
print output_prob
print 'Predicted digit is',output_prob.argmax()

feat = net.blobs['theta'].data[0]
print feat

#Print to file
"""
f = open(caffe_root + '../out.txt','w')
for i in range (0,10):
	#Input
	img = caffe.io.load_image('../../../Desktop/databases/dis_trs_part/'+str(i)+'.png',color=False) #str(sys.argv[i+1])+'.png',color=False)
	net.blobs['data'].data[...] = transformer.preprocess('data', img )


	output = net.forward() 

	output_prob = output['prob'][0]
	#print 'Predicted digit is',output_prob.argmax()
	digit = output_prob.argmax()
	f.write('Predicted digit is ' + str(digit))
	f.write('\n')

	feat = net.blobs['theta'].data[0]
	f.write('And the T is:\t[ '+str(feat[0])+' '+str(feat[1])+' '+str(feat[2])+' \n\t\t '+str(feat[3])+' '+str(feat[4])+' '+str(feat[5])+']\n')
	f.write('\n')
"""

#Blob's Dimensions
'''
for layer_name, blob in net.blobs.iteritems():
    print layer_name + '\t' + str(blob.data.shape)

print '....'

for layer_name, param in net.params.iteritems():
    print layer_name + '\t' + str(param[0].data.shape), str(param[1].data.shape)
'''


#filters1 = net.params['loc_conv1'][0].data
#vis_square(filters1.transpose(0, 2, 3, 1))
#raw_input()

#filters2 = net.params['conv1'][0].data
#vis_square(filters2[:32].reshape(32**1, 9, 9))

feat = net.blobs['data'].data[0, :20]
vis_square(feat)
feat = net.blobs['tra_data'].data[0, :20]
vis_square(feat)

#raw_input()
"""
feat = net.blobs['theta'].data[0]
plt.subplot(2, 1, 1)
plt.plot(feat.flat)
plt.subplot(2, 1, 2)
_ = plt.hist(feat.flat[feat.flat > 0], bins=100)
plt.show()
"""



