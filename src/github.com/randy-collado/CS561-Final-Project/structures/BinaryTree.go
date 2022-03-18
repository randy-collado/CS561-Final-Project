package structures

import (
	"fmt"
	"math"
)

const maxInt = math.MaxInt32

type BinaryTree struct {
	head    *Node
	size    int
	max_key int
	min_key int
}

type Node struct {
	left   *Node
	right  *Node
	key    int
	values []int
}

func Insert(tree BinaryTree, key int, value int) {
	SearchAndInsert(tree, key, value)
}

func OutputTree(tree BinaryTree, initialLevel int) {
	if tree.head == nil {
		return
	} else {
		fmt.Printf("Level %d: Key -> %d", initialLevel, tree.head.key)
		OutputTree(BinaryTree{head: tree.head.left, size: 0, max_key: 0, min_key: 0}, initialLevel+1)
		OutputTree(BinaryTree{head: tree.head.right, size: 0, max_key: 0, min_key: 0}, initialLevel+1)

	}
}

func NewTree() BinaryTree {
	return BinaryTree{head: nil, size: 0, max_key: 0, min_key: maxInt}
}

func SearchAndInsert(tree BinaryTree, key int, value int) {
	currentNode := tree.head
	var lastNode *Node
	for currentNode != nil || (*currentNode).key != key {
		lastNode = currentNode
		if key < currentNode.key {
			currentNode = currentNode.left
		} else {
			currentNode = currentNode.right
		}
	}

	if currentNode != nil {
		slice := append(currentNode.values, value)
		currentNode.values = slice
	} else {
		if key < lastNode.key {
			lastNode.left = &Node{left: nil, right: nil, key: key, values: []int{value}}
		} else {
			lastNode.right = &Node{left: nil, right: nil, key: key, values: []int{value}}
		}
	}
	tree.size++
}
