package main

import (
	"fmt"
	"math"
	"os"
	"time"
)

const maxInt = math.MaxInt32

type tree interface {
	Insert(key int, value int)
	Find(key int) *[]int
	Delete(key int)
	DeleteWithMatch(key int, predicate func(arg int) bool) bool
}

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

func (tree *BinaryTree) Insert(key int, value int) {
	tree.SearchAndInsert(key, value)
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

func New() *BinaryTree {
	return &BinaryTree{head: nil, size: 0, max_key: 0, min_key: maxInt}
}

func (tree *BinaryTree) Find(key int) *[]int {
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

func (tree *BinaryTree) Delete(key int) {
	tree.DeleteWithMatch(key, nil)
}

func (tree *BinaryTree) DeleteWithMatch(key int, predicate func(arg int) bool) bool {
	nodeValues := tree.Find(key)
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

func (tree *BinaryTree) SearchAndInsert_concurrent(key int, value int){
	currentNode := tree.head
	var lastNode *Node
	if currentNode == nil {
		tree.head = &Node{left: nil, right: nil, key: key, values: []int{value}}
		return
	}
	for currentNode != nil && currentNode.key != key && !(currentNode.left != nil && currentNode.right != nil) {
		lastNode = currentNode
		if key < currentNode.key {
			currentNode = currentNode.left
		} else {
			currentNode = currentNode.right
		}
	}

	if currentNode.left != nil && currentNode.right != nil{
		
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

func (tree *BinaryTree) SearchAndInsert(key int, value int) {
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

func DeleteTest(tree *BinaryTree, key int) {
	fmt.Printf("Delete Key %d\n", key)
	start := time.Now()
	tree.DeleteWithMatch(9, func(arg int) bool {
		if arg < 6 {
			return true
		} else {
			return false
		}
	})
	end := time.Now()
	elapsed := end.Sub(start)
	ret := tree.Find(key)

	fmt.Printf("key: %d -> ", key)
	fmt.Printf("value: %v\n", *ret)
	fmt.Printf("\nElapsed time: %d ns\n\n", elapsed.Nanoseconds())
}
func InsertTest(tree *BinaryTree) {
	tree.Insert(17, 5)
	tree.Insert(63, 5)
	tree.Insert(12, 5)
	tree.Insert(3, 5)
	tree.Insert(38, 5)
	tree.Insert(43, 5)
	tree.Insert(7, 5)
	tree.Insert(2, 5)
	tree.Insert(9, 5)
	tree.Insert(9, 7)

	key := 9

	fmt.Printf("Search Key %d\n", key)
	start := time.Now()
	ret := tree.Find(key)
	end := time.Now()

	elapsed := end.Sub(start)

	fmt.Printf("key: %d -> ", key)
	fmt.Printf("value: %v\n", *ret)
	fmt.Printf("Elapsed time: %d ns\n", elapsed.Nanoseconds())

	fmt.Print("\n")
}

func main() {
	tree := New()
	InsertTest(tree)
	DeleteTest(tree, 9)
	OutputTree(*tree, 0)
}
