//
// Created by Randy Collado on 3/17/22.
//

#ifndef CS561_FINAL_PROJECT_BINARYTREE_H
#define CS561_FINAL_PROJECT_BINARYTREE_H


#include <memory>

template <typename IntegralKeyType, typename ValueType>
struct Node {
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    IntegralKeyType key;
    ValueType value;
};

template<typename IntegralKeyType, typename ValueType>
class BinaryTree {


    BinaryTree();
    ~BinaryTree();

    //dont know whether to make it return true for good insert
    void addOne(IntegralKeyType key, ValueType value);

    std::vector<IntegralKeyType> addFromVector(std::vector<std::pair<IntegralKeyType, ValueType>> KVpairs);

    std::shared_ptr<Node<IntegralKeyType, ValueType>> find(IntegralKeyType key);

    std::vector<std::shared_ptr<Node<IntegralKeyType, ValueType>>> findRange(std::vector<IntegralKeyType> keys);



private:

    size_t size;
    size_t max_key;
    size_t min_key;
    Node<IntegralKeyType, ValueType> head;
};


#endif //CS561_FINAL_PROJECT_BINARYTREE_H
