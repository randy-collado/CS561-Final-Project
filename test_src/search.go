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

func worker(task_ch chan *Task, resChan chan *Node, finChan chan int, num int) {
	// fmt.Printf("Worker %d starts\n", num)
	for {
		if found {
			// Have found, exit
			break
		}
		// Wait for task
		task := <-task_ch
		// Check
		if task.node == nil || task.depth <= 0 {
			return
		}
		// println("Process:", node.key, "from worker", num)
		if task.srhType == 1 { // BFS Task
			for _, chNode := range task.node.ch {
				if chNode.key == task.key {
					// Found it, exit
					found = true
					finChan <- num
					return
				}
				// Send new node
				resChan <- chNode
			}
		} // else if other searching type

		// Finished one task
		finChan <- num
	}
	// println("Worker ends", num)
}

func scheduler(root *Node, resKey int) {

	maxRes := 100
	maxTask := 5
	found = false

	resChan := make(chan *Node, maxRes)

	cpus := runtime.NumCPU()
	runtime.GOMAXPROCS(cpus)

	workerNum := cpus - 1

	var taskChans [64]chan *Task
	var taskCnt [64]int

	for i := 0; i < workerNum; i++ {
		taskCnt[i] = 0
		taskChans[i] = make(chan *Task, maxTask)
	}

	finChan := make(chan int, workerNum)

	for i := 0; i < workerNum; i++ {
		go worker(taskChans[i], resChan, finChan, i)
	}

	var glTask []*Task

	initTask := Task{root, 1, 1, resKey}

	// glTask = append(glTask, &initTask)
	taskChans[0] <- &initTask
	taskCnt[0] += 1

	for {
		if found {
			println("P_BFS found")
			break
		}
		select {
		case k := <-finChan:
			// println("worker", k, "finish")

			// Worker k finished
			taskCnt[k] -= 1
			// println("worker", k, "taskCnt", taskCnt[k])

			// Any task left?
			if len(glTask) > 0 {
				// Give one to worker k immediately
				taskCnt[k] += 1
				taskChans[k] <- glTask[0]
				// println("Send to worker", k)
				glTask = glTask[1:]
			}
		case res := <-resChan:
			// Receive new node, gsenerate task
			// TODO: More balancer and schedules here

			newTask := Task{res, 1, 1, resKey}
			// If there's an idle worker, give to it
			flag := false
			for i := 0; i < workerNum; i++ {
				if taskCnt[i] == 0 {
					taskCnt[i] += 1
					taskChans[i] <- &newTask
					// println("Send to worker", i)
					flag = true
					break
				}
			}
			// Otherwise, put it to global list
			if !flag {
				// println("No idle workers")
				glTask = append(glTask, &newTask)
			}

		case <-time.After(time.Microsecond * 1000):
			// Check if no more workers running and results waited
			if len(glTask) == 0 { // No tasks left
				// All workers idle?
				isFin := true
				for i := 0; i < workerNum; i++ {
					if taskCnt[i] > 0 {
						isFin = false
						break
					}
				}
				if isFin {
					// Yes, finished
					if found {
						println("P_BFS found")
					} else {
						println("P_BFS Not found")
					}
					return
				}
				// Some workers running, continue waiting
			}

		}
	}

}

func gen_tree(curNode *Node, curDepth int) {
	if curDepth <= 0 {
		return
	}
	for i := 0; i < rand.Intn(8); i++ {
		tmpNode := Node{rand.Intn(1000), nil, nil}
		curNode.ch = append(curNode.ch, &tmpNode)
		gen_tree(&tmpNode, curDepth-1)
	}
}

func serial_bfs(root *Node, resKey int) {
	var nodeList []*Node
	nodeList = append(nodeList, root)
	flag := false
	for {
		if flag || len(nodeList) == 0 {
			break
		}
		curNode := nodeList[0]
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
	resKey := 10000

	root := Node{-1, nil, nil}
	maxDepth := 20
	gen_tree(&root, maxDepth)

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
