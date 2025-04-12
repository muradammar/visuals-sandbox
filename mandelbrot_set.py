import matplotlib.pyplot as plt
import numpy as np

#Mandelbrot equation: Zn = (Zn-1)^2 + C

#C is any number on the complex plane
#can color code how fast each number reaches a certain threshhold to get beautiful patterns

#make a 2D grid of pixels
xrange = np.linspace(-2, 1, 800)
yrange = np.linspace(-1.5, 1.5, 800)

x, y = np.meshgrid(xrange, yrange)

max_iterations = 1000

#x and y are 200 by 400 arrays
nrows = int(x.size / x[1].size)
ncols = x[1].size

print(nrows)
print(ncols)

#store the number of iterations of each pixel
pixel_iterations = np.zeros((800, 800))

#iterate through each pixel
for i in range(nrows):
    for j in range(ncols):
        c = x[i, j] + y[i, j]*1j
        z = 0
        num_iterations = 0
        
        #apply mandelbrot eqn
        while abs(z) < 2 and num_iterations < max_iterations:
            z = z**2 + c
            
            num_iterations += 1
        
        #store the number of iterations
        pixel_iterations[i, j] = num_iterations
        
    
#normalize pixels
pixel_iterations = pixel_iterations / 1000


plt.imshow(pixel_iterations, cmap='inferno', extent=[-2, 1, -1.5, 1.5])
plt.colorbar()  # optional: shows the color scale
plt.title("Mandelbrot Set")
plt.xlabel("Real Axis")
plt.ylabel("Imaginary Axis")
plt.show()