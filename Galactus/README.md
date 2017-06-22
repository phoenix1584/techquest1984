Galactus
=========

1. Objective

2. Dependencies
 * OS : Linux
 * arch : 64 bit
 * Compiler : gcc 4.4 or higher
 * boost 1.61
   Only source code needed, since so files being included in extern/lib folder. 
   URL : https://sourceforge.net/projects/boost/files/boost/1.61.0/boost_1_61_0.tar.bz2/download 

3. Build 
 * Update the variable BOOST_INC_PATH with absolute path of boost source code directory. 
 * To compile source and test cases 
    $ make 
 * To clean the code base
    $ make clean

4. Design Notes

h1. Components of System

* Basic Number System
  - Comprises of set of fixed symbols
  - Each symbol has predefined constant value.
  - Basic arithmatic rules for value translation
  - Unacceptable patterns
 
 * Query Subsystem
  - Assignment queries to assign alternative token to predefined constant value.
  - Assignment query to define a translation between tokens of two numeric systems.
  - Computational query to convert tokens into value.
  - Computational query to translate value between two number systems.

5. Test Case Usage

6. References
 
