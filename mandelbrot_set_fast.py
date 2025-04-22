import matplotlib.pyplot as plt
import numpy as np

#Mandelbrot equation: Zn = (Zn-1)^2 + C

#C is any number on the complex plane
#can color code how fast each number reaches a certain threshhold to get beautiful patterns

#there will be resolution^2 pixels calculated, play around with this number
#complexity is O(n^2) where n = resolution
resolution = 2000

#make a 2D grid of pixels
xrange = np.linspace(-2, 1, resolution)
yrange = np.linspace(-1.5, 1.5, resolution)
# xrange = np.linspace(-0.9, -0.7, resolution)
# yrange = np.linspace(0.0, 0.2, resolution)

x, y = np.meshgrid(xrange, yrange)

#store the iterations of the while loop
num_iterations = 0  
max_iterations = 100

#x and y are 200 by 400 arrays
nrows = int(x.size / x[1].size)
ncols = x[1].size

# print(nrows)
# print(ncols)

#store the number of iterations of each pixel
pixel_iterations = np.zeros((resolution, resolution))

#use a filter to stop updating values once they reach a threshold
mask = np.full((nrows, ncols), True, dtype=bool)
z = np.full((nrows, ncols), 0, dtype=float)

#apply eqn to the whole array
while num_iterations < max_iterations:
    
    #the mask only allows the computation to continue on values that haven't reached the threshold
    mask = np.where(abs(z) > 2, False, True)
    
    #depending on the mask, update pixel_iterations
    z = z**2 + ((x + y*1j) * mask)
    pixel_iterations += 1 * mask
    
    num_iterations += 1
    
plt.imshow(pixel_iterations, cmap='inferno', extent=[-2, 1, -1.5, 1.5])
plt.title("Mandelbrot Set")
plt.xlabel("Real Axis")
plt.ylabel("Imaginary Axis")
plt.show()
'''
plt.imshow(pixel_iterations, cmap='inferno', extent=[-2, 1, -1.5, 1.5])
plt.colorbar()  # optional: shows the color scale
plt.title("Mandelbrot Set")
plt.xlabel("Real Axis")
plt.ylabel("Imaginary Axis")
plt.show()
'''