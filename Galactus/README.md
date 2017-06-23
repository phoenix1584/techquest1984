Galactus
=========

# Objective #

  * Implement solution for a crude Natural language processing (NLP) framework.

# Dependencies #
 * OS : Linux
 * arch : 64 bit
 * Compiler : gcc 4.8.3 or higher (using c++11 flag)
 * boost 1.61
   Only source code needed. Necessary boost shared library files being included in extern/lib folder. 
   URL : https://sourceforge.net/projects/boost/files/boost/1.61.0/boost_1_61_0.tar.bz2/download 

# Build #
 * Update the variable BOOST_INC_PATH in file paths.env with absolute path of boost source code directory. 
   Example : BOOST_INC_PATH = /home/user/codebase/boost_1_61_0
 * To compile source and test cases 
    $ make 
 * To clean the code base
    $ make clean

# Test Case Usage #

# Design Notes : Components of System #

## Approach ##
 + Trying to implement a natural language toolkit that accepts queries and trains
   itself. 
 + Has a basic value system that dictates the validity of inputs and ensure the 
   consistency of data set incrementally.

## Basic Number System ##
  - Comprises of set of fixed symbols
  - Each symbol has predefined constant value.
  - Basic arithmetic rules for value translation
  - Unacceptable patterns
 
## Query Subsystem ##
  - Assignment queries to assign alternative token to predefined constant value.
  - Assignment query to define a translation between tokens of two numeric systems.
  - Computational query to convert tokens into value.
  - Computational query to translate value between two number systems.
  - Queries need to be separated based on presence of pivot word and terminator.
    Query Syntax samples
     * NUM_SYS_2 IS NUM_SYS_1
     * NUM_SYS_2 OBJ_SYS_1 IS VALUE_SYS_1
     * COMP_QUERY OPERATOR NUM_SYS_X TERMINATOR
       
 
## Assumptions ##

 + Roman system is constant as created at compile time.
 + 
  
## TODO ##

* Handling of duplicate pivot words like IS in the queries.
* Accept the base number system as input file. Challenge here is communicating the rules like arithmetics.


# Known Issues #


# References #

Third party frameworks

* Python NLTK : http://www.nltk.org/
 
