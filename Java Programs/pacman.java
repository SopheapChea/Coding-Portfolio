// Sopheap Sok
// 4/19/2018
// Program: Number of Optimal Pac Man Paths 
// Program uses dynamic programming to determine the maximum possible score Pac Man can achieve,
// the number of different paths that achieve that score and the first alphabetic path that yields the maximum possible score. 

import java.util.*;
import java.lang.*;

public class pacman
{
	public static void main(String[] args)
	{
		Scanner input = new Scanner(System.in);
		int r = input.nextInt();
		int c = input.nextInt();
		
		// Case where only one move is needed to get to the end.
		if (r == 2 && c == 1)
		{
			System.out.println("0" + " " + "1");
			System.out.println("D");
			return;
		}
		
		// Case where only one move is needed to get to the end.
		if (r == 1 && c == 2)
		{
			System.out.println("0" + " " + "1");
			System.out.println("R");
			return;
		}
		
		// Scan in game board into a 2D array.
		int[][] graph = new int[r][c];
			
		// Scan in the first letter.
		if (input.next().equals("P"))
			graph[0][0] = 0;
			
		// 1D format
		if (r == 1)
		{
			for (int j=1; j<c-1; j++)
				graph[0][j] = input.nextInt();
		}
			
		else if (c == 1)
		{
			for (int i=1; i<r-1; i++)
				graph[i][0] = input.nextInt();
		}
			
		// 2D format
		else
		{
			// Scan in the first rows.
			for(int j=1; j<c; j++)
					graph[0][j] = input.nextInt();
				
			// Scan in the middle rows.
			for (int i=1; i<r-1; i++)
			{
				for(int j=0; j<c; j++)
					graph[i][j] = input.nextInt();
			}
				
			// Scan in the last row.
			for(int j=0; j<c-1; j++)
				graph[r-1][j] = input.nextInt();
		}
			
		// Scan in the last letter.
		if (input.next().equals("E"))
			graph[r-1][c-1] = 0;
		
		// Call PathAndValue() method to solve problem.
		PathAndValue(graph, r, c);
		
		// Close scanner.
		input.close();
		return;
	}
	
	// Method derived the path and value array for the game board and call other methods to
	// determine the number of optimal paths and path reconstruction.
	public static void PathAndValue(int[][] graph, int r, int c)
	{
		// Declare 2D arrays for the path and value.
		String[][] path = new String[r][c];
		int[][] value = new int[r][c];
		
		// Initialize the path array with all "X".
		for (int i=0; i<path.length; i++)
			Arrays.fill(path[i], "X");
		
		// Case for 1D game boards.
		if (r == 1)
		{
			// Value right of the start is always itself.
			value[0][1] = graph[0][1];
			
			// Since it's a 1D board, only one type of move can be made, in this case only moves to the right.
			for (int i=1; i<c; i++)
				path[0][i] = "R";
			
			// Combine the previous value with current value to calculate traveled value.
			value[0][2] = graph[0][2] + graph[0][1];
			for (int i=3; i<c; i++)
				value[0][i] = graph[0][i] + value[0][i-1];
			
		}
		
		// Case for 1D game boards.
		else if (c == 1)
		{
			// Value below of the start is always itself.
			value[1][0] = graph[1][0];
			
			// Since it's a 1D board, only one type of move can be made, in this case only moves down.
			for (int i=1; i<r; i++)
				path[i][0] = "D";
			
			// Combine the previous value with current value to calculate traveled value.
			value[2][0] = graph[2][0] + graph[1][0];
			for (int i=2; i<r; i++)
				value[i][0] = graph[i][0] + value[i-1][0];
			
		}
		
		// Case for 2D game board.
		else
		{
			// Value below and to the right of the start is always itself.
			value[0][1] = graph[0][1];
			value[1][0] = graph[1][0];
			
			// Fill the top row with the only possible moves, which is right.
			for (int i=1; i<c; i++)
				path[0][i] = "R";
			
			// Fill the first column with the only possible moves, which is down.
			for (int i=1; i<r; i++)
				path[i][0] = "D";
			
			// Calculate the travel values for the first row and column if the board is larger than a 2X2.
			if (r != 2 && c != 2)
			{				
				value[2][0] = graph[2][0] + graph[1][0];
				for (int i=2; i<r; i++)
					value[i][0] = graph[i][0] + value[i-1][0];
				
				value[0][2] = graph[0][2] + graph[0][1];
				for (int i=3; i<c; i++)
					value[0][i] = graph[0][i] + value[0][i-1];
			}
			
			// Loop through all three arrays to fill in value and path arrays.
			for (int i=1; i<=r-1; i++)
			{
				for(int j=1; j<=c-1; j++)
				{
					// Calculate the current traveled value by determining whether from above or to the left 
					// will the max value be given.
					value[i][j] = Math.max(value[i-1][j], value[i][j-1]) + graph[i][j];
					
					// Depending whether the value to the left or above of the current location is larger, smaller,
					// or the same, the direction traveled is determine and stored in the path array. 
					if (value[i-1][j] == value[i][j-1])
						path[i][j] = "B";
					
					else if (value[i][j-1] > value[i-1][j])
						path[i][j] = "R";	
					
					else 
						path[i][j] = "D";
				}
			}
		}
		
		// Output the last cell of the 2D value array, which will hold the max possible score.
		System.out.print(value[r-1][c-1]);	
		
		// Call numOfPaths() and pathReconstruction() methods to determine the total number of optimal paths
		// and the reconstruction of the first optimal path.
		numOfPaths(path, r, c);
		pathReconstruction(path, r, c);
		
		return;
	}
	
	
	// Method determine the total number of optimal paths.
	public static void numOfPaths(String path[][], int r, int c)
	{
		// There is only one path if the game board is 1D.
		if (r == 1 || c == 1)
		{
			System.out.print(" " + "1");
			return;
		}
		
		// Declare a 2D totalPaths array to calculate to total number of optimal paths.
		int[][] totalPaths = new int[r][c];
		
		// Initialize the first row and column of the array to 1, since there is only one path possible
		// in all locations in first row and column.
		for (int i=0; i<totalPaths[0].length; i++)
			totalPaths[0][i] = 1;
		
		for (int i=0; i<totalPaths.length; i++)
			totalPaths[i][0] = 1;
		
		//Loop through the path and totalPaths arrays.
		for (int i=1; i<totalPaths.length; i++)
		{
			for (int j=1; j<totalPaths[i].length; j++)
			{
				// When "B" is encounter in the path array, we know that from both above and to left optimal path is 
				// possible, so we combine both the above and left value.
				if (path[i][j] == "B")
					totalPaths[i][j] = ((totalPaths[i-1][j] + totalPaths[i][j-1]) % 1000000007);
				
				// When "R" is encounter, the value to the left is taken.
				else if (path[i][j] == "R")
					totalPaths[i][j] = totalPaths[i][j-1];
				
				// When "D" is encounter, the value above is taken.
				else if (path[i][j] == "D")
					totalPaths[i][j] = totalPaths[i-1][j];
			}
		}
		
		// Output the last cell in totalPaths arrays, which holds the total number of optimize path.
		System.out.print(" " + totalPaths[r-1][c-1]);
		return;
	}
	
	// Method reconstruction the first alphabetic path.
	public static void pathReconstruction(String path[][], int r, int c)
	{
		StringBuilder firstPath = new StringBuilder();
		
		// Loop backwards through the path array.
		outerloop:
		for (int i=r-1; i>=0; i--)
		{
			for (int j=c-1; j>=0; j--)
			{
				// We've reached the start, exit both loop.
				if (path[i][j] == "X")
					break outerloop;
				
				// When "B" or "R" is reached we add "R" to front of string.
				if (path[i][j] == "B" || path[i][j] == "R")
					firstPath.insert(0,"R");
				
				// "D" is reached; insert "D" to front of string and set up loop to go up in array.
				else 
				{
					firstPath.insert(0,"D");
					j++;
					i--;
				}
				
			}
		}
		
		// Output the path store in firstPath.
		System.out.println();
		System.out.println(firstPath.toString());
		
		return;
	}
	
}
