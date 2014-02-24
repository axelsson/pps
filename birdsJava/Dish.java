
/*monitor for controlling the value (worms) between a producer and several consumers*/
public class Dish {
	private int value = 10; 	//initial value for the number of worms
	private boolean empty = false;
	
	/* put will make the thread wait if the dish isn't full, else
	 * it will make it full and notify the other threads waiting.*/
	public synchronized void put() { 
		while (!empty) { 
			try { 
				wait (); 
			} catch (InterruptedException e) { } 
		} 
		empty = false;
		value = 10;
		System.out.println ("Parent collected worms."); 
		notifyAll (); 
	} 
	/*take will wait while the dish is empty and will otherwise check if the dish
	 * actually is empty, and if so notify (so the one waiting for put can refill)
	 * else it will just decrement the value. */
	public synchronized void take(int id) { 
		while (empty) {
			try {
				wait (); 
			} catch (InterruptedException e) { } 
		}
		if (this.value == 0){
			empty = true; 
			System.out.println("Baby bird "+id + " called for parent.");
			notify();
		}
		else{
			this.value -= 1; 
			System.err.println("Baby bird "+id+" ate a worm, "+value+" worms left!");
		}
	} 
}
