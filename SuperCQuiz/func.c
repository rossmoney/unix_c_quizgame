#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char quizname[50] = {0};
char ** quizFiles = NULL;
char ** questions = NULL;
char ** answers = NULL;
char * intbuffer = NULL;
char ** commandLineArgs = NULL;
int * correctAnswers = NULL;
int numOfAnswers = 4;
int numOfQuizzes = 0;
int maxQuizFiles = 20;
int numOfQuestions = 0;
int selQSNum = 0, selectedAnswer = 0;
int questionCharLength = 500;
int answerCharLength = 100;

int i = 0, i2 = 0, correct = 0, incorrect = 0;
int incorrectQuestions[100] = {0};
int incorrectAnswers[100] = {0};
int correctQuestions[100] = {0};
int numOfArgs = 0;
char quizextension[] = ".quiz";

void freeMemory();
void selectQuestion();
void answerNew();
void enterAnswer();
void addUpPoints();
void selectQuizFile();

void setCommandArgs(int argc, char ** argv) {
    commandLineArgs = argv;
    numOfArgs = argc;
    intbuffer = (char*) malloc(sizeof (int));
}

void freeMemory() {
    int i = 0;
    if (questions != NULL) {
        for (i = 0; i < numOfQuestions; i++) {
            if (questions[i] != NULL) free(questions[i]);
        }
        free(questions);
    }
    if (answers != NULL) {
        for (i = 0; i < numOfAnswers; i++) {
            if (answers[i] != NULL) free(answers[i]);
        }
        free(answers);
    }
    if (quizFiles != NULL) {
        for (i = 0; i < numOfQuizzes; i++) {
            if (quizFiles[i] != NULL) free(quizFiles[i]);
        }
        free(quizFiles);
    }
    free(correctAnswers);
    free(intbuffer);
    //if(commandLineArgs != NULL) free(commandLineArgs);
}

void printReport() {
    printf("%i correct answers, %i incorrect answers. Your score: %i/%i\n", correct, incorrect, correct, (correct + incorrect));
    printf("Correct: \n");
    FILE *sortfile;
    sortfile = fopen("correct.txt", "w");
    for (i = 0; i < correct; i++) {
        fprintf(sortfile, "%i\n", (int) correctQuestions[i]);
    }
    fclose(sortfile);

    char pmode[] = "r"; // Read mode
    char op[256]; // Placeholder for results
    int ret;
    i = 0;
    FILE *fp = popen("sort correct.txt", pmode); // File stream id
    ret = fscanf(fp, "%s", op); // Read pipe
    while (ret != EOF) { // If an EOF is encountered, then quit
        correctQuestions[i] = atoi(op);
        ret = fscanf(fp, "%s", op); // Read stdout
        i++;
    }
    pclose(fp); // Close - this blocks if the child is still running

    for (i = 0; i < correct; i++) {
        printf("%i)%s : Your Answer: %s\n", correctQuestions[i], questions[correctQuestions[i] - 1],
                answers[((correctQuestions[i] - 1) * numOfAnswers) + correctAnswers[correctQuestions[i] - 1] - 1]);
    }
    printf("Incorrect: \n");

    sortfile = fopen("incorrect.txt", "w");
    for (i = 0; i < incorrect; i++) {
        fprintf(sortfile, "%i\n", incorrectQuestions[i]);
    }
    fclose(sortfile);

    i = 0;
    fp = popen("sort incorrect.txt", pmode); // File stream id
    ret = fscanf(fp, "%s", op); // Read pipe
    while (ret != EOF) { // If an EOF is encountered, then quit
        incorrectQuestions[i] = atoi(op);
        ret = fscanf(fp, "%s", op); // Read stdout
        i++;
    }
    pclose(fp); // Close - this blocks if the child is still running

    for (i = 0; i < incorrect; i++) {
        printf("%i)%s Your Answer: %s\n", incorrectQuestions[i], questions[incorrectQuestions[i] - 1],
                answers[((incorrectQuestions[i] - 1) * numOfAnswers) + incorrectAnswers[i] - 1]);
    }
}

void selectQuestion() {
    strcpy(intbuffer, "");
    if (numOfArgs > 2) {
        strcpy(intbuffer, commandLineArgs[2]);
        if (strcmp(intbuffer, "") == 0) {
            return;
        }
    }
    if (numOfArgs == 1) {
        printf("Enter question number to display (1 - %i) or R for a random question:", numOfQuestions);
        scanf("%s", intbuffer);
    }
    if (strncmp(intbuffer, "R", strlen(intbuffer)) == 0) {
        selQSNum = rand() % numOfQuestions + 1;
    } else {
        if (!isdigit(intbuffer[0])) {
            if (numOfArgs < 3) {
                printf("Input is not a number or R!");
                selectQuestion();
            }
            return;
        } else {
            selQSNum = atoi(intbuffer);
            if (selQSNum > numOfQuestions || selQSNum < 1) {
                if (numOfArgs < 3) {
                    printf("There are only %i questions! Enter a number between 1 and %i.\n", numOfQuestions, numOfQuestions);
                    selectQuestion();
                }
                return;
            }
        }
    }
    printf("Your Question is: %i)%s\n", selQSNum, questions[selQSNum - 1]);
    printf("Choose the Correct Answer:\n");
    for (i = 0; i < numOfAnswers; i++) {
        printf("%i) %s\n", (i + 1), answers[((selQSNum - 1) * numOfAnswers) + i]);
    }
    enterAnswer();
}

void answerNew() {
    char response[1];
    strcpy(response, "");
    if (numOfArgs > 4) {
        strcpy(response, commandLineArgs[4]);
        if (strcmp(response, "") == 0) {
            return;
        }
    }
    if (numOfArgs == 1) {
        printf("Do you want to answer another question? Y or N: ");
        scanf("%s", response);
    }
    if (strncmp(response, "Y", strlen(response)) == 0) {
        selectQuestion();
        return;
    } else {
        printReport();
    }
}

void enterAnswer() {
    strcpy(intbuffer, "");
    if (numOfArgs > 3) {
        strcpy(intbuffer, commandLineArgs[3]);
        if (strcmp(intbuffer, "") == 0) {
            return;
        }
    }
    if (numOfArgs == 1) {
        printf("Enter your choice:");
        scanf("%s", intbuffer);
    }
    if (!isdigit(intbuffer[0])) {
        if (numOfArgs < 4) {
            printf("\nInvalid Answer Number!\n");
            enterAnswer();
        }
        return;
    }
    selectedAnswer = atoi(intbuffer);
    if (selectedAnswer > numOfAnswers || selectedAnswer < 1) {
        if (numOfArgs < 4) {
            printf("\nInvalid Answer Number!\n");
            enterAnswer();
        }
        return;
    }
    addUpPoints();
}

void addUpPoints() {
    if (selectedAnswer == correctAnswers[selQSNum - 1]) {
        printf("Well done you were correct!\n");
        correctQuestions[correct] = selQSNum;
        correct++;
    } else {
        printf("Incorrect this time, sorry!\n");
        incorrectQuestions[incorrect] = selQSNum;
        incorrectAnswers[incorrect] = selectedAnswer;
        incorrect++;
    }
    answerNew();
}

void substring(int start, int stop, char *text) {
    sprintf(text, "%.*s", stop - start, &text[start]);
}

void printQuizFiles() {
    char pmode[] = "r"; // Read mode
    char op[256]; // Placeholder for results
    int ret;
    quizFiles = malloc(maxQuizFiles * sizeof (int));
    for (i = 0; i < maxQuizFiles; i++) {
        quizFiles[i] = malloc(100 * sizeof (char *));
    }
    FILE * fp = popen("ls", pmode); // File stream id
    ret = fscanf(fp, "%s", op); // Read pipe
    numOfQuizzes = 0;
    while (ret != EOF) { // If an EOF is encountered, then quit
        char cmpbuffer[strlen(op)];
        strcpy(cmpbuffer, op);
        substring(strlen(op) - 5, strlen(op), cmpbuffer);
        //printf("cmpbuffer: '%s', op:'%s'\n", cmpbuffer, op);
        if (strcmp(cmpbuffer, quizextension) == 0) {
            numOfQuizzes++;
            strcpy(quizFiles[numOfQuizzes], op);
            printf("%i) %s\n", numOfQuizzes, op);
        }
        ret = fscanf(fp, "%s", op); // Read stdout
        i++;
    }
    pclose(fp);
    if (numOfQuizzes == 0) printf("No quiz files to load!\n");

}

void selectQuizFile() {

    i = 0;
    strcpy(quizname, "");
    if (numOfArgs > 1) {
        strcpy(quizname, commandLineArgs[1]);
        if (strcmp(quizname, "") == 0) {
            return;
        }
    }
    if (numOfArgs == 1) {
        printQuizFiles();
        if (numOfQuizzes == 0) return;
        printf("Enter number of quiz to start: ");
        scanf("%s", intbuffer);

        if (!isdigit(intbuffer[0])) {
            printf("\nInvalid Quiz Number!\n");
            selectQuizFile();
            return;
        }
        int selectedQuizFile = atoi(intbuffer);
        if (selectedQuizFile < 1 || selectedQuizFile > numOfQuizzes) {
            printf("\nQuiz Number Out of Range (max is %i)!\n", numOfQuizzes);
            selectQuizFile();
            return;
        } else {
            strcpy(quizname, quizFiles[atoi(intbuffer)]);
        }

    }
    printf("You selected file %s!\n", quizname);

    numOfQuestions = readInQuizFile(quizname);
    if (numOfQuestions == 0) {
        if (numOfArgs < 2) {
            selectQuizFile();
        }
        return;
    }

    selectQuestion();

    //Code to check contents of arrays.
    /*for (i = 0; i < numOfQuestions; i++) {
        printf("question array --> %s\n", questions[i]);
        for (i2 = 0; i2 < numOfAnswers; i2++) {
            printf("\tanswer array --> %s\n", answers[(i * numOfAnswers) + i2]);
        }
        printf("correct answer --> %i\n", correctAnswers[i]);
    }*/
}

int readInQuizFile(char * filename) {

    FILE *quizfile;
    quizfile = fopen(filename, "r");

    int i = 0, i2 = 0, ret;
    char * p;
    char intBuffer[sizeof (int) + 1];

    if (quizfile != NULL) {
        fgets(intBuffer, sizeof (intBuffer), quizfile);
        if ((p = strchr(intBuffer, '\n')) != NULL) *p = '\0';
        numOfQuestions = atoi(intBuffer);
        printf("%i Questions In File\n", numOfQuestions);

        questions = malloc(numOfQuestions * sizeof (int));
        for (i = 0; i < numOfQuestions; i++) {
            questions[i] = malloc(questionCharLength * sizeof (char *));
        }
        answers = malloc(numOfQuestions * numOfAnswers * sizeof (int));
        for (i = 0; i < (numOfQuestions * numOfAnswers); i++) {
            answers[i] = malloc(answerCharLength * sizeof (char *));
        }
        correctAnswers = malloc(numOfQuestions * sizeof (int));

        char questionLine[questionCharLength];
        char answerLine[answerCharLength];

        for (i = 0; i < numOfQuestions; i++) {
            fgets(questionLine, sizeof (questionLine), quizfile);
            if ((p = strchr(questionLine, '\n')) != NULL) *p = '\0';
            strcpy(questions[i], questionLine);
            for (i2 = 0; i2 < numOfAnswers; i2++) {
                fgets(answerLine, sizeof (answerLine), quizfile);
                if ((p = strchr(answerLine, '\n')) != NULL) *p = '\0';
                strcpy(answers[(i * numOfAnswers) + i2], answerLine);
            }
            fgets(intBuffer, sizeof (intBuffer), quizfile);
            if ((p = strchr(intBuffer, '\n')) != NULL) *p = '\0';
            correctAnswers[i] = atoi(intBuffer);
        }
        fclose(quizfile);

    } else {
        perror(filename);
    }

    return numOfQuestions;
}