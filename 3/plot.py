import matplotlib.pyplot as plt

x = [2, 4, 6, 8, 10, 16]
y = [1.772012, 1.798389, 1.833454, 1.811745, 1.832687, 1.734506, ]

plt.plot(x, y)
plt.title("Matrix Size = 1024")
plt.xlabel("Number of Threads")
plt.ylabel("Speed-up")
plt.show()