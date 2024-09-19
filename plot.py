from matplotlib import pyplot as plt 

fig, ax = plt.subplots() # Create a figure contianing a single axes.
X = [1,2,3,4]
Y = [1,4,2,3]
ax.plot(X, Y)       # plot some data on the axes.
plt.show()          # Show the figure contianing a single axes



TraceFiles= [
        "Application_Traces/bzip.trace/bzip.trace",
        "Application_Traces/gcc.trace/gcc.trace",
        "Application_Traces/sixpack.trace/sixpack.trace",
        "Application_Traces/swim.trace/swim.trace"
] 


Data = {
    TraceFiles[0]: {},
    TraceFiles[1]: {},
    TraceFiles[2]: {},
    TraceFiles[3]: {}
}




