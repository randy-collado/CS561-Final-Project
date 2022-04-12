#include <vector>
#include <deque>
#include <random>
#include <cassert>
#include <iostream>

#pragma once

struct Node
{
    Node(size_t branchingFactor){
        maxChildren = branchingFactor;
        key = -1;
	children.reserve(branchingFactor);
    }
    size_t maxChildren;
    int key;
    std::vector<int> values;
    std::vector<Node*> children;
};

struct dummy_node { int x, int y, int z };

class Tree{
    public:
        Tree(size_t branchingFactor){
            branch = branchingFactor;
        }

        ~Tree(){

        }
        void fill(long numInserts, long upperBound, std::vector<int>& keys, std::vector<int>& values){
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(0, upperBound);
            for(long i = 0; i < numInserts; ++i){
                int key = distrib(gen);
                int value = distrib(gen);
                keys.push_back(key);
                values.push_back(value);
                add(key, value);
            }
        }
        void add(int key, int value){
            add_impl(key, value);
        }

        std::vector<int> BFS(int key){
            std::deque<Node*> queue = std::deque<Node*>();
            if (head == nullptr) return std::vector<int>();
            queue.push_back(head);
            for (;;){
                Node* currentNode = queue.front(); queue.pop_front();
                if (currentNode == nullptr) return std::vector<int>();
                if (currentNode->key == key){
                    std::vector<int> ret;
                    ret.insert(ret.begin(), (*currentNode).values.begin(), (*currentNode).values.end());
                    return ret;
                } else{
                    queue.insert(queue.end(), currentNode->children.begin(), currentNode->children.end());
                }
            }
        }

        std::vector<int> ParallelBFS(int key){
            std::deque<Node*> queue = std::deque<Node*>();
            if (head == nullptr) return std::vector<int>();
            queue.push_back(head);
            #pragma omp parallel
            {
                #pragma omp for num_threads(4)
                for (;;){
                    Node* currentNode = queue.front(); queue.pop_front();
                    if (currentNode == nullptr) return std::vector<int>();
                    if (currentNode->key == key){
                        // std::vector<int> ret;
                        // ret.insert(ret.begin(), (*currentNode).values.begin(), (*currentNode).values.end());
                        return currentNode->values;
                    } else{
                        queue.insert(queue.end(), currentNode->children.begin(), currentNode->children.end());
                    }
                }
            }
        }

    private:
        void add_impl(int key, int value){
            std::deque<Node*> queue = std::deque<Node*>();
            if (head == nullptr){
                head = new Node(branch);
                head->key = key;
                head->values.push_back(value);
                head->children = std::vector<Node*>();
                return;
            }
            queue.push_back(head);

            while (!queue.empty()){
                Node* currentNode = queue.front(); queue.pop_front();
                if (currentNode == nullptr){
                    assert(false && "unreachable");
                } else if (currentNode->key == key){
                    currentNode->values.push_back(value);
                    return;
                } else if (currentNode->children.size() < branch){
                    Node* node = new Node(branch);
                    node->key = key;
                    node->values.push_back(value);
                    currentNode->children.push_back(node);
                    return;
                } else {
                    queue.insert(queue.end(), currentNode->children.begin(), currentNode->children.end());
                }
            }
        }
        Node* head;
        size_t numElements;
        size_t branch;
};

