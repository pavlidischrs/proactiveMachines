

import sys
caffe_root = '../../../Desktop/caffe-stn-master/'
sys.path.insert(0, caffe_root + 'python')
import caffe


caffe.set_mode_cpu()

solver = caffe.SGDSolver('st_mnist_part/lenet_auto_solver.prototxt')

solver.solve()





#solver.net.forward()
#solver.test_nets[0].forward() 




#from pylab import *

# display data
'''
print [(k, v.data.shape) for k, v in solver.net.blobs.items()] 
print '......'
print [(k, v[0].data.shape) for k, v in solver.net.params.items()]
print '......'
'''
#visualise input 
'''
def vis_square(data):
    #ake an array of shape (n, height, width) or (n, height, width, 3)
    #   and visualize each (height, width) thing in a grid of size approx. sqrt(n) by sqrt(n)"""
    
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
    plt.save('~/Desktop/1.jpg')
'''


#check.py
'''
count=0;
for i in range(0,12):
 solver.step(1)
 if (math.fmod(i, 600))==0:
  count= count + 1
  plt.subplot(1, 1, 1)
  imshow(solver.net.blobs['tra_data'].data[:8, 0].transpose(1, 0, 2).reshape(60, 8*60), cmap='gray'); axis('off')
  plt.savefig(caffe_root + '../../Desktop/'+str(count)+'.png', dpi=100)

  feat = solver.net.blobs['theta'].data[0]
  plt.subplot(2, 1, 1)
  plt.plot(feat.flat)
  plt.subplot(2, 1, 2)
  _ = plt.hist(feat.flat[feat.flat > 0], bins=100)
  plt.savefig(caffe_root + '../../Desktop/thetas_'+str(count)+'.png', dpi=100)  
  #plt.show()
  #plt.show()
'''  



#more check
'''
feat = solver.net.blobs['theta'].data[0]
plt.subplot(2, 1, 1)
plt.plot(feat.flat)
plt.subplot(2, 1, 2)
_ = plt.hist(feat.flat[feat.flat > 0], bins=100)
plt.show()

feat = solver.net.blobs['data'].data[:8, 0]
vis_square(feat)
'''

#imshow(solver.net.blobs['tra_data'].data[:8, 0].transpose(1, 0, 2).reshape(60, 8*60), cmap='gray'); axis('off')
#plt.show()
#print 'train labels:', solver.net.blobs['label'].data[:8]
 
