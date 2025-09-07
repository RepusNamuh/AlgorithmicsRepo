/* Program to implement Australian House of Representatives preferential
voting and determine election outcomes from vote preference orderings.
Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
August 2025, with the intention that it be modified by students
to add functionality, as required by the assignment specification.
All included code is (c) Copyright University of Melbourne, 2025
Student Authorship Declaration:
(1) I certify that except for the code provided in the initial skeleton
file, the program contained in this submission is completely my own
individual work, except where explicitly noted by further comments that
provide details otherwise. I understand that work that has been
developed by another student, or by me in collaboration with other
students, or by non-students as a result of request, solicitation, or
payment, may not be submitted for assessment in this subject. I
understand that submitting for assessment work developed by or in
collaboration with other students or non-students constitutes Academic
Misconduct, and may be penalized by mark deductions, or by other
penalties determined via the University of Melbourne Academic Honesty
Policy, as described at https://academicintegrity.unimelb.edu.au.
(2) I also certify that I have not provided a copy of this work in either
softcopy or hardcopy or any other form to any other student, and nor will
I do so until after the marks are released. I understand that providing
my work to other students, regardless of my intention or any undertakings
made to me by that other student, is also Academic Misconduct.
(3) I further understand that providing a copy of the assignment
specification to any form of code authoring or assignment tutoring
service, or drawing the attention of others to such services and code
that may have been made available via such a service, may be regarded as
Student General Misconduct (interfering with the teaching activities of
the University and/or inciting others to commit Academic Misconduct). I
understand that an allegation of Student General Misconduct may arise
regardless of whether or not I personally make use of such solutions or
sought benefit from such actions.
Signed by: Cong Le
Dated: 03/09/2025
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>

#define NAMEMAX 20
#define MAXCANDIDATES 20 
#define MAXVOTES 1000
#define TRUE 1
#define FALSE 0
#define FIRSTROUND 1
#define FIRSTPREF 1
#define STAGETWO 2
#define STAGETHREE 3
#define NOTFOUND -1
#define MAJORITY 50.00

typedef char name_t[NAMEMAX + 1];

typedef struct {
    name_t name;
    int votes[MAXVOTES];
    int firstPrefCount; 
    int originalCount; // number of votes in favour of this candidate
    int eliminated;
} candidate_t;

typedef struct {
    int totalCandidates;
    int remainingCandidates;
    int totalVotes;
} pollStatus_t;

///////////////////////////////////////////////////////////////////////

int getword(char W[], int limit);

int readVotes(candidate_t votingPool[], pollStatus_t *stat, int limit);

void stageOne(candidate_t votingPool[], pollStatus_t stat);

void stageTwo(candidate_t votingPool[], pollStatus_t stat, int stage);

void redistributeVotes(candidate_t votingPool[], pollStatus_t stat, 
                                                        int elimIndex);

int elimination(candidate_t votingPool[], pollStatus_t stat, int *winner);

void statusPrintOut(candidate_t votingPool[], int elimIndex, int *elected, 
                    int *winner, pollStatus_t stat);

void insertionSort(candidate_t votingPool[], int noCandidates);
void shiftLeft(candidate_t votingPool[], int start, int *end);
void reset(candidate_t votingPool[], pollStatus_t *stat);
///////////////////////////////////////////////////////////////////////

// main program provides traffic control
int
main(int argc, char *argv[]) {
    // initialising variable and arrays
    candidate_t votingPool[MAXCANDIDATES], dummy;
    pollStatus_t stat;
    stat.totalCandidates = 0;
    stat.totalVotes = 0;

    //Reading the input
    scanf("%d", &stat.totalCandidates);
    if (stat.totalCandidates > MAXCANDIDATES) {
        printf("Too many candidates");
        exit(EXIT_FAILURE);
    }

    // Getting the names of candidates and initialising
    // votes count and elimination status to 0
    int i = 0;
    while (i < stat.totalCandidates) {
        getword(dummy.name, NAMEMAX);
        strcpy(votingPool[i].name, dummy.name);
        votingPool[i].firstPrefCount = 0;
        votingPool[i].eliminated = FALSE;
        i++;
    }

    // Getting the votes for each candidate
    readVotes(votingPool, &stat, MAXVOTES);

    // Outputting stage one results
    printf("\n");
    stageOne(votingPool, stat);

    // Stage two and three of the preferential voting system
    stageTwo(votingPool, stat, STAGETWO);
    printf("\n");
    reset(votingPool, &stat);
    stageTwo(votingPool, stat, STAGETHREE);

    // all done, time to go home
    printf("tadaa!\n");
    fflush(stdout);
    return 0;
}

///////////////////////////////////////////////////////////////////////

// extract a single word out of the standard input, of not
// more than limit characters, argument array W must be
// limit+1 characters or bigger.
// function adapted from Figure 7.13 of Programming, Problem Solving,
// and Abstraction with C, by Alistair Moffat
//
int
getword(char W[], int limit) {
    int c, len=0;
    /* first, skip over any non alphabetics */
    while ((c=getchar())!=EOF && !isalpha(c)) {
        /* do nothing more */
    }
    if (c==EOF || c== '\n') {
        return EOF;
    }
    /* ok, first character of next word has been found */
    W[len++] = c;
    while (len<limit && (c=getchar())!=EOF && isalpha(c)) {
        /* another character to be stored */
        W[len++] = c;
    }
    /* now close off the string */
    W[len] = '\0';
    return 0;
}

// Reading the votes until end of file
// For each voter read in their preferences for each candidate.
int 
readVotes(candidate_t votingPool[], pollStatus_t *stat, int limit) {
    int c, i = 0, preference;
    int *totalVotes = &stat->totalVotes;
    int *totalCandidates = &stat->totalCandidates;

    while ((c=scanf("%d", &preference)) == 1 && *totalVotes < limit) {
        // new line indicates a new voter
        if (i >= *totalCandidates) {
            i = 0;
            (*totalVotes)++;
        }

        votingPool[i].votes[*totalVotes] = preference;
        // if the preference for the current candidate is 
        // 1 (first preference), increase their vote count.
        if (preference == FIRSTPREF) {
            votingPool[i].firstPrefCount++;
            votingPool[i].originalCount++;
        }
        i++;
    }
    (*totalVotes)++;
    return 0;
}

void stageOne(candidate_t votingPool[], pollStatus_t stat) {
    // Stage one of the preferential voting system
    printf("Stage 1\n");
    printf("=======\n");
    printf("read %d candidates and %d votes\n", 
            stat.totalCandidates, stat.totalVotes);
    printf("voter %d preferences...\n", stat.totalVotes);

    name_t tempArray[stat.totalCandidates];
    // Linear Reordering of candidates based on 
    // the last voter's preferences.
    for (int i = 0; i < stat.totalCandidates; i++) {
        int index = votingPool[i].votes[stat.totalVotes - 1];
        strcpy(tempArray[index - 1], votingPool[i].name);
    }

    // Print out the last voter's preferences
    for (int i = 0; i < stat.totalCandidates; i++) {
        printf("    rank %2d: %s\n", i + 1, tempArray[i]);
    }

    printf("\n");
}

// Elimination stage by removing candidate with lowest winning votes
// each round and redistribute the votes.
void stageTwo(candidate_t votingPool[], pollStatus_t stat, int stage) {

    printf("Stage %d\n", stage);
    printf("=======\n");
    int elected = FALSE, round = FIRSTROUND;
    stat.remainingCandidates = stat.totalCandidates;

    while (!elected) {
        printf("round %d...\n", round);

        // Move eliminated candidate to the end of the array
        
        if (stage == STAGETHREE) {
            // Insertion Sort, sorting from highest preference to lowest
            insertionSort(votingPool, stat.remainingCandidates);
        }
        
        int winner = NOTFOUND;
        int elimIndex = elimination(votingPool, stat, &winner);

        statusPrintOut(votingPool, elimIndex, &elected, &winner, stat);

        shiftLeft(votingPool, elimIndex, &stat.remainingCandidates);
        
        redistributeVotes(votingPool, stat, elimIndex);
        round++;
    }
}

void shiftLeft(candidate_t votingPool[], int start, int *end) {
    candidate_t temp = votingPool[start];
    for (int i = start; i < *end - 1; i++) {
        votingPool[i] = votingPool[i + 1];
    }
    votingPool[--*end] = temp;
}

// Sort by number of firstPrefCount, descending
void insertionSort(candidate_t votingPool[], int noCandidates) {
    // Insertion Sort structured from Geeks for Geeks

    for (int candidate = 1; candidate < noCandidates; candidate++) {

        candidate_t key = votingPool[candidate];
        int j = candidate - 1;

        while (j >= 0 && (votingPool[j].firstPrefCount <= 
                key.firstPrefCount)) {
            
            // Same vote count, sort alphabetically
            if (votingPool[j].firstPrefCount == 
                key.firstPrefCount) {
                
               // Smaller name goes first 
                if (strcmp(votingPool[j].name, key.name) < 0) {
                    break;
                }
            }

            votingPool[j + 1] = votingPool[j];
            j--;
        }
        votingPool[j + 1] = key;
    }
}

// Eliminate the candidate with the lowest poll by Linear Search
int elimination(candidate_t votingPool[], pollStatus_t stat, int *winner) {

    //Initilising value to first candidate.
    int minVotes = votingPool[0].firstPrefCount;
    int maxVotes = votingPool[0].firstPrefCount;
    int elimIndex = 0, potentialWinner = 0;

    for (int candidate = 0; candidate < stat.remainingCandidates; candidate++) {
        if (votingPool[candidate].firstPrefCount < minVotes) {
            minVotes = votingPool[candidate].firstPrefCount;
            elimIndex = candidate;
        }
        if (votingPool[candidate].firstPrefCount > maxVotes) {
            maxVotes = votingPool[candidate].firstPrefCount;
            potentialWinner = candidate;
        }
    }
    double percent = (double)maxVotes / (double)(stat.totalVotes) * 100.0;
    if (percent > MAJORITY) {
        *winner = potentialWinner;
    }
    votingPool[elimIndex].eliminated = TRUE;

    return elimIndex;
}

// Print out the candidate status before elimination
// Announce the eliminated candidate or the elected candidate after elimination
void statusPrintOut(candidate_t votingPool[], int elimIndex, 
                int *elected, int *winner, pollStatus_t stat) {
    
    for (int i = 0; i < stat.remainingCandidates; i++) {

        int voteCount = votingPool[i].firstPrefCount;
        double percent = (double)voteCount / (double)(stat.totalVotes) * 100.0;

        printf("    %-20s: %3d votes, %5.1lf%%\n",
                votingPool[i].name, voteCount, percent);
    }

    printf("    ----\n");

    // Winner declared if they have more than 50% of the votes
    // before elimination process
    if (*winner != NOTFOUND) {

        printf("    %s is declared elected\n", 
            votingPool[*winner].name);

        *elected = TRUE;
    }

    else {
        printf("    %s is eliminated and votes distributed\n", 
            votingPool[elimIndex].name);
    }
    printf("\n");
}

// Redistribute the votes of the eliminated candidate
// to the next preferred candidate on each ballot
void redistributeVotes(candidate_t votingPool[], pollStatus_t stat, 
                                                    int elimIndex) {

    // To include preferences of eliminated candidates
    // e.g, after second round of elimination,
    // all third preferences and below, from all ballots need to 
    // move into the remaining candidates
    int highestPreft = 
        stat.totalCandidates - stat.remainingCandidates + FIRSTPREF;

    for (int i = 0; i < stat.totalVotes; i++) {
        int pref = votingPool[stat.remainingCandidates].votes[i];
        // Lower preference than require for redistribution
        if (pref > highestPreft) { 
            continue;
        }

        // Find the next preferred candidate
        // by looking for the lowest preference number
        // among the remaining candidates.
        int incrementIndex = 0, lowestPref = MAXCANDIDATES + 1;

        //flag to indicate a lower preference in the ballot than
        // highestPreft
        int lowerPrefFound = FALSE; 

        for (int j = 0; j < stat.remainingCandidates;j++) {
            // Within this particular ballot,
            // if there is a candidate with a higher preference
            // than the eliminated candidate,
            // then no redistribution is needed.
            int currentPreft = votingPool[j].votes[i];
            if (currentPreft < pref) {
                lowerPrefFound = TRUE;
                break;
            }

            if (currentPreft < lowestPref) {
                lowestPref = votingPool[j].votes[i];
                incrementIndex = j;
            }
        }
        if (!lowerPrefFound) {
            votingPool[incrementIndex].firstPrefCount++;
        }
    }
}
// Reset the elimination status and firstPrefCount
void reset(candidate_t votingPool[], pollStatus_t *stat) {
    for (int i = 0; i < stat->totalCandidates; i++) {
        votingPool[i].firstPrefCount = votingPool[i].originalCount;
        votingPool[i].eliminated = FALSE;
    }
    stat->remainingCandidates = stat->totalCandidates;
}

// algorithms are fun :)