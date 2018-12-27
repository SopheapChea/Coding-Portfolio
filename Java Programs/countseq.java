// Sopheap Sok
// so683950
// COP 3503C-0001, Spring 2018
// 4/4/2018
// Program #5: Counting Subsequences
// Program uses dynamic programing to determine the number of occurrences of a given subsequence in a given sequence of letters.

import java.util.*;

public class countseq
{
	public static void main(String[] args)
	{
		long answer = 0;
		Scanner input = new Scanner(System.in);
		int numTest = input.nextInt();
		
		// Process all cases.
		for (int i=0; i<numTest; i++)
		{
			String sequence = input.next();
			String subSeq = input.next();
	
			answer = dynamicCount(sequence, subSeq);
			
			System.out.println(answer);
		}
	}
	
	// Method uses dynamic programing to find number of subsequence in a sequence of letters.
	public static long dynamicCount(String sequence, String subSeq)
	{
		
		int lengthX = sequence.length();
		int lengthY = subSeq.length();
		
		// 2D array to store results of subproblems.
		long[][] table = new long[lengthX+1][lengthY+1];
		
		// Fill the first row of the table with 1s to represent empty character.
		// The first column contains all 0s except the first block.
		for (int i=0; i<=lengthX; i++)
			table[i][0] = 1;
		
		// Loop through the array and fill it up bottom to top.
		for (int i=1; i<=lengthX; i++)
		{
			for (int j=1; j<=lengthY; j++)
			{
				// If the last characters of prefixes match, 
				// add the left cell and the upper left cell together. 
				if (sequence.charAt(i-1) == subSeq.charAt(j-1))
					table[i][j] = table[i-1][j-1] + table[i-1][j];
				
				// Otherwise, take the value of the previous cell.
				else
					table[i][j] = table[i-1][j];
			}
		}
		
		// The answer is store in the last cell of the table. 
		return table[lengthX][lengthY];
	}
	
}