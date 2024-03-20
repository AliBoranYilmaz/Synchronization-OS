# Synchronization-OS
A school project I did about thread synchronization in the operating systems course

Problem:
Write a multithreaded programming code that calculates the sum of the numbers in each column in
the matrix given in the input file and then sorts these calculated values and prints to the screen. A
different thread for each column must perform these calculations. Each thread will calculate the sum
of the numbers in each column in the matrix and write the column sum into a shared common array.
After all the column sums are added, a final thread will sort these array elements and print them to
the screen.

Mutex is used for synchronization.
