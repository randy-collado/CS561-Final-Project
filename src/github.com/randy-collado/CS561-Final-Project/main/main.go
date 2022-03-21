package main

import (
	"fmt"
	"math"
	"os"
	"time"
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

func Insert(tree *BinaryTree, key int, value int) {
	SearchAndInsert(tree, key, value)
}

func OutputTree(tree BinaryTree, initialLevel int) {
	if tree.head == nil {
		return
	} else {
		OutputTree(BinaryTree{head: tree.head.left, size: 0, max_key: 0, min_key: 0}, initialLevel+1)
		fmt.Printf("Level %d: Key -> %d\n", initialLevel, tree.head.key)
		OutputTree(BinaryTree{head: tree.head.right, size: 0, max_key: 0, min_key: 0}, initialLevel+1)

	}
}

func NewTree() BinaryTree {
	return BinaryTree{head: nil, size: 0, max_key: 0, min_key: maxInt}
}

func Find(tree *BinaryTree, key int) *[]int {
	currentNode := tree.head
	var lastNode *Node
	if currentNode == nil {
		return nil
	}
	for currentNode != nil && currentNode.key != key {
		lastNode = currentNode
		if key < currentNode.key {
			currentNode = currentNode.left
		} else {
			currentNode = currentNode.right
		}
	}

	if currentNode != nil {
		return &currentNode.values
	}
	_ = lastNode
	return nil
}

func Delete(tree *BinaryTree, key int) {
	DeleteWithMatch(tree, key, nil)
}

func DeleteWithMatch(tree *BinaryTree, key int, predicate func(arg int) bool) bool {
	nodeValues := Find(tree, key)
	if nodeValues == nil {
		fmt.Fprintf(os.Stderr, "Error: key %d not found", key)
		return false
	}
	var newArr []int
	if predicate == nil {
		*nodeValues = []int{}
		return true
	}
	for i, r := range *nodeValues {
		if !predicate(r) {
			newArr = append(newArr, r)
		}
		_ = i
	}
	*nodeValues = newArr
	return true
}

func SearchAndInsert(tree *BinaryTree, key int, value int) {
	currentNode := tree.head
	var lastNode *Node
	if currentNode == nil {
		tree.head = &Node{left: nil, right: nil, key: key, values: []int{value}}
		return
	}
	for currentNode != nil && currentNode.key != key {
		lastNode = currentNode
		if key < currentNode.key {
			currentNode = currentNode.left
		} else {
			currentNode = currentNode.right
		}
	}

	if currentNode != nil {
		currentNode.values = append(currentNode.values, value)
	} else {
		if key < lastNode.key {
			lastNode.left = &Node{left: nil, right: nil, key: key, values: []int{value}}
		} else {
			lastNode.right = &Node{left: nil, right: nil, key: key, values: []int{value}}
		}
	}
	tree.size++
}

func main() {
	tree := NewTree()
	Insert(&tree, 17, 5)
	Insert(&tree, 63, 5)
	Insert(&tree, 12, 5)
	Insert(&tree, 3, 5)
	Insert(&tree, 38, 5)
	Insert(&tree, 43, 5)
	Insert(&tree, 7, 5)
	Insert(&tree, 2, 5)
	Insert(&tree, 9, 5)
	Insert(&tree, 9, 7)

	key := 9

	fmt.Printf("Search Key %d\n", key)
	start := time.Now()
	ret := Find(&tree, key)
	end := time.Now()

	elapsed := end.Sub(start)

	fmt.Printf("key: %d -> ", key)
	fmt.Printf("value: %v\n", *ret)
	fmt.Printf("Elapsed time: %d ns\n", elapsed.Nanoseconds())

	fmt.Print("\n")

	fmt.Printf("Delete Key %d\n", key)
	start = time.Now()
	DeleteWithMatch(&tree, 9, func(arg int) bool {
		if arg < 6 {
			return true
		} else {
			return false
		}
	})
	end = time.Now()
	elapsed = end.Sub(start)
	ret = Find(&tree, key)

	fmt.Printf("key: %d -> ", key)
	fmt.Printf("value: %v\n", *ret)
	fmt.Printf("\nElapsed time: %d ns\n\n", elapsed.Nanoseconds())

	OutputTree(tree, 0)
}
