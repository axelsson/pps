Homework set 2 by Johanna Axelsson

1. The Hungry Birds Problem  (one producer - multiple consumers) (20 points)
The program will simulate a birds nest with baby birds that eat a bunch of worms that their parent gathered (random number between 1-10. One thread each is created for the parent and the babies, and they will use semaphores to synchronize when to eat and when to refill the worms. 

Usage (on ubuntu):
make birds
./birds numBirds

The argument is neccessary and will set the number of baby birds. The program will simulate their nest until cancelled. 

2. The Bear and Honeybees Problem (multiple producers - one consumer) (20 points)
This program simulates a bear that eats honey from a pot that bees gathered (full pot = 10 rounds of gathering). Threads are created for the bear and one each for the bees. The synchronization is done with semaphores. 

Usage (on ubuntu):
make honey
./honey numBees

The argument is neccessary and is the number of bees that will be collecting honey for the bear. The program simulates the situation until interrupted. 