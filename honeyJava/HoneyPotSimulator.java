import java.util.ArrayList;

public class HoneyPotSimulator {
	
	static int numberOfBees = 5;	//number of producer threads
	static int potsForBear = 10; 	//number of pots the bear will eat before program closes. 
	
	/*
	 * main will create the consumer and the producers and start their threads. The program
	 * will run until the bear has eaten a number of honeypots.
	 * 
	 * FAIRNESS of solution: Same as for birds problem, solution isn't fair due to 
	 * lack of queue, but added a "gathering time" for the bees which will make it 
	 * more fair. 
	 */
	public static void main(String[] args) {
		{ 	
			if (args.length == 1)
				numberOfBees = Integer.parseInt(args[0]);
			HoneyPot cell = new HoneyPot (); 	//monitor for controlling honey
			ArrayList<Bee> bees = new ArrayList<Bee>();	//producers
			for (int i = 0; i < numberOfBees; i++) {
				bees.add(new Bee (cell, i));
				System.err.println("created producer");
			}
			Bear c = new Bear (cell, potsForBear); 	//consumer
			System.err.println("created consumer");
			c.start (); //start consumer thread	
			for (Bee bee : bees) {
				bee.start();	//start producer threads
			}
			try { 
				c.join (); 	//join when consumer is finished
			} catch (InterruptedException e) { }; 
			for (Bee bee : bees) {	//stop the producers
				bee.setStop();
				bee.interrupt();
			}
		} 
	}

}
