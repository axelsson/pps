import java.util.ArrayList;

public class BirdFeedingSimulator {
	
	static int numberOfBabies = 5;	//number of producer threads
	static int feedingRounds = 10; 	//number of times the parent will get food before program closes. 
	
	/*
	 * main will create the consumer and the producers and start their threads. The program
	 * will run until the parent has fed the babies 10 rounds of worms.
	 * 
	 * FAIRNESS of solution: 
	 * The solution is initially not fair, since there is no specific order between
	 * the threads waiting for the monitor. However, the sleeping time makes it more 
	 * fair since that will make some of the "competition" disappear for a while. For
	 * a better fairness, some sort of queue should be used so that one can guarantee 
	 * that all birds eventually will get food. 
	 * */
	public static void main(String[] args) {
		{ 	
			if (args.length == 1)
				numberOfBabies = Integer.parseInt(args[0]);
			Dish cell = new Dish (); 	//monitor for controlling worms dish
			ArrayList<BabyBird> babies = new ArrayList<BabyBird>();	//producers
			for (int i = 0; i < numberOfBabies; i++) {
				babies.add(new BabyBird (cell, i));
				System.err.println("created producer");
			}
			Parent c = new Parent (cell, feedingRounds); 	//consumer
			System.err.println("created consumer");
			c.start (); //start consumer thread	
			for (BabyBird baby : babies) {
				baby.start();	//start producer threads
			}
			try { 
				c.join (); 	//join when consumer is finished
			} catch (InterruptedException e) { }; 
			for (BabyBird baby : babies) {	//stop the producers
				baby.setStop();
				baby.interrupt();
			}
		} 
	}

}
