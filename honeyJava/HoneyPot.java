/*monitor for controlling access between the producers and the consumer of the honeypot*/
public class HoneyPot {
	private int value = 0; 	//amount of stored honey
	private boolean full = false;
	
	/*take will wait while the pot is not full and then empty it when signaled. It will
	 * thereafter signal all other threads waiting to access the monitor.*/
	public synchronized void take() { 
		while (!full) { 
			try { 
				wait (); 
			} catch (InterruptedException e) { } 
		} 
		full = false;
		value = 0;
		System.out.println ("Bear ate honey."); 
		notifyAll (); 
	} 
	/*put will wait while the pot is full, and then begin to produce more honey unless 
	 * the put becomes full, by then it will signal the waiting thread (the consumer).*/
	public synchronized void put(int id) { 
		while (full) {
			try {
				wait (); 
			} catch (InterruptedException e) { } 
		}
		if (this.value == 10){
			full = true; 
			System.err.println("Bee "+id + " notified bear.");
			notify();
		}
		else{
			this.value += 1; 
			System.err.println("Bee "+id+" made honey, "+value+" portions in pot");
		}
	} 
}
