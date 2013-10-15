unix_c_quizgame
===============

Demonstrating my C capability, another uni project.

Super C Quiz v1.0 README
========================

This utility runs in the form of a quiz, questions are read in from a pre-made quiz file 
and prompts come up on screen to select a particular question or a random selection.

You then progress through by selecting an answer and getting the oppurtunity to answer another 
question, at which point you can answer another one and it loops back around, or you can cancel
and get a report of how you did with the answers you selected.

The utility can be run headless, i.e without prompts to read the output into a gui application.


Known Issues/Dependencies
=========================

Dependent on the sort command to sort the questions by number ascending for the report.
Quiz file must be in the same directory as the program, or the 'current working directory'.
Directory the program is run in must be writable for temporary files to be created to send to the sort command.
