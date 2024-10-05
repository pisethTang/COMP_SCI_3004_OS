import matplotlib.pyplot as plt 


def rectangle (x,y,size):
    return [x, x+size, x+size, x,x], [y,y,y+size,y+size,y]


def sierpinski_carpet(x,y,size):
    if size < 1: return 
    for i in range(3):
        sx = x + i * size/3
        for j in range(3):
            sy = y + j*size/3
            if (i == 1 and j == 1):
                xs,ys = rectangle(sx,sy,size/3)
                plt.fill(xs,ys,color='black')
            else:
                sierpinski_carpet(sx,sy,size/3)

sierpinski_carpet(0,0,200)
plt.axis('scaled')
plt.show()

















