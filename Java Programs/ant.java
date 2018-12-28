// Sopheap Sok
// 2/2/2018
// Program: Ant Tunneling 
// Program implement Prim's algorithm to determine the minimum total
// cost to dig a tunnel network that connects all ant hills.

import java.util.*;

public class ant
{	
	ArrayList<edge> map;
	
	// Constructor that create ant object, map, as an ArrayList of edge.
	public ant()
	{
		map = new ArrayList<edge>();
	}
	
	
	// Method that implements Prim's algorithm. Takes in the array of ArrayList storing the 
	// edge objects and a starting vertex.
	public static int Prims(ant[] paths, int vertex)
	{
		// Initialize the total cost counter, boolean used array and the priority queue.  
		int totalcost = 0;
		boolean[] used = new boolean[paths.length];
		PriorityQueue<edge> pq = new PriorityQueue<edge>();
		
		//Add all the edges for the given vertex to the priority queue and mark the vertex as used.
		for (int i=0; i<paths[vertex].map.size(); i++)
			pq.add(paths[vertex].map.get(i));
		used[vertex] = true;
		
		// Loop through until the priority queue is empty and remove the smallest edge cost and
		// add to total if the vertex is not in the used array. 
		while (!pq.isEmpty())
		{
			// Remove the edge with the smallest cost and check if it is already used. If so,
			// continue to next iteration.
			edge e = pq.poll();
			
			if (used[e.V])
				continue;
			
			// Vertex has not been used, so its cost is added to the total and all the edges
			// connected to the vertex is added to the priority queue and vertex is marked as used.
			else
			{
				totalcost += e.cost;
				for (int i=0; i<paths[e.V].map.size(); i++)
					pq.add(paths[e.V].map.get(i));
				used[e.V] = true;
			}
			
		}
		
		return totalcost;
	}
	
	public static void main(String[] args)
	{
		
		Scanner input = new Scanner(System.in);
		int numCampus = input.nextInt();
		int totalcost;
		
		// Process all campus or towns of ant hills.
		for (int campus=1; campus<=numCampus; campus++)
		{
			int hills = input.nextInt();
			int numtunnels = input.nextInt();
					
			// Check if a solution can be found given the number of hills to connect and the available
			// tunnels to dig. 
			if (!(numtunnels >= (hills-1)))
			{
				System.out.print("Campus #" + campus + ":" + " ");
				System.out.println("I'm a programmer, not a miracle worker!");
				
				// It is determined that a solution can't be found, so all input pertaining to the campus
				// must be skipped, or the next iteration will be invalid. 
				int skip;
				for (int j=0; j<numtunnels; j++)
				{
					for (int i=0; i<3; i++)
						skip = input.nextInt();
				}
				continue;
				
			}
			
			// Declare an array of ant objects and filling the array with an ArrayList. 
			// Creation of an array of ArrayList.
			ant[] paths = new ant[hills+1];
			for(int i=0;i<=hills;i++) 
				paths[i] = new ant();

			// Iterate through each tunnel.
			for (int i=0; i<numtunnels; i++)
			{
				// Scan in each vertex of an edge and its cost.
				int v1 = input.nextInt();
				int v2 = input.nextInt();
				int cost = input.nextInt();
				
				// Create edge objects with the vertex and cost.
				// Insert opposite edge to corresponding vertex in the array.  
				edge insertvertex1 = new edge(v2, cost);
				edge insertvertex2 = new edge(v1, cost);
				paths[v1].map.add(insertvertex1);
				paths[v2].map.add(insertvertex2);
			}
			
			// Call Prims method and store return value in totalcost to be outputted. 
			totalcost = Prims(paths, 1);
			
			System.out.println("Campus #" + campus + ":" + " " + totalcost);
		}
		input.close();
		
	}
}

// Class that creates edge objects and implements comparable for the priority queue.
class edge implements Comparable<edge>
{
	int V, cost;
	
	public edge(int V, int cost)
	{
		this.V = V;
		this.cost = cost;
	}
	
	public int compareTo(edge v)
	{
		return this.cost - v.cost;
	}
}
