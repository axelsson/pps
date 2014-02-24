/*bear is a consumer that will take honey until it has gotten 10 pots. */
public class Bear extends Thread{
	private HoneyPot cell; 
	private int n; 
	public Bear (HoneyPot cell, int n) { 
		this.cell = cell; 
		this.n = n; 
	} 
	public void run () { 
		for (int i = 0; i < n; i++) { 
			cell.take (); 
		} 
	} 

}
