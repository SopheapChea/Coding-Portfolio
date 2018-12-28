// Sopheap Sok
// 3/2/2018
// Program: Camera
// Program determines the minimum number of pictures needed to capture all the objects present by using 
// a greedy algorithm.


import java.util.*;

public class camera
{
	public static int totalPic = 0;
	ArrayList<range> plan;
	
	// Constructor that create camera object, plan, as an ArrayList of range.
	public camera()
	{
		plan = new ArrayList<range>();
	}
	
	public static void main(String[] args)
	{
		Scanner input = new Scanner(System.in);
		int numObj = input.nextInt();
		int d = input.nextInt();
		int start = input.nextInt();
		int end = input.nextInt();
		int objectBefore = 0;
		ArrayList<Double> projectionCoords = new ArrayList<Double>();

		// Process all objects.
		for (int i=0; i<numObj; i++)
		{
			int x = input.nextInt();
			int y = input.nextInt();
			
			// Check how many objects are in front of the wall. If objects are in front, no calculations
			// are needed and projection () method is not called.
			if (y < d)
			{
				objectBefore++;
				continue;
			}
			
			// Object is behind the wall, send the coordinates, and the start and end point of the Glass Partition
			// to projection() method to calculate projection range and add the return values to the ArrayList projectionCoords.
			projectionCoords.addAll(projection(x, y, d, start, end));	
		}
		
		// If there are only one object or if all the objects are in front of the wall, one picture is taken only.
		if(numObj == 1 || numObj == objectBefore )
		{
			totalPic = 1;
			System.out.println(totalPic);
			return;
		}
		
		// Subtract the number of object in front of the wall to total number of objects.
		numObj -= objectBefore;
		
		// Call greedy() method
		greedy(projectionCoords, numObj);
		
		// Print out number of pictures needed to incorporate all objects.
		System.out.println(totalPic);
		
	}
	
	// Method calculates the projection of each object on the x axis.
	public static ArrayList <Double> projection(int x, int y, int d, int start, int end)
	{
		double y2 = d;
		double x2 = start;
		double m, b, projection;
		ArrayList<Double> projectionCoords = new ArrayList<Double>();
		
		// Each object has a range where its picture can be taken, so two x point is calculate for each.
		for(int i=0; i<2; i++)
		{
			// If the object does not makes a vertical line with either the beginning or end of the Glass Partition
			// the slop is calculate.
			if (x != x2)
			{
				m = (y2-y)/(x2-x);
				b = y-(m*x);

				projection = (0.0-b)/m;

				projectionCoords.add(projection);
				x2 = end;
			}
			
			// Object makes a vertical line, just input the x into projectionCoords array as start or ending point.
			else
			{
				projection = x;
				projectionCoords.add(projection);

				x2 = end;
				
			}
			
		}
		
		return projectionCoords;
	}
	
	// Method takes the projections ArrayList of each object and determine at what point do the most object
	// overlapped at.
	public static void greedy(ArrayList <Double> projectionCoords, int numObj)
	{
		double x1, x2;
		int k = 0, l = 0, data = 0;
		
		// Create a new ArrayList and copy over all the projections. 
		ArrayList <Double> sortedList = new ArrayList<Double> ();
		sortedList.addAll(projectionCoords);
		
		
		//New ArrayList is added to a HashSet to delete duplicated values and copy back to the ArrayList.
		Set<Double> Hset = new HashSet<Double> ();
		Hset.addAll(sortedList);
		sortedList.clear();
		sortedList.addAll(Hset);
		Collections.sort(sortedList);
		
		// Create an array of ArrayLists.
		camera[] points = new camera[sortedList.size() + 1];
		for (int i=0; i<=sortedList.size(); i++)
			points[i] = new camera();
	
		// Loop through each value stored in the sortedList array and determine whether 
		// the value falls between the range of the object.
		for(int i=0; i<sortedList.size(); i++)
		{
			double val = sortedList.get(i);
			
			
			for(int j=0; j<projectionCoords.size();)
			{
				x1 = projectionCoords.get(j++);
				x2 = projectionCoords.get(j++);
				
				// The value is in the range of the object, make the projection points into an object and 
				// add to the array of ArrayList.
				if (val >= x1 && val <= x2)
				{
					range object = new range(x1, x2);
					
					points[k].plan.add(object);
				}
				
			}
			k++;
			
		}
		
		// Create an array of object range to store the index of each ArrayList and the number of object in the list.
		range [] size = new range[sortedList.size()];
		
		// Create an array of object range to store objects already captured in a picture.
		range[] used = new range [sortedList.size()];
		
		// Loop through and store the index of each ArrayList and its size in an object array.
		for(int loop=0; loop<sortedList.size(); loop++)
		{
			for(int i=0; i<sortedList.size(); i++)
			{
				int sizeHere = points[i].plan.size();
				
				// Store the index and size of each ArrayList.
				size[i]= new range(i, sizeHere);
				
			}
			
			// Sort the object array by size or number of object in each ArrayList.
			Arrays.sort(size);
			
			// Index 0 of the object array will have the max number of object at a given point.
			// The index is used to reference the array of ArrayList to grab the projection of the object.
			// The projection is then added to a used array and data is incremented to keep track of how many 
			// object is in the array. A picture is taken every time we reference index 0 of the sorted size array. 
			for(int j=0; j<points[size[0].index].plan.size(); j++)
			{
				range usedObject = points[size[0].index].plan.get(j);
				used[l++] = usedObject;
				data++;
			}
			totalPic++;
			
			// Loop through the array of ArrayList and remove all object already captured.
			for(int i=0; i<sortedList.size(); i++)
			{
				for(int j=0; j<points[i].plan.size(); j++)
				{
					range object = points[i].plan.get(j);
					
					if (contains(used, object, data))
					{
						points[i].plan.remove(j);
						
					}
				}
			}
			
			// When the number of object in the used array match that of the total object behind the wall
			// the loop is broken and the method return back to main.
			if (data >= numObj)
				break;

		}
		return;
	}
	
	// Simple method to determine with the projection is already in the used array.
	public static boolean contains(range[] used, range usedObject, int data)
	{
		for (int i=0; i<data; i++)
		{
			range n = used[i];
			if (n.x1 == usedObject.x1 && n.x2 == usedObject.x2)
				return true;
			
		}
		return false;
	} 
	
}

// Class that creates two different range objects and implements comparable.
class range implements Comparable<range>
{
	public double x1, x2;
	public int index, size;
	
	// Projection constructor.
	public range(double x1, double x2)
	{
		this.x1 = x1;
		this.x2 = x2;
	}
	
	// Index and size constructor.
	public range(int index, int size)
	{
		this.index = index;
		this.size = size;
	}
	
	// Compare by size and return the largest.
	public int compareTo(range other)
	{
		return other.size - this.size;
	}
	
}



