package main

import (
	"github.com/randy-collado/CS561-Final-Project/structures"
)

func main() {
	tree := NewTree()
	Insert(tree, 3, 5)
	outputTree(tree, 0)
}
