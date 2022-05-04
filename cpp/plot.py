from cProfile import label
import matplotlib.pyplot as plt
import numpy as np

# test0: num of threads
threads = [1,2,4,8,16,32,64,80,100,120]
# serial_8000 = [3082915 for i in range(len(threads))]
parallel_8000 = [3128549, 1764186, 896362, 499187, 299863, 224372, 223671, 216548, 212367, 211370]
parallel_4000 = [1489575, 749055, 431638, 253360, 159290, 111392, 111302, 102929, 103758, 105520]
parallel_1000 = [360250, 213824, 120136, 61022, 36678, 29015, 29893, 29779, 28684, 27830]
# plt.plot(threads, parallel_8000,label='8K Random Read')
# plt.plot(threads, parallel_4000,label='4K Random Read')
# plt.plot(threads, parallel_1000,label='1K Random Read')
# plt.legend(loc = 'upper right')
# plt.xlabel("Number of Threads")
# plt.ylabel("Reading Time (ms)")
# plt.show()

# test1: num of threads
threads = [1,2,4,8,16,32,64,80,100,120]
pbfs_20000_8_cpu8 = [7177, 4068, 2246, 1258, 798, 620, 576, 577, 558, 562]
pdfs_20000_8_8 = [7683, 3923, 2421, 1350, 820, 571, 534, 544, 551, 552]
# plt.plot(threads, pbfs_20000_8_cpu8,label='Parallel BFS with 8-CPU')
# plt.plot(threads, pdfs_20000_8_8,label='Parallel DFS with 8-CPU 8-fSize')
# plt.legend(loc = 'upper right')
# plt.xlabel("Number of Threads")
# plt.ylabel("Searching Time (ms)")
# plt.show()

# test2: num of cpu
num_cpu = [2,4,6,8]
pbfs_20000_8_cpu2 = [7515,3873,2205,1278,782,567,548,542,530,605]
pbfs_20000_8_cpu4 = [7221,3791,2218,1318,759,617,571,569,558,563]
pbfs_20000_8_cpu6 = [6857,4089,2236,1247,780,602,559,566,559,564]
# plt.plot(threads, pbfs_20000_8_cpu2,label='Parallel BFS with 2-CPU')
# plt.plot(threads, pbfs_20000_8_cpu4,label='Parallel BFS with 4-CPU')
# plt.plot(threads, pbfs_20000_8_cpu6,label='Parallel BFS with 6-CPU')
# plt.plot(threads, pbfs_20000_8_cpu8,label='Parallel BFS with 8-CPU')
# plt.legend(loc = 'upper right')
# plt.xlabel("Number of Threads")
# plt.ylabel("Searching Time (ms)")
# plt.show()

# test3: fsize in iotest (search the whole tree)
fsize_iotest = [4,8,10,12,14,16,20,32]
fsize_4 = [7896,4272,2310,1527,806,587,549,536,533,628]
fsize_8 = [7683,3923,2421,1350,820,571,534,544,551,552]
fsize_10 = [8290,4488,2376,1507,823,665,632,639,761,641]
fsize_12 = [7547,3881,2273,1229,754,880,775,802,792,908]
fsize_14= [8548,4348,2338,1379,1215,1419,1205,1224,1217,1216]
fsize_16 = [8290,4247,2208,1962,1808,1825,1960,1822,1863,1975]
fsize_20 = [7638,4091,3752,3799,3654,3717,3490,3760,3722,3548]
fsize_32 = [8376,8536,8742,8434,8190,8772,7707,9015,8884,8157]
# plt.plot(threads, fsize_4,label='Parallel DFS with 4-fSize')
# plt.plot(threads, fsize_8,label='Parallel DFS with 8-fSize')
# plt.plot(threads, fsize_10,label='Parallel DFS with 10-fSize')
# plt.plot(threads, fsize_12,label='Parallel DFS with 12-fSize')
# plt.plot(threads, fsize_14,label='Parallel DFS with 14-fSize')
# plt.plot(threads, fsize_16,label='Parallel DFS with 16-fSize')
# plt.plot(threads, fsize_20,label='Parallel DFS with 20-fSize')
# plt.plot(threads, fsize_32,label='Parallel DFS with 32-fSize')
# plt.legend(loc = 'upper right')
# plt.xlabel("Number of Threads")
# plt.ylabel("Searching Time (ms)")
# plt.show()


# test4: fsize in random key
fsize_randtest = [2,4,6,8,10,12,14,16,20]
fsize_20000_4 = [12996,12976,12521,9965,11747,18155,33234,48871,74979]
fsize_50000_2 = [31148,29414,23336,38824,92481,171826,208007,213923,203460]
fsize_50000_4 = [33199,32093,30948,22038,21746,39328,73182,109278,184187]
fsize_50000_8 = [31533,31550,29376,29114,28755,25089,22705,28576,54575]
fsize_80000_4 = [51187,51592,51179,41154,35906,45761,83850,122422,255360]

plt.plot(fsize_randtest, fsize_20000_4,label="20000-Node & 4-Branch")
plt.plot(fsize_randtest, fsize_50000_2, label="50000-Node & 2-Branch")
plt.plot(fsize_randtest, fsize_50000_8,label="50000-Node & 8-Branch")
plt.plot
(fsize_randtest, fsize_80000_4,label="80000-Node & 4-Branch")
plt.xlabel("fSize")
plt.ylabel("Total Searching Time (ms)")
plt.legend()
plt.show()

# x = np.arange(len(fsize_randtest))
# total_width, n = 0.8, 4
# width = total_width / n
# plt.bar(x, fsize_20000_4,tick_label=fsize_randtest,label="20000-Node & 4-Branch", width=width)
# plt.bar(x+width, fsize_50000_2,tick_label=fsize_randtest, label="50000-Node & 2-Branch",width=width)
# plt.bar(x+2*width, fsize_50000_8,tick_label=fsize_randtest,label="50000-Node & 8-Branch", width=width)
# plt.bar(x+3*width, fsize_80000_4,tick_label=fsize_randtest,label="80000-Node & 4-Branch", width=width)
# plt.xlabel("fSize")
# plt.ylabel("Total Searching Time (ms)")
# plt.legend()
# plt.show()

# test5: algorithm efficiency with different tree size
labels = ["S-BFS","S-DFS","P-BFS","P-DFS"]
test_20000_4 = [129402, 75246, 9673, 9965]
test_50000_2 = [256822, 210897, 18943, 23336]
test_50000_4 = [278710, 278710, 20622, 21746]
test_50000_8 = [264069, 186948, 19818, 22705]
test_80000_4 = [537486, 344307, 37590, 35906]
# plot 1
# x = np.arange(len(labels))
# total_width, n = 0.8, 3
# width = total_width / n
# plt.bar(x, test_20000_4, tick_label=labels, label="20000-Node & 4-Branch", width=width)
# plt.bar(x+width, test_50000_4, tick_label=labels, label="50000-Node & 4-Branch", width=width)
# plt.bar(x+2*width, test_80000_4, tick_label=labels, label="80000-Node & 4-Branch", width=width)
# plt.ylabel("Total Searching Time (ms)")
# plt.legend()
# plt.show()

# plot 2
# x = np.arange(len(labels))
# total_width, n = 0.8, 3
# width = total_width / n
# plt.bar(x, test_50000_2, tick_label=labels, label="50000-Node & 2-Branch", width=width)
# plt.bar(x+width, test_50000_4, tick_label=labels, label="50000-Node & 4-Branch", width=width)
# plt.bar(x+2*width, test_50000_8, tick_label=labels, label="50000-Node & 8-Branch", width=width)
# plt.ylabel("Total Searching Time (ms)")
# plt.legend()
# plt.show()


# test6: algorithm efficiency with different branch size
