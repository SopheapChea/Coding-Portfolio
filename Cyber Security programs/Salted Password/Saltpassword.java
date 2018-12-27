// Sopheap Sok
// so683950
// CIS 3360 0R02, Spring 2018
// 4/5/2018
// Program Assignment 3: Salted Password
// Program conduct a dictionary attack by hashing every password and comparing the hash values.

import java.util.*;
import java.math.*;
import java.io.*;

public class Saltpassword
{
	public static void main(String[] args) throws Exception
	{
		// Prompt user to enter name of file; catch filename; create a new File and read with Scanner.
		System.out.println("Please enter the name of the password file.");
		Scanner passwordFile = new Scanner(System.in);
		String filename = passwordFile.next();
		File file = new File(filename);
		
		// Make sure the file entered exist.
		if (!file.exists())
		{
			System.out.println("Error: file does not exist.");
			return;
		}
		
		// Make sure there is stuff in the file.
		if (file.length() == 0)
		{
			System.out.println("Error: file is empty.");
			return;
		}
		
		Scanner input = new Scanner(file);
		
		// Sent pointer to file to method createDictionary() to create dictionary file.
		createDictionary(input);
		
		// Call method compareHashValue() to compare user input hash value to those in the dictionary file.
		compareHashValue();
		
		// Create new scanner to read in if user would like to input another hash value to compare. 
		Scanner tryagain = new Scanner(System.in);
		
		// Keep asking user if they want to continue entering hash value to compare and keep calling method 
		// compareHashValue() if answer scanned in is "y".
		// Otherwise break loop and end program. 
		while(true)
		{
			System.out.println("Do you want to try again? (y/n):");
			if (tryagain.next().equals("y"))
				compareHashValue();
			else
				break;
		}
	}
	
	// Method takes user input hash value and compare it to those in the dictionary file.
	public static void compareHashValue() throws Exception
	{
		// Declare entries counter and open dictionary file.
		int entries = 0;
		File dictionaryFile = new File("dictionary.txt");
		Scanner dictionary = new Scanner(dictionaryFile);
		
		// Prompt user to enter hash value to compare; scan input in and store in targetValue string.
		System.out.println("Please enter the target hash value.");
		Scanner target = new Scanner(System.in);
		String targetValue = target.next();
		
		// Loop through dictionary file.
		while (dictionary.hasNext())
		{
			// Store the password, salt, and hash value of each line in file; increment entries counter.
			String Pword = dictionary.next();
			String salt = dictionary.next();
			String hashVal = dictionary.next();
			entries++;
			
			// Compare input value to the hash values in file.
			// If match is found, output the password and salt associated with the value; output value of
			// entries counter and return to main.
			if (hashVal.equals(targetValue))
			{
				System.out.println("Password was recovered!");
				System.out.println("Password: " + " " + Pword);
				System.out.println("Salt value:" + " " + salt);
				System.out.println("Number of entries searched" + " " + entries);
				return;
			}
		}
		
		// At this point the whole dictionary file has been searched and no match have been found.
		System.out.println("Password was not found!");
		System.out.println("Number of entries searched" + " " + entries);
	}
	
	// Method takes pointer to input file and call other methods to calculate hash value for each password and salt combinations,
	// writing all results into a file name dictionary.txt.
	public static void createDictionary(Scanner input) throws IOException
	{
		// Create file to write to.
		File file = new File("dictionary.txt");
		FileWriter fileWriter = new FileWriter(file);
		PrintWriter printWriter = new PrintWriter(fileWriter);
		
		// Loop through all password in file.
		while (input.hasNext())
		{
			// For each password, method convert() and hashFuntion() is called 1000 times for the 1000
			// different salt that can be append to the front of the password. 
			String password = input.next();
			for (int salt=0; salt<1000; salt++)
			{
				// Method convert() is called to take the password and change them to their ASCII values and append salt to the front.
				String sequence = convert(salt, password);
				
				// Method hashFuntion() is called to calculate the hash value for each sequence of a given password.
				long hashValue = hashFuntion(sequence);
				
				// Make the salt 3 digit long.
				String padSalt = String.format("%03d", salt);
				
				// Write to dictionary file.
				printWriter.println(password + " " + padSalt + " " + hashValue);
			}
		}
		
		// Close all files.
		input.close();
		printWriter.close();
	}
	
	// Method calculate hash values according to a hash function. 
	public static long hashFuntion(String sequence)
	{
		long hashValue = 0;
		
		// Split the sequence into a right and left for calculations. 
		StringBuilder left = new StringBuilder();
		StringBuilder right = new StringBuilder();
		
		for (int i=0; i<8; i++)
			left.append(sequence.charAt(i));
		
		for (int i=8; i<sequence.length(); i++)
			right.append(sequence.charAt(i));
		
		String leftstr = left.toString();
		String rightstr = right.toString();
		
		// Convert the left and right strings to longs.
		long leftValue = Long.parseLong(leftstr);
		long rightValue = Long.parseLong(rightstr);
		
		// Calculate the hash value.
		hashValue = ((243*leftValue) + rightValue) % 85767489;
		
		return hashValue;
	}
	
	// Method takes the salt and pad it and then combine it with the converted password. 
	public static String convert(int salt, String password)
	{
		// Take each character of the password and change it to its ASCII representation. 
		StringBuilder str = new StringBuilder();
		for (char c: password.toCharArray())
			str.append((int)c);
		
		// Pad the salt to make it 3 digits in length.
		String pad = String.format("%03d", salt);
		
		// Combine the salt and converted password.
		String withoutSalt = str.toString();
		String sequence = pad + withoutSalt;
		
		return sequence;
	}
	
}