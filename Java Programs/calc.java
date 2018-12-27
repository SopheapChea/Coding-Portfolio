// Sopheap Sok
// so683950
// COP 3503C-0001, Spring 2018
// 3/8/2018
// Program #4: The a-b-c Calculator
// Program uses BFS to determine the fewest number of button pressed needed to obtain the target value on the calculator display.

import java.util.*;

public class calc
{	
	public static void main(String[] args)
	{
		Scanner input = new Scanner(System.in);
		int numbInput = input.nextInt();
		
		// Process all cases.
		for(int i=0; i<numbInput; i++)
		{
			int a = input.nextInt();
			int b = input.nextInt();
			int c = input.nextInt();
			int t = input.nextInt();
	
			BFS(a, b, c, t);
		}
	}
	
	// Method is called by BFS() method to calculate the adjacent nodes of the value polled from the queue.
	public static int[] findAdj(int a, int b, int c, int num)
	{	
		int addVal, mulVal, divVal;
		int[] adjacent = new int[3];
		
		// Calculate each adjacent nodes to num.
		addVal = num + a;
		mulVal = num * b;
		divVal = num / c;
		
		// Mod all three values to keep it no more than 6 digit length and store in an array to return.
		adjacent[0] = addVal%1000000;
		adjacent[1] = mulVal%1000000;
		adjacent[2] = divVal%1000000;
		
		return adjacent;
	}
	
	// Method uses BFS to find the shortest path to the target value.
	public static void BFS(int a, int b, int c, int t)
	{
		int [] nodes;
		
		// Create the queue and add the starting value in it.
		Queue<Integer> queue = new LinkedList<Integer>();
		queue.offer(0);
		
		// Distance array keep track of the distance of each node from the start and also act as a used array.
		int[] distance = new int[1000000];
		Arrays.fill(distance, -1);
		distance[0] = 0;
		
		// Loop until there are no more values in the queue.
		while (!queue.isEmpty())
		{	
			// Grab the head of the queue.
			int v = queue.poll();
			
			// Call method to calculate the adjacent notes of the value poll from queue.
			nodes = findAdj(a, b, c, v);
			
			// Loop through all three adjacent nodes of v.
			for (int adj: nodes)
			{
				// If the nodes match the target value then print out its distance, which is the distance of
				// its parent node plus 1.
				if (adj == t)
				{
					System.out.println(distance[v]+1);
					return;
				}
				
				// The value poll from the queue has not been seen. Calculate the distance, 
				// store in the array, and add to queue.
				if (distance[adj] == -1)
				{
					distance[adj] = distance[v] + 1;
					queue.add(adj);
					
				}	
			}
		}
		// BFS has finish and the target value has not been found, thus this case is unsolvable. 
		System.out.println("-1");	
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}