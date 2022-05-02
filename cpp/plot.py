import matplotlib.pyplot as plt

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
plt.plot(threads, pbfs_20000_8_cpu2,label='Parallel BFS with 2-CPU')
plt.plot(threads, pbfs_20000_8_cpu4,label='Parallel BFS with 4-CPU')
plt.plot(threads, pbfs_20000_8_cpu6,label='Parallel BFS with 6-CPU')
plt.plot(threads, pbfs_20000_8_cpu8,label='Parallel BFS with 8-CPU')
plt.legend(loc = 'upper right')
plt.xlabel("Number of Threads")
plt.ylabel("Searching Time (ms)")
plt.show()

# test3: fsize in iotest (search the whole tree)
# fsize_iotest = [4,8,10,12,14,16,20,32]
# fsize_4 = [7896,4272,2310,1527,806,587,549,536,533,628]
# fsize_8 = [7683,3923,2421,1350,820,571,534,544,551,552]
# fsize_10 = [8290,4488,2376,1507,823,665,632,639,761,641]
# fsize_12 = [7547,3881,2273,1229,754,880,775,802,792,908]
# fsize_14= [8548,4348,2338,1379,1215,1419,1205,1224,1217,1216]
# fsize_16 = [8290,4247,2208,1962,1808,1825,1960,1822,1863,1975]
# fsize_20 = [7638,4091,3752,3799,3654,3717,3490,3760,3722,3548]
# fsize_32 = [8376,8536,8742,8434,8190,8772,7707,9015,8884,8157]
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

# test4: fsize in random target key test

# test5: algorithm efficiency with different tree size

# test6: algorithm efficiency with different branch size
