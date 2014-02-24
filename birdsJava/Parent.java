/*parent is the producer that will put in more worms for the babies until a given number
 * of rounds has been served.*/
public class Parent extends Thread{
	private Dish cell; 
	private int feedingRounds; 
	
	public Parent (Dish cell, int rounds) { 
		this.cell = cell; 
		this.feedingRounds = rounds; 
	} 
	
	public void run () { 
		for (int i = 0; i < feedingRounds; i++) { 
			cell.put (); 
		} 
	} 

}
