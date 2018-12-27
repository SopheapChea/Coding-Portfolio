// Sopheap Sok
// so683950
// CIS 3360 0R02, Spring 2018
// 3/23/2018
// Programming Assignment 2: Program implement Cyclic Redundancy Check (CRC) calculation and verification.


import java.util.*;
import java.math.*;

public class crc
{
	// Static polynomial stored in an ArrayList.
	public static ArrayList<Integer> poly = new ArrayList<Integer>() 
	{{ 
		add(1); add(1); add(0); add(0); add(1); add(1); add(0); add(1); add(1); add(0); add(1); add(0); add(1);
	}};
	
	public static void main(String[] args)
	{
		// Flag is for verification.
		boolean flag = false;
		
		// Prompt user to enter message to be calculated, limiting the length to max of 5.
		System.out.print("Please enter your message in hexadecimal format(3-5 hex digits in length):");
		Scanner input = new Scanner(System.in);
		String hex = input.next();
		
		// Check to see if the input message is of length 5 or smaller. If length exceeds the max, an error message is outputted and program terminates.
		if (hex.length() > 5)
		{
			System.out.println("Error: Message length is too long. Program max size is 5 characters.");
			System.out.println("Input Size:" + hex.length()); 
			System.out.println("Program Terminated.");
			return;
		}
		
		// Check to make sure that the entered message is in hex.
		boolean isHex = hex.matches("[0-9a-fA-F]+");
		if (!isHex)
		{
			System.out.println("Error: Message entered is not in hexadecimal. Program Terminated.");
			return;
		}
		
		// Call HexToBinary() method to convert input message to binary.
		String binary = HexToBinary(hex);
		
		// Print out the polynomial to screen.
		System.out.print("Polynomial:" + " ");
		for (int i: poly)
			System.out.print(i);
		System.out.println();
		
		// Call StringToArray() method on the binary string to convert from String to ArrayList.
		ArrayList<Integer> PadMessage = StringToArray(binary);

		// Call calcCRC() method to calculated the CRC for the input message. Flag is still set to false.
		ArrayList<Integer> crc = calcCRC(PadMessage, flag);
		
		// Print to screen the original binary message and the calculated crc together.
		
		
		// Call BinaryToHex() method to convert the calculated message to hex.
		BinaryToHex(crc, binary);
		
		// Start the verification process by asking user to input message for verification.
		System.out.println("\nStarting the verification process...");
		System.out.print("Please enter hex message to be verify:" + " ");
		
		// Take user input and call CRCVerifcation() method.
		String verifyToBe = input.next();
		
		// Check to make sure that the entered message is in hex.
		isHex = verifyToBe.matches("[0-9a-fA-F]+");
		if (!isHex)
		{
			System.out.println("Error:Message entered is not in hexadecimal. Program Terminated.");
			return;
		}
		
		CRCVerifcation(verifyToBe, flag);
		
		return;
	}
	
	// Method takes the calculated crc array and original binary message, combine and convert to hex.
	public static void BinaryToHex(ArrayList<Integer> crc, String binary)
	{
		// Create a new string builder and copy from ArrayList .
		StringBuilder builder = new StringBuilder();
		for (int n: crc)
			builder.append(n);
		
		// Convert builder to string and append the original binary message to it.
		String crcString = builder.toString();
		String BinMessage = binary + crcString;
		
		// Print to screen the final message.
		System.out.print("Final appended massage: " + " ");
		System.out.println(BinMessage);
		
		// Convert final message from binary to hexadecimal.
		BigInteger dec = new BigInteger(BinMessage, 2);
		String hexMessage = dec.toString(16);
		
		// Print to screen the converted message
		System.out.print("Final appended message in Hex:" + " " + hexMessage);
	}
	
	// Method takes a string in hex and convert it to binary, padding it with the require number of 0s in the front so string remains a multiple of 4.
	public static String HexToBinary(String hex)
	{
		BigInteger dec = new BigInteger(hex, 16);
		String binary = dec.toString(2);
		String padBinary;
		
		// Pad with the require amount of 0s to be a multiple of 4.
		if (binary.length() % 4 != 0)
		{
			int pad = 4 - (binary.length() % 4);
			if (pad == 1)
				padBinary = "0" + binary;
			
			else
			{
				padBinary = "0" + binary;
				for(int i=0; i<pad-1; i++)
					padBinary = "0" + padBinary;
			}
		}
		
		// No padding is necessary. 
		else 
			padBinary = binary;
		
		System.out.println("\nYour message has been converted to binary:" + " " + padBinary);
		
		return padBinary;
	}
	
	// Method calculate the CRC for a giving message.
	public static ArrayList<Integer> calcCRC(ArrayList<Integer> PadMessage, boolean flag)
	{
		int startingZ = 0, j = 0, k = 0, total = 0;
		ArrayList<Integer> xorResult = new ArrayList<Integer>();
		ArrayList<Integer> crc = new ArrayList<Integer>();
		
		System.out.print("\nInitial data:" + "\t     ");
		for (int i: PadMessage)
			System.out.print(i);
		System.out.println();
		
		// Run until all portion of the message is used.
		while (k != PadMessage.size())
		{
			// For the verification process; Check to see if there are no remainder. 
			if (flag)
			{
				for (int check: PadMessage)
				{
					if (check == 0)
						total++;
					
				}
				
				if (total == PadMessage.size())
					return PadMessage;
				
			}
			
			// Go through the message and find where the first 1 is located.
			for (int i=0; i<PadMessage.size(); i++)
			{
				startingZ++;
				if (PadMessage.get(i) == 1)
					break;
			}
			
			startingZ -= 1;
			
			// For the verification process; Check to see if pointer is about to go out of bound in the next loop. If true, the verification failed,
			// so 1 is add to the front for the array and return to print fail message.
			if (flag)
			{
				if ((poly.size()+startingZ) > PadMessage.size())
				{
					PadMessage.add(0, 1);
					return PadMessage;
				}
			}
			
			// Prevent pointer from going out of bound in following loop.
			if ((poly.size()+startingZ) > PadMessage.size())
				break;

			// Loop through starting at the first 1 seen and xor the message with the polynomial, storing the result in an ArrayList.
			for (k=startingZ; k<poly.size()+startingZ; k++)
			{
				xorResult.add(PadMessage.get(k) ^ poly.get(j++));	
			}
			
			// Add the beginning 0s we skipped over into the result ArrayList
			for(int i=0; i<startingZ; i++)
				xorResult.add(0, 0);
			
			// Calculate how many filler 0s needed to add to the end of result.
			int filler = PadMessage.size() - xorResult.size();
			
			// Add in filler 0s.
			for (int i=0; i<filler; i++)
				xorResult.add(0);
			
			// Print to screen the result.
			System.out.print("Intermediate result:" + " ");
			for (int i: xorResult)
				System.out.print(i);
			System.out.println();
			
			// Remove the filler 0s.
			for (int i=0; i<filler; i++)
				xorResult.remove(xorResult.size() - 1);
			
			// Rewrite the portion of the message array used with the result of the portion from the result array.
			for (int i=0; i<xorResult.size(); i++)
				PadMessage.set(i, xorResult.get(i));
			
			// Clear the result array and reinitialize counters for next iteration.  
			xorResult.clear();
			j = 0;
			startingZ = 0;
			total = 0;
		}
		
		// Loop through and print to screen the calculate crc with the appropriate amount of leading 0s and copy the crc into an ArrayList to return. 
		j = PadMessage.size() - poly.size() + 2;
		System.out.print("CRC:" + " ");
		for (int i=0; i<PadMessage.size(); i++)
		{
			if (j !=0)
				j--;
		
			if (j == 0)
			{
				System.out.print(PadMessage.get(i));
				crc.add(PadMessage.get(i));
			}
			
		}
		System.out.println();	
		return crc;
	}
	
	// Method verify a given string by calling calcCRC() method and checking the remainder.
	public static void CRCVerifcation(String verifyToBe, boolean flag)
	{
		// Call HexToBinary() method to convert hex to binary.
		String binary = HexToBinary(verifyToBe);
		
		// Call StringToArray() method on binary string.
		ArrayList<Integer> PadMessage = StringToArray(binary);
		
		// Call calcCRC() method with verification flag marked as true.
		flag = true;
		ArrayList<Integer> crc = calcCRC(PadMessage, flag);
		
		// Check the returned array for any remainder; If none verification succeeded, otherwise verification failed.
		for (int check: crc)
		{
			if (check == 1)
			{
				System.out.println("Verification Failed!");
				return;
			}
		}
		System.out.println("Verification Succeeded!");
		return;
	}

	// Method takes a string and convert it to an ArrayList, padding with necessary 0s.
	public static ArrayList<Integer> StringToArray(String binary)
	{
		ArrayList<Integer> PadMessage = new ArrayList<Integer>();
		
		// Convert string to ArrayList
		for (char c: binary.toCharArray())
			PadMessage.add(Integer.valueOf(String.valueOf(c)));
		
		// Pad with 0s
		for (int i=0; i<poly.size()-1; i++)
			PadMessage.add(0);
		
		// Print to Screen the padded message. 
		System.out.print("Modified binary message:" + " ");
		for (int i: PadMessage)
			System.out.print(i);
		
		
		return PadMessage;
	}

}