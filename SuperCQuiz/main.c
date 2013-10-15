/* 
 * File:   main.c
 * Author: student
 *
 * Created on 21 February 2012, 11:44
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * 
 */
int main(int argc, char** argv) {
    //argument format <quizname> <question> <answer> <norepeat(Y)>
    srand(time(NULL));
    setCommandArgs(argc, argv);
    if(argc > 1)
    {
        if(strcmp(argv[1], "listquiz") == 0)
        {
            printQuizFiles();
            freeMemory();
            return (EXIT_SUCCESS);
        }
    } 
    selectQuizFile();
    freeMemory();
    return (EXIT_SUCCESS);
}

