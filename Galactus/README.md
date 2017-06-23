Galactus
=========

# Objective #

  * Implement solution for Merchant's Guide to the Galaxy problem OR a crude Natural language processing (NLP) framework.
  * Detailed problem statement
  You decided to give up on earth after the latest financial collapse left 99.99% of the earth's population with 0.01% of the wealth. Luckily, with the scant sum of money that is left in your account, you are able to afford to rent a spaceship, leave earth, and fly all over the galaxy to sell common metals and dirt (which apparently is worth a lot).
  Buying and selling over the galaxy requires you to convert numbers and units, and you decided to write a program to help you.
  The numbers used for intergalactic transactions follows similar convention to the roman numerals and you have painstakingly collected the appropriate translation between them.
  Roman numerals are based on seven symbols:
  Symbol  Value

  I   1
  V   5 
  X  10
  L  50
  C  100
  D  500
  M  1,000

  Numbers are formed by combining symbols together and adding the values. For example, MMVI is 1000 + 1000 + 5 + 1 = 2006. Generally, symbols are placed in order of value, starting with the largest values. When smaller values precede larger values, the smaller values are subtracted from the larger values, and the result is added to the total. For example MCMXLIV = 1000 + (1000 − 100) + (50 − 10) + (5 − 1) = 1944.

  The symbols "I", "X", "C", and "M" can be repeated three times in succession, but no more. (They may appear four times if the third and fourth are separated by a smaller value, such as XXXIX.) "D", "L", and "V" can never be repeated.
  "I" can be subtracted from "V" and "X" only. "X" can be subtracted from "L" and "C" only. "C" can be subtracted from "D" and "M" only. "V", "L", and "D" can never be subtracted.
  Only one small-value symbol may be subtracted from any large-value symbol.
  A number written in [16]Arabic numerals can be broken into digits. For example, 1903 is composed of 1, 9, 0, and 3. To write the Roman numeral, each of the non-zero digits should be treated separately. Inthe above example, 1,000 = M, 900 = CM, and 3 = III. Therefore, 1903 = MCMIII.

  (Source: Wikipedia ( [17]http://en.wikipedia.org/wiki/Roman_numerals)

   Input to your program consists of lines of text detailing your notes on the conversion between intergalactic units and roman numerals.

   You are expected to handle invalid queries appropriately.

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
 
