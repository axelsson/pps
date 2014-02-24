import java.util.Random;

/*bee is a producer that will try to add more honey to the pot, which takes some time*/
public class Bee extends Thread{
	private HoneyPot cell; 
	private int id;
	private boolean Stop = false; 
	public Bee (HoneyPot cell, int id) { 
		this.cell = cell; 
		this.id = id;
	} 
	public void setStop () { 
		Stop = true; 
	} 
	public void run () { 
		Random r = new Random();
		while (!Stop) { 
			try { 
				sleep (1000*(r.nextInt(3)+1)); //gathering takes some time
			} catch (InterruptedException e) { } 
			cell.put (id);
		} 
	}
}
