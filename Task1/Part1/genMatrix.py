import numpy as np
arr = np.random.rand(201,201)
np.savetxt('A.txt',arr,delimiter=' ',fmt='%f')
arr = np.random.rand(201,201)
np.savetxt('B.txt',arr,delimiter=' ',fmt='%f')

