// Sopheap Sok
// COP 4600: Programming assignment 2
// Program simulate Disk Scheduling. Program implements the ﬁrst-come-ﬁrst-served,
// shortest seek time ﬁrst, SCAN, C-SCAN, LOOK, and C-LOOK scheduling algorithms.
// 11/30/18

package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"strings"
	"strconv"
	"sort"
)

func main() {
	
	// Read input file from command line
	file, err := ioutil.ReadFile(os.Args[1])
	if err != nil {
		fmt.Println("Can not read input file.")
		panic(err)
	}
	
	// Open and read file
	input := string(file)
	keyword := strings.Fields(input)
	
	// Determine what operation to perform based on file input
	switch keyword[1]{
	
		case "fcfs":
			fcfs(keyword)
		case "sstf":
			sstf(keyword)
		case "scan":
			SCAN(keyword)
		case "c-scan":
			cScan(keyword)
		case "look":
			LOOK(keyword)
		case "c-look":
			cLook(keyword)
		default:
			fmt.Println("Algorithm not recognize")	
	}
		
}

// Function handle all errors that could occur
func handleErrors (initial int, upper int, lower int, file []string) []int{
	var request int
	var skip []int
	
	// ABORT errors
	if upper < lower {
		fmt.Printf("ABORT(13):upper (%d) < lower (%d)\n", upper, lower)
		os.Exit(13)
	}

	if initial < lower {
		fmt.Printf("ABORT(12):initial (%d) < lower (%d)\n", initial, lower)
		os.Exit(12)
	}
	
	if initial > upper {
		fmt.Printf("ABORT(11):initial (%d) > upper (%d)\n", initial, upper)
		os.Exit(11)
	}
	
	// Handle request errors. Occur if requests are larger than upper bound or lower than lower bound.
	// Will print out error requests and add the request to a slice to skip over.
	for i := range file {
		
		if file[i] == "cylreq"{
			
			i++
			request,_ = strconv.Atoi(file[i])
			
			if request > upper || request < lower {
				fmt.Printf("ERROR(15):Request out of bounds: req (%d) > upper (%d) or  < lower (%d)\n", request, upper, lower)
				skip = append(skip, request)
			}
		}
	}
	
	// Return a slice of skipped requests.
	return skip
}

// Function will only print out requests that are within the bounds.
func handleSkips (file []string, skip []int) []int{
	var request int
	var skipped bool
	var requests []int
	s := 0
	j := 0
	
	// Loop through input file and only add requests that are not in the skip slice to the requests slice.
	for i := range file {
				
		if file[i] == "cylreq"{
			
			request,_ = strconv.Atoi(file[i+1])
			
			for s != len(skip) {
			
				if skip[j] == request {
					s = len(skip)
					skipped = true
				}
				if j != len(skip) - 1 { 
					j++
				} else {break}
				s++
			}
			s = 0
			j = 0
			
			if skipped == false {
				requests = append(requests, request)
			}
			
			skipped = false
		}
	
	}
	
	fmt.Printf("\tCylinder requests:\n")
	for j := range requests { fmt.Printf("\t\tCylinder %5d\n", requests[j])}

	// Return slice of requests that are within the bounds.
	return requests

}

// Function extract the initial parameters from file and print accordingly. 
func parameters(file []string, algorithm string) (int, int, int, []int){
	var lower, upper, initial int

	// Extract parameters info from input file.
	for i := range file {
		if file[i] == "lowerCYL"{
			lower,_ = strconv.Atoi(file[i+1])
		}
		
		if file[i] == "upperCYL"{
			upper,_ = strconv.Atoi(file[i+1])
		}
		
		if file[i] == "initCYL"{
			initial,_ = strconv.Atoi(file[i+1])
		}
		
		if file[i] == "cylreq"{break}
		
	
	}
	
	// Call handleErrors function to fine skip requests and handle any errors.
	skip := handleErrors(initial, upper, lower, file)
	
	// Print out correct operation that is being used.
	switch algorithm {
	
		case "fcfs":
			fmt.Println("Seek algorithm: FCFS")
		case "sstf":
			fmt.Println("Seek algorithm: SSTF")
		case "scan":
			fmt.Println("Seek algorithm: SCAN")
		case "c-scan":
			fmt.Println("Seek algorithm: C-SCAN")
		case "look":
			fmt.Println("Seek algorithm: LOOK")
		case "c-look":
			fmt.Println("Seek algorithm: C-LOOK")
		default:
			fmt.Println("Error: Algorithm not recognize")		
	}
	
	// Print out parameters
	fmt.Printf("\tLower cylinder: %5d\n", lower)
	fmt.Printf("\tUpper cylinder: %5d\n", upper)
	fmt.Printf("\tInit cylinder:  %5d\n", initial)
	
	// Return parameters and skip slice.
	return lower, upper, initial, skip
}

// Function perform First Come First Serve disk algorithm.
// The First-ComeFirst-Served algorithm is just that: disk requests are fulﬁlled in the order of their arrival.
func fcfs (file []string){
	var requests []int
	count := 0
	
	// Call parameters function to extract needed parameters from file.
	_, _, initial, skip := parameters(file, "fcfs")
	
	// Call handleSkips function to produce a slice that only hold requests that are within the bounds.
	requests = handleSkips(file, skip)
	
	// Loop through the requests slice and calculate the travel distance.
	for j := range requests {
		fmt.Printf("Servicing %5d\n", requests[j])
		
		if j == 0 {
			if requests[j] > initial {
				count = requests[j] - initial
			} else { count = initial - requests[j] }
		} else {
			if requests[j] > requests[j-1] {
				count += requests[j] - requests[j-1]
			} else { count += requests[j-1] - requests[j] }
		}
	}
	
	fmt.Printf("FCFS traversal count = %5d\n", count)
}

// Function perform Shortest Seek Time First disk algorithm.
// The Shortest Seek Time First design goal is to move the shortest possible distance
// ﬁrst - thru to the end of the current list of disk requests.
func sstf(file [] string) {
	var requests []int
	var backwards, ordered bool 
	insert := 0
	count := 0
	
	// Call parameters function to extract needed parameters from file.
	_, _, initial, skip := parameters(file, "sstf")
	
	// Call handleSkips function to produce a slice that only hold requests that are within the bounds.
	requests = handleSkips(file, skip)
	
	// Sort requests from small to large.
	sort.Ints(requests)
	
	// Loop though requests slice and determine which scenario to use, go in order (start from beginning of slice),
	// go to the left of initial first, or go to the right of initial first.
	for k := range requests {
		
		// Start from beginning of slice.
		if (k == 0) && (requests[k] > initial) {
			insert = 0
			ordered = true
			break
		}
		
		// Determine if initial is closes to request on the left or right of it.
		if requests[k] > initial {
			
			// Determine the start position
			if (initial - requests[k-1]) > (requests[k] - initial) {
					insert = k
			} else {
				insert = k-1
				
			}
			
			// Determine if the slice needs to be traversed backwards.
			if (requests[insert] - requests[insert - 1]) < (requests[insert+1] - requests[insert]) {
				backwards = true
				break
			} else {break}
		}
		
		// Condition if a request have the save value as the start value.
		if requests[k] == initial {
			insert = k
			
			if (requests[insert] - requests[insert - 1]) < (requests[insert+1] - requests[insert]) {
				backwards = true
				break
			} else {break}
			
		}
	}	
	
	// Initial is closes to request to the left of it. 
	if backwards == true {
	
		// Traverse the requests slice backwards, while calculating the travel distance.
		for j := insert; j >= 0; j-- {
			fmt.Printf("Servicing %5d\n", requests[j])
		
			if j == insert {
				if requests[j] > initial {
					count = requests[j] - initial
				} else { count = initial - requests[j] }
			} else {
				if j != 0 {
					count += requests[j+1] - requests[j] 
				} else {
					count += requests[j+1] - requests[j]
					break
				}
				
			}
		
		}
		
		// Now travel to the other side of initial, and keep calculating the travel distance.
		for j := insert+1; j < len(requests); j++ {
			fmt.Printf("Servicing %5d\n", requests[j])
			
			if j == insert+1 {
				count += requests[j] - requests[0]
			} else {count += requests[j] - requests[j-1]}
		}
	
	}
	
	// Initial is closer to the request to the right of it.
	if backwards == false && ordered == false {
	
		// Traverse slice forward starting from request closest to initial, while calculating the travel distance.
		for j := insert; j < len(requests); j++ {
			fmt.Printf("Servicing %5d\n", requests[j])
			
			if j == insert {
				if requests[j] > initial {
					count = requests[j] - initial
				} else { count = initial - requests[j] }
			} else {count += requests[j] - requests[j-1]}
		}
		
		// Now travel to the other side of initial, and keep calculating the travel distance.
		for j := insert-1; j >= 0; j-- {
			fmt.Printf("Servicing %5d\n", requests[j])
			
			if j == (insert - 1) {
				count += requests[len(requests)-1] - requests[j]
			} else { 
				if j != 0 {
				count += requests[j+1] - requests[j] 
				} else {
					count += requests[j+1] - requests[j]
					break
				}
			}
		}
	
	}
	
	// Initial is closest to first request in slice. Go in order and calculate travel distance.
	if ordered == true {
		for j := insert; j < len(requests); j++ {
			fmt.Printf("Servicing %5d\n", requests[j])
			
			if j == insert {
				count = requests[j] - initial
			} else {count += requests[j] - requests[j-1]}
		}
	}
	
	fmt.Printf("SSTF traversal count = %5d\n", count)
}

// Function perform Scan disk algorithm.
/* SCAN works like an elevator does. Starting from the initial cylinder, the head will traverse upwards 
while processing requests. Upon reaching the upper cylinder, the head will then traverse to the lower cylinder
while processing requests. This process will remain active as long as there are unfulﬁlled requests */
func SCAN(file [] string) {
	var requests []int
	var ordered bool
	insert := 0
	count := 0 
	
	// Call parameters function to extract needed parameters from file.
	_, upper, initial, skip := parameters(file, "scan")
	
	// Call handleSkips function to produce a slice that only hold requests that are within the bounds.
	requests = handleSkips(file, skip)
	
	// Sort requests from small to large.
	sort.Ints(requests)
	
	// Loop though requests slice and determine which scenario to use, go in order (start from beginning of slice),
	// or go to the right of initial first.
	for k := range requests {
		
		// Requests can be filled in order.
		if (k == 0) && (requests[k] > initial) {
			insert = 0
			ordered = true
			break
		}
		
		// Find request with shortest distance from initial.
		if (requests[k] > initial) || (requests[k] == initial) {
			insert = k
			break
		}
	}
	
	// Either requests are served from start or from within the slice. 
	if ordered == true {
		for j := insert; j < len(requests); j++ {
			fmt.Printf("Servicing %5d\n", requests[j])
			
			if j == insert {
				count = requests[j] - initial
			} else {count += requests[j] - requests[j-1]}
		}
	
	} else {
		
		// Starting within the slice.
		for j := insert; j < len(requests); j++ {
			fmt.Printf("Servicing %5d\n", requests[j])
			
			if j == insert {
				count = requests[j] - initial
			} else {count += requests[j] - requests[j-1]}
		}
		
		// Reached the last request, calculate distance from it to upper bound and add to total distance count.
		if upper > requests[len(requests)-1] {
			count += upper - requests[len(requests)-1]
		}
		
		// Travel back down slice, while serving requests and calculating distance.
		for j := insert-1; j >= 0; j-- {
			fmt.Printf("Servicing %5d\n", requests[j])
			
			if j == (insert - 1) {
				count += upper - requests[j]
			} else { 
				if j != 0 {
				count += requests[j+1] - requests[j] 
				} else {
					count += requests[j+1] - requests[j]
					break
				}
			}
		}
	}
	
	fmt.Printf("SCAN traversal count = %5d\n", count)
}

// Function perform C-Scan disk algorithm.
/* Circular scanning(C-SCAN) works just like the elevator to some extent. It begins its scan upward and works it way all
the way to the end of the system. Once it hits the top it jumps to the other end and moves in the same direction. */
func cScan(file [] string) {
	var requests []int
	var ordered bool
	insert := 0
	count := 0 
	
	// Call parameters function to extract needed parameters from file.
	lower, upper, initial, skip := parameters(file, "c-scan")
	
	// Call handleSkips function to produce a slice that only hold requests that are within the bounds.
	requests = handleSkips(file, skip)
	
	// Sort requests from small to large.
	sort.Ints(requests)
	
	// Loop though requests slice and determine which scenario to use, go in order (start from beginning of slice),
	// or go to the right of initial first.
	for k := range requests {
		
		// Requests can be filled in order.
		if (k == 0) && (requests[k] > initial) {
			insert = 0
			ordered = true
			break
		}
		
		// Find request with shortest distance from initial.
		if (requests[k] > initial) || (requests[k] == initial) {
			insert = k
			break
		}
	}
	
	// Either requests are served from start or from within the slice. 
	if ordered == true {
		for j := insert; j < len(requests); j++ {
			fmt.Printf("Servicing %5d\n", requests[j])
			
			if j == insert {
				count = requests[j] - initial
			} else {count += requests[j] - requests[j-1]}
		}
	
	} else {
		
		// Starting within the slice.
		for j := insert; j < len(requests); j++ {
			fmt.Printf("Servicing %5d\n", requests[j])
			
			if j == insert {
				count = requests[j] - initial
			} else {count += requests[j] - requests[j-1]}
		}
		
		// Reached the last request, calculate distance from it to upper bound and add to total distance count.
		if upper > requests[len(requests)-1] {
			count += upper - requests[len(requests)-1]
		}
		
		// Reached the upper bound travel to bottom and calculate the distance.
		count += upper - lower
		
		// Start from being and go to first request filled.
		for j := 0; j < insert; j++ {
			fmt.Printf("Servicing %5d\n", requests[j])
			
			if j == 0 {
				count += requests[j] - lower
			} else { count += requests[j] - requests[j-1]}
		}
	
	
	}
	
	fmt.Printf("C-SCAN traversal count = %5d\n", count)
}

// Function perform Look disk algorithm.
/* The LOOK algorithm is the same as the SCAN algorithm in that it also honors requests on both sweep direction of the disk head,however,
this algorithm "Looks" ahead to see if there are any request spending in the direction of head movement. */
func LOOK(file [] string) {
	var requests []int
	var ordered bool
	insert := 0
	count := 0 
	
	// Call parameters function to extract needed parameters from file.
	_, _, initial, skip := parameters(file, "look")

	// Call handleSkips function to produce a slice that only hold requests that are within the bounds.
	requests = handleSkips(file, skip)
	
	// Sort requests from small to large.
	sort.Ints(requests)
	
	// Loop though requests slice and determine which scenario to use, go in order (start from beginning of slice),
	// or go to the right of initial first.
	for k := range requests {
		
		// Requests can be filled in order.
		if (k == 0) && (requests[k] > initial) {
			insert = 0
			ordered = true
			break
		}
		
		// Find request with shortest distance from initial.
		if (requests[k] > initial) || (requests[k] == initial) {
			insert = k
			break
		}
	}
	
	// Either requests are served from start or from within the slice. 
	if ordered == true {
		for j := insert; j < len(requests); j++ {
			fmt.Printf("Servicing %5d\n", requests[j])
			
			if j == insert {
				count = requests[j] - initial
			} else {count += requests[j] - requests[j-1]}
		}
	
	} else {
		
		// Starting within the slice.
		for j := insert; j < len(requests); j++ {
			fmt.Printf("Servicing %5d\n", requests[j])
			
			if j == insert {
				count = requests[j] - initial
			} else {count += requests[j] - requests[j-1]}
		}
		
		// Reached last request moving up, head down.
		for j := insert-1; j >= 0; j-- {
			fmt.Printf("Servicing %5d\n", requests[j])
			
			if j == (insert - 1) {
				count += requests[len(requests)-1] - requests[j]
			} else { 
				if j != 0 {
				count += requests[j+1] - requests[j] 
				} else {
					count += requests[j+1] - requests[j]
					break
				}
			}
		}
	
	
	}
	
	fmt.Printf("LOOK traversal count = %5d\n", count)
}

// Function perform C-Look disk algorithm.
/* C-LOOK uses the LOOK algorithm with the tweak of when the disk arm gets to the end of the disk, it jumps immediately
back to the other end. As in C-SCAN, C-LOOK is a circular LOOK algorithm. */
func cLook(file [] string) {
	var requests []int
	var ordered bool
	insert := 0
	count := 0 
	
	// Call parameters function to extract needed parameters from file.
	_, _, initial, skip := parameters(file, "c-look")
	
	// Call handleSkips function to produce a slice that only hold requests that are within the bounds.
	requests = handleSkips(file, skip)
	
	// Sort requests from small to large.
	sort.Ints(requests)
	
	// Loop though requests slice and determine which scenario to use, go in order (start from beginning of slice),
	// or go to the right of initial first.
	for k := range requests {
		
		// Requests can be filled in order.
		if (k == 0) && (requests[k] > initial) {
			insert = 0
			ordered = true
			break
		}
		
		// Find request with shortest distance from initial.
		if (requests[k] > initial) || (requests[k] == initial) {
			insert = k
			break
		}
	}
	
	// Either requests are served from start or from within the slice.
	if ordered == true {
		for j := insert; j < len(requests); j++ {
			fmt.Printf("Servicing %5d\n", requests[j])
			
			if j == insert {
				count = requests[j] - initial
			} else {count += requests[j] - requests[j-1]}
		}
	
	} else {
		
		// Starting within the slice.
		for j := insert; j < len(requests); j++ {
			fmt.Printf("Servicing %5d\n", requests[j])
			
			if j == insert {
				count = requests[j] - initial
			} else {count += requests[j] - requests[j-1]}
		}
		
		// Reached the top; go to bottom and head back up until reached the first request filled.
		for j := 0; j < insert; j++ {
			fmt.Printf("Servicing %5d\n", requests[j])
			
			if j == 0 {
				count += requests[len(requests)-1] - requests[0]
			} else { count += requests[j] - requests[j-1]}
		}
	
	}
	
	fmt.Printf("C-LOOK traversal count = %5d\n", count)
}
