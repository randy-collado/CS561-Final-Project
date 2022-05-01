#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <chrono>
#include <future>
#include <thread>
#include "TreeSerializer.hpp"
#include <omp.h>
#include <sched.h>

void testRead(char* filepath){
    int fd = open(filepath, O_RDWR | O_CREAT | O_DIRECT);
    if (fd < 0) {
        std::cerr << "[ERROR]: DirectSearch fd Initialization failed\n[MESSAGE]: file" << filepath << " unable to be opened or created";
        exit(1);
    }
    int ret;
    char* buf;
    ret = posix_memalign((void **)&buf, 512, 512);
    if (ret) {
        perror("posix_memalign failed");
        exit(1);
    }
    ret = pread(fd, buf, 512, 0);
    // std::cout << "Bytes read: " << ret << "node id: " << ((S_Node*)buf)->key << std::endl;
    if(ret<0) std::cout << "Could Not Read Offset "<< 0 <<" & Reason: " + std::string(strerror(errno))<<std::endl;

    // S_Node* buf_;
    // ret = pread(fd, (char*)buf_, 512, 0);
    // std::cout << "Bytes read: " << ret << "node id: " << buf_->key << std::endl;
    // if(ret<0) std::cout << "Could Not Read Offset "<< 0 <<" & Reason: " + std::string(strerror(errno))<<std::endl;
}

void testSerial(int fd){
    for (int i=0; i<128; i++){
        int ret;
        char* buf;
        ret = posix_memalign((void **)&buf, 512, 512);
        if (ret) {
            perror("posix_memalign failed");
            exit(1);
        }
        ret = pread(fd, buf, 512, 512*i);
        // std::cout << "Bytes read: " << ret << "node id: " << ((S_Node*)buf)->key << std::endl;
        if(ret<0) std::cout << "Could Not Read Offset "<< 512*i <<" & Reason: " + std::string(strerror(errno))<<std::endl;
    }
}

void parallelRead(int fd, int offset){
    int ret;
    char* buf;
    ret = posix_memalign((void **)&buf, 512, 512);
    if (ret) {
        perror("posix_memalign failed");
        exit(1);
    }
    ret = pread(fd, buf, 512, 512*offset);
    // std::cout << "Read node id: " << ((S_Node*)buf)->key << std::endl;
    if(ret<0) std::cout << "Could Not Read Offset "<< 512*offset <<" & Reason: " + std::string(strerror(errno))<<std::endl;
}

void testParallel(int fd){
    #pragma omp parallel for
    for (int i = 0; i < 128; i++)
    {
        parallelRead(fd, i);
    }

    // std::thread t1(parallelRead, fd, 0);
    // std::thread t2(parallelRead, fd, 1);
    // std::thread t3(parallelRead, fd, 2);
    // std::thread t4(parallelRead, fd, 3);
    // std::thread t5(parallelRead, fd, 4);
    // std::thread t6(parallelRead, fd, 5);
    // std::thread t7(parallelRead, fd, 6);
    // std::thread t8(parallelRead, fd, 7);
    // t1.join();
    // t2.join();
    // t3.join();
    // t4.join();
    // t5.join();
    // t6.join();
    // t7.join();
    // t8.join();
}



int main(int argc, char** argv){
    if (argc < 3) {
        std::cerr << "[ERROR]: Usage is ./diotest <filepath> <key>" << std::endl;
        exit(1);
    }
    
    // Test 1: use direct io to read a S_Node
    // testRead(argv[1]);

    // Test 2: multithread to read direct io
    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);
    CPU_SET(0, &cpu_set);
    CPU_SET(1, &cpu_set);
    CPU_SET(2, &cpu_set);
    CPU_SET(3, &cpu_set);
    if(sched_setaffinity(0, sizeof(cpu_set), &cpu_set) < 0)
        perror("sched_setaffinity");

    std::cout << "Start Testing" << std::endl;
    int fd = open(argv[1], O_RDONLY | O_DIRECT);
    if (fd < 0) {
        std::cerr << "[ERROR]: DirectSearch fd Initialization failed\n[MESSAGE]: file" << argv[1] << " unable to be opened or created";
        exit(1);
    }
    auto start = std::chrono::high_resolution_clock::now();
    testSerial(fd);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Serial Timing: " << duration.count() << std::endl;

    for (int i = 1; i <= 64; i *= 2) {
        start = std::chrono::high_resolution_clock::now();
        std::cout << "----- Thread " << i << " -----"  << std::endl; 
        omp_set_num_threads(i);
        testParallel(fd);
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << "Parallel Timing: " << duration.count() << std::endl; 
    }

    int ab = 100000000;
    while (ab > 0){
        ab --;
    }
}
