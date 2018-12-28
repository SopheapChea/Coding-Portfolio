// Sopheap Sok
// 1/19/2018
// Program: Sudoku Solver
// Program implement backtracking to solve Sudoku puzzles.

import java.util.*;

public class sudoku
{
	final public static int N = 9;
	final public static int FREE = 0;
	final public static int SQUARE = 3; 
	private int[][] puzzle;
	
	// Constructor creates a puzzle from command line file input and store in a 2D array.
	public sudoku(Scanner input)
	{
		puzzle = new int[N][N];
			
		for (int r=0; r<N; r++)
		{
			for (int c=0; c<N; c++)
				puzzle[r][c] = input.nextInt();
		}
	}
	
	public static void main(String[] args)
	{
		Scanner input = new Scanner(System.in);
		int numPuzzles = input.nextInt();
		
		// Process all puzzles.
		for (int game=1; game<=numPuzzles; game++)
		{
			// Create a sudoku object by calling the constructor with the input to make the puzzle.
			sudoku myPuzzle = new sudoku(input);
			
			System.out.println("Test case "+game+":"+"\n");
			
			// Call solve() method on sudoku object and print out solution by calling
			// printPuzzle() method if a solution is found.
			if (myPuzzle.solve())
				myPuzzle.printPuzzle();
			
			else
				System.out.print("No solution possible.\n\n\n");
		}
	}
	
	// Backtracking method that search for free or empty spaces in the puzzle and insert possible
	// number from 1-9 by calling on duplicateRow(), duplicateCol(), and duplicateBox() methods to see if
	// the number is found in the specific block's row, column, or 3x3 box.
	private boolean solve()
	{
		int r = 0, c = 0;
		
		// Loop through the 2D array storing the puzzle to find empty spaces marked by 0s.
		for (r=0; r<N; r++)
		{
			for (c=0; c<N; c++)
			{
				// If an open place is found, loop through possible number 1-9 and assign the first possible number.
				if (puzzle[r][c] == FREE)
				{
					for (int tryNum=1; tryNum<=N; tryNum++)
					{
						// Check for duplication in row, column, and 3x3 box and assign number if all three method returns false.
						if (!duplicateRow(r, tryNum) && !duplicateCol(c, tryNum) && !duplicateBox(r, c, tryNum))
						{
							puzzle[r][c] = tryNum;
							
							// Call method again on next available place. 
							if (solve())
								return true;
							
							// Backtrack if solve() method returns false to previous free place to try a different number. 
							else puzzle[r][c] = FREE;
						}
					}
					// No solution can be found.
					return false;
				}
			}
		}
		return true;
	}
	
	// Check for duplication of a given number at a specified row by looping through each column in the row and returning true if
	// the number is found and false otherwise. 
	private boolean duplicateRow(int r, int tryNum)
	{
		for (int c=0; c<N; c++)
		{
			if (puzzle[r][c] == tryNum)
				return true;
		}
		return false;
	}
	
	// Check for duplication of a given number at a specified column by looping through each row in the column and returning true if
	// the number is found and false otherwise.
	private boolean duplicateCol(int c, int tryNum)
	{
		for (int r=0; r<N; r++)
		{
			if (puzzle[r][c] == tryNum)
				return true;
		}
		return false;
	}
	
	// Check for duplication of a given number at a specified 3x3 box by first calculating the location of the 3x3 box given the row and 
	// column of the free space and looping through each space in the box and returning true if the number is found and false otherwise.
	private boolean duplicateBox(int r, int c, int tryNum)
	{
		int row, col;
		
		row = r - r%SQUARE;
		col = c - c%SQUARE;
		
		for (int i=0; i<SQUARE; i++)
		{
			for(int j=0; j<SQUARE; j++)
			{
				if(puzzle[row + i][col + j] == tryNum)
					return true;
			}
		}
		return false;
	}
	
	// Simple method to print out the solved puzzle by looping through the 2D array. 
	private void printPuzzle()
	{
		for (int r= 0; r<puzzle.length; r++) 
		{
			for (int c=0; c<puzzle[r].length; c++) 
				System.out.print(puzzle[r][c] + " ");
					
			System.out.println();
		}
		System.out.println("\n");
	}
}
