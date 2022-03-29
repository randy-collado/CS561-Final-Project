package main

import (
	"math/rand"
	"runtime"
	"sync"
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

var taskPool = sync.Pool{
	New: func() interface{} {
		return new(Task)
	},
}

var nodePool = sync.Pool{
	New: func() interface{} {
		return new(Node)
	},
}

func worker(glTaskChan chan *Task, num int) {
	var curTask *Task = nil
	var nextTask *Task = nil
	for {
		if found {
			// Have found, exit
			break
		}
		if curTask == nil {
			curTask = <-glTaskChan
		}

		// Check
		if curTask.node == nil || curTask.depth <= 0 {
			continue
		}
		if curTask.srhType == 1 { // BFS Task
			flag := false
			for _, chNode := range curTask.node.ch {
				if chNode.key == curTask.key {
					// Found it, exit
					found = true
					return
				}
				tmpTask := taskPool.Get().(*Task)
				tmpTask.node = chNode
				tmpTask.srhType = 1
				tmpTask.depth = 1
				tmpTask.key = curTask.key
				// tmpTask := Task{chNode, 1, 1, curTask.key}
				if !flag {
					nextTask = tmpTask
					flag = true
				} else {
					// Send new task
					glTaskChan <- tmpTask
				}

			}
		} // else if other searching type

		// Finish this task
		taskPool.Put(curTask)
		curTask = nextTask
		nextTask = nil
	}
}

func scheduler(root *Node, resKey int) {
	found = false

	cpus := runtime.NumCPU()
	runtime.GOMAXPROCS(cpus)

	workerNum := cpus

	glTaskChan := make(chan *Task, 1000000)

	for i := 0; i < workerNum; i++ {
		go worker(glTaskChan, i)
	}
	initTask := taskPool.Get().(*Task)
	initTask.node = root
	initTask.depth = 1
	initTask.key = resKey
	initTask.srhType = 1
	glTaskChan <- initTask

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
		gen_tree(&tmpNode, curDepth-1)
	}
	// for _, chNode := range curNode.ch {
	// 	gen_tree(chNode, curDepth-1)
	// }
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

func serial_dfs2(curNode *Node, resKey int, depthLeft int) bool {
	if curNode.key == resKey {
		return true
	}

	if depthLeft == 0 {
		return false
	}

	for _, chNode := range curNode.ch {
		if serial_dfs2(chNode, resKey, depthLeft-1) {
			return true
		}
	}
	return false
}

func serial_iddfs(root *Node, resKey int, maxDepth int) {
	for i := 1; i <= maxDepth; i++ {
		if serial_dfs2(root, resKey, maxDepth) {
			println("IDDFS Found")
			return
		}
	}
	println("IDDFS Not Found")
}

func main() {
	rand.Seed(time.Now().UnixNano())

	root := Node{-1, nil, nil}
	maxDepth := 18
	genCnt = 0
	gen_tree(&root, maxDepth)

	println(genCnt)
	resKey := rand.Intn(genCnt)

	startTime := time.Now()
	sdRes := serial_dfs(&root, resKey)
	if sdRes {
		println("DFS found")
	} else {
		println("DFS Not found")
	}
	elapTime := time.Since(startTime) / time.Millisecond
	println("Serial DFS: ", elapTime, "ms")

	startTime = time.Now()
	serial_bfs(&root, resKey)
	elapTime = time.Since(startTime) / time.Millisecond
	println("Serial BFS: ", elapTime, "ms")

	startTime = time.Now()
	serial_iddfs(&root, resKey, maxDepth)
	elapTime = time.Since(startTime) / time.Millisecond
	println("Serial IDDFS: ", elapTime, "ms")

	startTime = time.Now()
	scheduler(&root, resKey)
	elapTime = time.Since(startTime) / time.Millisecond
	println("Parallel BFS: ", elapTime, "ms")
}
