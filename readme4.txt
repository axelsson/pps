Homework set 4 by Johanna Axelsson

1. The Hungry Birds Problem  (one producer - multiple consumers) (20 points)
The program will simulate a birds nest with baby birds that eat a bunch of worms
 that their parent gathered. One thread each is created for the parent and the 
 babies, and they will use a monitor to synchronize when to eat and when to 
 gather more worms. 

Usage:
javac BirdFeedingSimulator.java
java BirdFeedingSimulator numberOfBabies

The argument is optional and will set the number of baby birds. Default number is 5. 
The program will run until the parent has gathered 10 rounds of worms. 

2. The Bear and Honeybees Problem (multiple producers - one consumer) (20 points)
This program simulates a bear that eats honey from a pot that bees gathered 
(full pot = 10 rounds of gathering). Threads are created for the bear and one 
each for the bees. The synchronization is done with a monitor. 

Usage:
javac HoneyPotSimulator.java
java HoneyPotSimulator numberOfBees

The argument is optional and is the number of bees that will be collecting honey for
 the bear. Default is set to 5. The program will run until the bear has eaten 10 pots
 of honey. 