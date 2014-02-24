import java.util.Random;

/* babybird is a consumer that will try to take worms when avaliable, and sleep afterwards.*/
public class BabyBird extends Thread{
	private Dish cell; 
	private int id;
	private boolean Stop = false; 
	public BabyBird (Dish cell, int id) { 
		this.cell = cell; 
		this.id = id;
	} 
	public void setStop () { 
		Stop = true; 
	} 
	public void run () { 
		Random r = new Random();
		while (!Stop) { 
			cell.take (id); 
			try { 
				sleep (1000*(r.nextInt(5)+1)); 
			} catch (InterruptedException e) { } 
		} 
	}
}
