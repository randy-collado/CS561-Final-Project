#include "Tree.cpp"
#include <chrono>
#include <iostream>
#include <vector>
#include <numeric>
#include <thread>
#include "omp.h"

int main(int argc, char** argv){
    if (argc < 3){
        std::cerr << "Usage: ./main <branching-factor> <num-elements>\n";
        exit(1);
    }
    long branch = std::stoi(argv[1]);
    long num_elements = std::stol(argv[2]);
    Tree tree(branch);
    std::vector<int> keys;
    std::vector<int> values;
    tree.fill(num_elements, 2000l, keys, values);

    std::vector< std::vector<int> > outputs;

    std::cout << "Key: " << keys[3] << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    outputs.push_back(tree.BFS(keys[3]));
    outputs.push_back(tree.BFS(keys[12]));
    outputs.push_back(tree.BFS(keys[36]));
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> runtime = (end-start) / 3.0;

    std::cout << "Runtime for single-thread implementation: " << runtime.count() << "ms\n";
    {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(50ms);
    }
    auto start2 = std::chrono::high_resolution_clock::now();
    auto a = tree.ParallelBFS(keys[3]);
    auto b = tree.ParallelBFS(keys[12]);
    auto c = tree.ParallelBFS(keys[36]);
    auto end2 = std::chrono::high_resolution_clock::now();

    outputs.push_back(a);
    outputs.push_back(b);
    outputs.push_back(c);
    std::chrono::duration<double, std::milli> runtime2 = (end2 - start2)/3.0;
    std::cout << "Runtime for OpenMP implementation: " << runtime2.count() << "ms\n";
    std::cout << ((outputs[0] == outputs[3]) && (outputs[1] == outputs[4]) && (outputs[2] == outputs[5])) << std::endl;
    
}
