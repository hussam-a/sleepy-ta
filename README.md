# sleepy-ta
Simulation of the Sleepy TA problem (AKA Sleeping Barber problem)

# Compiling and Running

- To compile
  * gcc -pthread main.c Ta.c Utility.c Student.c -o [OutputFileName]
 
- To run
  * ./[OutputFileName] Arg1
  * Arg1 = the number of students.
  * If Arg1 is not put in, default is 6 students.
  * Chairs are fixed to 3.
  * If more arguments put in, program will fail intentionally notifying the user to check the arguments.
