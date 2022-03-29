package main

import (
	"math/rand"
	"runtime"
	"time"
)

type Node struct {
	key    int
	ch     []*Node
	values []int
}

type Task struct {
	node    *Node
	srhType int
	depth   int
	key     int
	// other task args
}

var found bool

var terminated bool

func worker(glTaskChan chan *Task, num int) {
	var curTask *Task
	for {
		if found {
			// Have found, exit
			break
		}
		curTask = <-glTaskChan
		// Check
		if curTask.node == nil || curTask.depth <= 0 {
			continue
		}
		if curTask.srhType == 1 { // BFS Task
			for _, chNode := range curTask.node.ch {
				if chNode.key == curTask.key {
					// Found it, exit
					found = true
					return
				}
				// Send new task
				glTaskChan <- &Task{chNode, 1, 1, curTask.key}
			}
		} // else if other searching type

		// // Finished one task
		// finChan <- num
	}
}

func scheduler(root *Node, resKey int) {
	found = false

	cpus := runtime.NumCPU()
	runtime.GOMAXPROCS(cpus)

	workerNum := cpus - 1

	glTaskChan := make(chan *Task, 10000000)

	for i := 0; i < workerNum; i++ {
		go worker(glTaskChan, i)
	}

	initTask := Task{root, 1, 1, resKey}
	glTaskChan <- &initTask

	timeOut := 0
	for {
		if found {
			println("P_BFS Found")
			break
		}
		if timeOut >= 3 {
			println("P_BFS Not Found")
			break
		}
		<-time.After(time.Microsecond * 50)

		// Check if no more workers running and results waited
		if len(glTaskChan) == 0 {
			timeOut++
		} else {
			timeOut = 0
		}
	}

}

var genCnt int

func gen_tree(curNode *Node, curDepth int) {
	if curDepth <= 0 {
		return
	}
	for i := 0; i < rand.Intn(8); i++ {
		tmpNode := Node{genCnt, nil, nil}
		genCnt++
		curNode.ch = append(curNode.ch, &tmpNode)
		// gen_tree(&tmpNode, curDepth-1)
	}
	for _, chNode := range curNode.ch {
		gen_tree(chNode, curDepth-1)
	}
}

func serial_bfs(root *Node, resKey int) {
	var nodeList []*Node
	nodeList = append(nodeList, root)
	flag := false
	var curNode *Node
	for {
		if flag || len(nodeList) == 0 {
			break
		}
		curNode = nodeList[0]
		nodeList = nodeList[1:]
		for _, chNode := range curNode.ch {
			if chNode.key == resKey {
				flag = true
				break
			}
			nodeList = append(nodeList, chNode)
		}
	}
	if flag {
		println("BFS Found")
	} else {
		println("BFS Not Found")
	}
}

func serial_dfs(curNode *Node, resKey int) bool {
	if curNode.key == resKey {
		return true
	}

	for _, chNode := range curNode.ch {
		if serial_dfs(chNode, resKey) {
			return true
		}
	}
	return false
}

func main() {
	rand.Seed(time.Now().UnixNano())

	root := Node{-1, nil, nil}
	maxDepth := 18
	genCnt = 0
	gen_tree(&root, maxDepth)

	println(genCnt)
	resKey := genCnt - 1

	startTime := time.Now()
	serial_bfs(&root, resKey)
	elapTime := time.Since(startTime) / time.Millisecond
	println("Serial BFS: ", elapTime, "ms")

	startTime = time.Now()
	sdRes := serial_dfs(&root, resKey)
	if sdRes {
		println("DFS found")
	} else {
		println("DFS Not found")
	}
	elapTime = time.Since(startTime) / time.Millisecond
	println("Serial DFS: ", elapTime, "ms")

	startTime = time.Now()
	scheduler(&root, resKey)
	elapTime = time.Since(startTime) / time.Millisecond
	println("Parallel BFS: ", elapTime, "ms")
}
