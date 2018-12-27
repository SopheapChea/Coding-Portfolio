// Sopheap SOk
// COP 4600: Programming assignment 1
// **Note sjf does not work!!!
// Program simulate the CPU scheduling of several processes for the conﬁgured First-Come First-Served, preemptive Shortest Job First, and Round-Robin CPU scheduling algorithms.

package main

import (
	"fmt"
	"io/ioutil"
	"bufio"
	"os"
	"strings"
	"strconv"
	"container/list"
)

var process[10] string
var originalProcess[10] string
var originalArrival [10] int
var burst [10]int
var originalBurst [10]int
var arrivalTime [10] int


func main() {
	var finishTime,	process int
	
	file, err := ioutil.ReadFile(os.Args[1])
	fileOut,errOut := os.Create(os.Args[2])
	w := bufio.NewWriter(fileOut)
	
	if err != nil || errOut != nil {
		fmt.Println("Can not read input file or can not open output file.")
		panic(err)
	}
	defer fileOut.Close()
	
	
	
	input := string(file)
	parse := strings.Fields(input)
	
	for i, _ := range parse {
	
		if (parse[i]) == "processcount" {
			
			process, _ = strconv.Atoi(parse[i+1])
			fmt.Fprintf(w, " %2d processes\n", process)
		}
		w.Flush()
		
		if parse[i] == "runfor" {
			finishTime, _= strconv.Atoi((parse[i+1]))
		}
		
		if parse[i] == "use" {
			if parse[i+1] == "fcfs" {
				fcfs(parse, i+1, finishTime, process, w)
			} else if parse[i+1] == "rr" {
				rr(parse, i+1, finishTime, process, w)
			} else if parse[i+1] == "sjf" {
				sjf(parse, i+1, finishTime, process, w)
			} else {fmt.Println("Error! System does not recognize the scheduling algorithm")}
		}
	}
	
}

func ScanAndOrder ( parse []string, i int, finishTime int, numProcess int) {
	var p, c int
	b := 1
	
	
	for i, _ := range parse {
	
		if parse[i] == "name" {
			i++
			process[p] = parse[i]
			originalProcess[p] = parse[i]
			
			i++
			
			
			if parse[i] == "arrival" {
				i++
				arrivalTime[p],_ = strconv.Atoi(parse[i])
				originalArrival[p] = arrivalTime[p] 
				i++
				
				if parse[i] == "burst" {
					i++
					burst[p],_ = strconv.Atoi(parse[i])
					originalBurst[p] = burst[p]
					i++
					
				}
				
				if p != 0 {
					//fmt.Printf("arrivalTime[%d] = %d > arrivalTime[%d] = %d\n", p-b, arrivalTime[p-b], p, arrivalTime[p])

					if arrivalTime[p-b] > arrivalTime[p-c] {
					
						//fmt.Printf("arrivalTime[%d] = %d > arrivalTime[%d] = %d\n", p-b, arrivalTime[p-b], p, arrivalTime[p])
						temp := arrivalTime[p-b]
						arrivalTime[p-b] = arrivalTime[p]
						arrivalTime[p] = temp
						
						tempName := process[p-b]
						process[p-b] = process[p]
						process[p] = tempName
						
						tempBurst := burst[p-b]
						burst[p-b] = burst[p]
						burst[p] = tempBurst
						
						b++
						c++
						if p != 1 {
							for arrivalTime[p-b] > arrivalTime[p-c] {
								temp := arrivalTime[p-b]
								arrivalTime[p-b] = arrivalTime[p-c]
								arrivalTime[p-c] = temp
								
								tempName := process[p-b]
								process[p-b] = process[p-c]
								process[p-c] = tempName
								
								tempBurst := burst[p-b]
								burst[p-b] = burst[p-c]
								burst[p-c] = tempBurst
								
								b++
								c++
							}
						}
						b = 1
						c = 0
					}
				}				
			}
			p++
		}		
	} 
}

func fcfs (parse []string, i int, finishTime int, numProcess int, w *bufio.Writer) {
	 
	var completed [10] int
	var selected [10] int
	var k, idle, TotalBurst, t, a, j, selectTime, betweenIdle int
	allArrived := true 
	indexMap := make(map[string]int)
	
	fmt.Fprintln(w,"Using First-Come First-Served")

	ScanAndOrder (parse, i+1, finishTime, numProcess)
	
	for z, _ := range originalProcess {
		indexMap[originalProcess[z]] = j
			j++
	}
	
	/* for s:= 0; s<finishTime; s++ {
		fmt.Printf("process: %s arrival: %d  burst: %d\n", process[s], arrivalTime[s], burst[s])
	} */
	
	for _, num := range burst {
		TotalBurst += num
	}
	
	i = 0
	
	fmt.Fprintf(w, "Time  %2d : %s arrived\n", arrivalTime[a], process[i])
	fmt.Fprintf(w, "Time  %2d : %s selected (burst  %2d)\n", t, process[i], burst[i])
	selected[indexMap[process[i]]] = t

	for t = 0; t<finishTime; t++ {
	
		// Check if current burst time is greater or equal to the arrive of the next processes.
		// Allow for other processes to arrive before current process is completed. 
		if i < (numProcess - 1) {
			k = i + 1
			
			for burst[i] >= arrivalTime[k] {
			
				if k < a {break}
				
				fmt.Fprintf(w, "Time  %2d : %s arrived\n", arrivalTime[k], process[k])
				t = arrivalTime[k]
				k++
				a = k
			}
		}	
		//fmt.Printf("burst[%d] = %d, arrivalTime[%d] = %d\n", i, burst[i], k, arrivalTime[k])
		
		//fmt.Printf("%d = %d\n", t, (burst[i] + selectTime))
		
		// Check if the current process is completed.
		if t == (burst[i] + selectTime) {
			fmt.Fprintf(w, "Time  %2d : %s finished\n", burst[i] + selectTime, process[i])
			completed[indexMap[process[i]]] = burst[i] + selectTime
			
			if i == numProcess - 1 {break}
			
			if (burst[i] + t) < arrivalTime[i+1] {
				idle = arrivalTime[i+1] - (burst[i] + t)
				
				for j:= 0; j<idle; j++ {
				fmt.Fprintf(w, "Time  %2d : Idle\n", t)
				t++
				}
				
			} else if burst[i] < arrivalTime[i+1] {
			
				if i == 0 {
					idle = arrivalTime[i+1] - burst[i]
					
					for j:= 0; j<idle; j++ {
					fmt.Fprintf(w, "Time  %2d : Idle\n", t)
					t++
					betweenIdle++
					}
					i++
					fmt.Fprintf(w, "Time  %2d : %s arrived\n", arrivalTime[i], process[i])
				} else {i++}
				
			} else { i++}
			
			// Select the next process if there are processes left.
			if i < numProcess {
			fmt.Fprintf(w, "Time  %2d : %s selected (burst  %2d)\n", t, process[i], burst[i])
			selectTime = t
			selected[indexMap[process[i]]] = t
			}
		}
		
		// Check if the completion time of the current process is greater than the arrival time of
		// the next process. If so, other processes is allow to arrive.
		if allArrived == true {
			if (burst[i] + t) >= arrivalTime[a] {
				if t != 0 {
					if i != numProcess - 1 {
						
						for a != numProcess {
							
							fmt.Fprintf(w, "Time  %2d : %s arrived\n", arrivalTime[a], process[a])
							t = arrivalTime[a]
							
							//fmt.Printf("a: %d\n", a)

							if a == (numProcess-1) {
								allArrived = false
								break
							} else {a++}
							
							//fmt.Printf("a: %d\n", a)

							if t == (burst[i] + selectTime) {break}
							if arrivalTime[a] > (burst[i] + selectTime) {break}
								
							//fmt.Printf("a: %d\n", a)
							//fmt.Printf("burst[%d] + %d <= arrivalTime[%d]\n", i, t,a)
							if (burst[i] + t) <= arrivalTime [a] {break}
								
						}
					}
				}
			}
		}
		
		if t ==  TotalBurst {break}
		//fmt.Printf("i: %d\n", i)
		t = burst[i] + selectTime - 1
	}
	
	if (TotalBurst + betweenIdle) != finishTime {
		idle = finishTime - (TotalBurst + betweenIdle)
			
		for j:= 0; j<idle; j++ {
			fmt.Fprintf(w, "Time  %2d : Idle\n", t)
			t++
		}
	}
	
	fmt.Fprintf(w, "Finished at time  %d\n\n", finishTime)
	
	for j:= 0; j<numProcess; j++ {
		fmt.Fprintf(w, "%s wait  %2d turnaround  %2d\n", originalProcess[j],selected[j] - originalArrival[j],completed[j] - originalArrival[j])
	}
	w.Flush()
}

func rr(parse []string, i int, finishTime int, numProcess int, w *bufio.Writer) {
	var quantum, a, t, TotalBurst int
	allArrived := true
	queue := list.New()
	remainingBurst := make(map[string]int)
	finish := make(map[string]int)

	for i, _ := range parse {
		if parse[i] == "quantum" {
			i++
			quantum,_ = strconv.Atoi(parse[i])
			break;
		}
	}
	
	
	fmt.Fprintln(w, "Using Round-Robin")
	fmt.Fprintf(w, "Quantum   %d\n\n",quantum)
	
	ScanAndOrder(parse, i, finishTime, numProcess)
	
	for _, num := range burst {
		TotalBurst += num
	}
	

	
	fmt.Fprintf(w, "Time  %2d : %s arrived\n", arrivalTime[a], process[a])
	fmt.Fprintf(w, "Time  %2d : %s selected (burst  %2d)\n", t, process[a], burst[a])
	queue.PushBack(process[a])
	remainingBurst[process[a]] = burst[a]
	a++
	pop := queue.Front()
	pulled := pop.Value.(string)
	queue.Remove(pop)
	
	for t<finishTime {
		if queue.Len() != 0 {
			pop := queue.Front()
			pulled = pop.Value.(string)
			queue.Remove(pop)
				
			fmt.Fprintf(w, "Time  %2d : %s selected (burst  %2d)\n", t, pulled, remainingBurst[pulled])
		} 
		
		if allArrived == true {
			for arrivalTime[a] <= (t + quantum) && arrivalTime[a] <= remainingBurst[pulled] + t{
				fmt.Fprintf(w, "Time  %2d : %s arrived\n", arrivalTime[a], process[a])
				queue.PushBack(process[a])
				remainingBurst[process[a]] = burst[a]
				
				if a == (numProcess - 1) {
					allArrived = false
					break
				} else {a++}
			} 
		}
		
		if remainingBurst[pulled] >= quantum {
			remainingBurst[pulled] -= quantum
			
			t += quantum
			
			if remainingBurst[pulled] != 0 {
				queue.PushBack(pulled)
			}
			
		} else if remainingBurst[pulled] < quantum {
			t += remainingBurst[pulled]
			remainingBurst[pulled] = 0
			
		}
	
		if remainingBurst[pulled] == 0 {
			fmt.Fprintf(w, "Time  %2d : %s finished\n", t, pulled)
			finish[pulled] = t
		} 
		
		if t == TotalBurst{
			for t < finishTime{
				fmt.Fprintf(w, "Time  %2d : Idle\n", t)
				t++
			}
		}
		
	
	}
	
	fmt.Fprintf(w, "Finished at time  %d\n\n", finishTime)
	
	for j:= 0; j<numProcess; j++ {
		fmt.Fprintf(w, "%s wait  %2d turnaround  %2d\n", originalProcess[j],finish[originalProcess[j]] - originalArrival[j] - originalBurst[j],finish[originalProcess[j]] - originalArrival[j])
	}
	
	w.Flush()
}

func sjf(parse []string, i int, finishTime int, numProcess int, w *bufio.Writer) { 
	var TotalBurst, a, t, betweenIdle int
	allArrived := true
	run := true
	sjfQueue := list.New()
	remainingBurst := make(map[string]int)
	finish := make(map[string]int)
	
	fmt.Fprintln(w, "Using preemptive Shortest Job First")
	
	ScanAndOrder(parse, i, finishTime, numProcess)
	
	for _, num := range burst {
		TotalBurst += num
	}
	
	fmt.Fprintf(w, "Time  %2d : %s arrived\n", arrivalTime[a], process[a])
	fmt.Fprintf(w, "Time  %2d : %s selected (burst  %2d)\n", t, process[a], burst[a])
	
	sjfQueue.PushBack(process[a])
	pop := sjfQueue.Front()
	currentProc := pop.Value.(string)
	sjfQueue.Remove(pop)
	remainingBurst[currentProc] = burst[a]
	a++
	i = 0
	for t < finishTime{
		
		if allArrived == true {
			//fmt.Println(remainingBurst[currentProc])
			Outer:
			for (remainingBurst[currentProc] + t) >= arrivalTime[a] {
				
				fmt.Fprintf(w, "Time  %2d : %s arrived\n", arrivalTime[a], process[a])
				
				// New arrival is less than current running
				if burst[a] < (remainingBurst[currentProc] - (arrivalTime[a] - t)) {
					
					remainingBurst[currentProc] -= (arrivalTime[a] - t)
					t = arrivalTime[a]
					
					if sjfQueue.Len() == 0 {
						sjfQueue.PushBack(currentProc)
						currentProc = process[a]
						remainingBurst[currentProc] = burst[a]
						fmt.Fprintf(w, "Time  %2d : %s selected (burst  %2d)\n", t, currentProc, remainingBurst[currentProc])
						
						if a == (numProcess - 1) {
							allArrived = false
							break
						} else {a++}
						
					} else {
						j := sjfQueue.Front()
						for run{
							
							if remainingBurst[currentProc] > remainingBurst[j.Value.(string)] {
								j = j.Next()
								
								if remainingBurst[currentProc] < remainingBurst[j.Value.(string)] {
									sjfQueue.InsertBefore(currentProc,j)
									run = false
									break
								} else {
									j = j.Next()
								}
							
							}
							
							if j == nil {
								sjfQueue.PushBack(currentProc)
								run = false
								break
							
							}
						
						}
						currentProc = process[a]
						remainingBurst[currentProc] = burst[a]
					
						if a == (numProcess - 1) {
							allArrived = false
							break
						} else {a++}
					
						fmt.Fprintf(w, "Time  %2d : %s selected (burst  %2d)\n", t, currentProc, remainingBurst[currentProc])
					}
				} else { // New arrival goes into queue
						
						if sjfQueue.Len() == 0 {
							
							sjfQueue.PushBack(process[a])
							remainingBurst[process[a]] = burst[a]
							
							if a == (numProcess - 1) {
								allArrived = false
								break Outer
							} else {a++}
							
						} else {
							
							j := sjfQueue.Front()
							num := 0
							for num < sjfQueue.Len(){
			
								if burst[a] > remainingBurst[j.Value.(string)] {
									if num != sjfQueue.Len()-1{
										j = j.Next()
										num++
									}
									
									if burst[a] < remainingBurst[j.Value.(string)] {
										sjfQueue.InsertBefore(process[a],j)
										remainingBurst[process[a]] = burst[a]
										run = false
										
										if a == (numProcess - 1) {
											allArrived = false
											break Outer
										} else {a++}
										
										break
									} else {
										if num != sjfQueue.Len(){
											j = j.Next()
											num++
										}
									}
								
								} else {
									if num != sjfQueue.Len()-1{
										j = j.Next()
										num++
									}
									
									if burst[a] > remainingBurst[j.Value.(string)] {
										sjfQueue.InsertBefore(process[a],j)
										remainingBurst[process[a]] = burst[a]
										
										if a == (numProcess - 1) {
											allArrived = false
											break Outer
										} else {a++}
										
										break
									} else {
										if num != sjfQueue.Len()-1{
											j = j.Next()
											num++
										}
									}
								
								
								}
								num++
								
								
			
							}
							sjfQueue.PushBack(process[a])
							remainingBurst[process[a]] = burst[a]
									
							if a == (numProcess - 1) {
								allArrived = false
									break Outer
							} else {a++}
									
							break
						}
					
					
					
					
					
					remainingBurst[currentProc] -= (arrivalTime[a-1] - t)
					//fmt.Println(remainingBurst[currentProc])
					t = arrivalTime[a-1]
					
				}
				/* fmt.Println(remainingBurst[currentProc])
				fmt.Println(t)
				fmt.Println(remainingBurst[currentProc]+t)
				fmt.Println(arrivalTime[a]) */
			} 
			
			if sjfQueue.Len() == 0 && (remainingBurst[currentProc] + t) < arrivalTime[a] {
				t += remainingBurst[currentProc]
				fmt.Fprintf(w, "Time  %2d : %s finished\n", t, currentProc)
				finish[currentProc] = t
				
				idle := arrivalTime[a] - t
				for j:= 0; j<idle; j++ {
					fmt.Fprintf(w, "Time  %2d : Idle\n", t)
					t++
					betweenIdle++
				}
				
				fmt.Fprintf(w, "Time  %2d : %s arrived\n", arrivalTime[a], process[a])
				fmt.Fprintf(w, "Time  %2d : %s selected (burst  %2d)\n", t, process[a], burst[a])
			
				sjfQueue.PushBack(process[a])
				pop := sjfQueue.Front()
				currentProc = pop.Value.(string)
				sjfQueue.Remove(pop)
				remainingBurst[currentProc] = burst[a]
				
				if a == (numProcess - 1) {
					allArrived = false
				} else {a++}
			} 
		}
		
		/* fmt.Println(remainingBurst[currentProc]+t)
		fmt.Println(arrivalTime[a]) */
		//fmt.Println(t)
		if (remainingBurst[currentProc] + t) < arrivalTime[a] || a == (numProcess-1) || remainingBurst[currentProc] == 0 {
				//fmt.Println(t)
			
				t += remainingBurst[currentProc]
				fmt.Fprintf(w, "Time  %2d : %s finished\n", t, currentProc)
				remainingBurst[currentProc] = 0
				finish[currentProc] = t
					
				if sjfQueue.Len() != 0 {
					pop := sjfQueue.Front()
					currentProc = pop.Value.(string)
					sjfQueue.Remove(pop)
					
					fmt.Fprintf(w, "Time  %2d : %s selected (burst  %2d)\n", t, currentProc, remainingBurst[currentProc])
					
					
					
				}
				//fmt.Println(currentProc)
			}
		
		 
		
		if t == (TotalBurst + betweenIdle){
			for t < finishTime{
				fmt.Fprintf(w, "Time  %2d : Idle\n", t)
				t++
			}
		}
		
	}

	fmt.Fprintf(w, "Finished at time  %d\n\n", finishTime)
	
	for j:= 0; j<numProcess; j++ {
		fmt.Fprintf(w, "%s wait  %2d turnaround  %2d\n", originalProcess[j],finish[originalProcess[j]] - originalArrival[j] - originalBurst[j],finish[originalProcess[j]] - originalArrival[j])
	}
}

















