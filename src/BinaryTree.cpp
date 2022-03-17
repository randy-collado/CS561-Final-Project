//
// Created by Randy Collado on 3/17/22.
//

#include "BinaryTree.h"
#include <vector>

template<typename IntegralKeyType, typename ValueType>
BinaryTree<IntegralKeyType, ValueType>::BinaryTree(){}

template<typename IntegralKeyType, typename ValueType>
BinaryTree<IntegralKeyType, ValueType>::~BinaryTree(){}


template<typename IntegralKeyType, typename ValueType>
void BinaryTree<IntegralKeyType, ValueType>::addOne(IntegralKeyType key, ValueType value){

}

template<typename IntegralKeyType, typename ValueType>
std::vector<IntegralKeyType> BinaryTree<IntegralKeyType, ValueType>::addFromVector(std::vector<std::pair<KeyType, ValueType>> KVpairs){
    auto returnVec = std::vector<IntegralKeyType>{};
    for (auto&& KV : KVpairs){
        returnVec.push_back(KV.first);
        addOne(KV.first, KV.second);
    }
    return returnVec;
}