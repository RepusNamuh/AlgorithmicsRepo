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
#define MAXVOTES 999
#define TRUE 1
#define FALSE 0
#define FIRSTROUND 1

typedef char name_t[NAMEMAX + 1];

typedef struct {
    name_t name;
    int votes[MAXVOTES];
    int firstPrefCount;  // number of votes in favour of this candidate
    int eliminated;
} candidate_t;

///////////////////////////////////////////////////////////////////////

int getword(char W[], int limit);

int readVotes(candidate_t candidates[], int noCandidates, 
                int *noVotes, int limit);

void stageOne(candidate_t candidates[], int noCandidates, int noVotes);

void stageTwo(candidate_t candidates[], int noCandidates, 
                int noVotes, int stage);

void redistributeVotes(candidate_t candidates[], int noCandidates, 
                            int noVotes, int elimIndex);

int elimination(candidate_t votingPool[], int *remainingCandidates);

void statusReport(candidate_t votingPool[], int noCandidates, 
                int elimIndex, int *elected, int noVotes);

void insertionSort(candidate_t votingPool[], int noCandidates);
///////////////////////////////////////////////////////////////////////

// main program provides traffic control
int
main(int argc, char *argv[]) {
    // initialising variable and arrays
    candidate_t candidates[MAXCANDIDATES], dummy;
    int noCandidates = 0, noVotes = 0;

    //Reading the input
    scanf("%d", &noCandidates);
    if (noCandidates > MAXCANDIDATES) {
        printf("Too many candidates");
        exit(EXIT_FAILURE);
    }

    // Getting the names of candidates and initialising
    // votes count and elimination status to 0
    int i = 0;
    while (i < noCandidates) {
        getword(dummy.name, NAMEMAX);
        strcpy(candidates[i].name, dummy.name);
        candidates[i].firstPrefCount = 0;
        candidates[i].eliminated = 0;
        i++;
    }

    // Getting the votes for each candidate
    readVotes(candidates, noCandidates, &noVotes, MAXVOTES);

    // Outputting stage one results
    printf("\n");
    stageOne(candidates, noCandidates, noVotes);

    // Stage two and three of the preferential voting system
    stageTwo(candidates, noCandidates, noVotes, 2);
    printf("\n");
    stageTwo(candidates, noCandidates, noVotes, 3);

    // all done, time to go home
    printf("tadaa!\n");
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
readVotes(candidate_t candidates[], int noCandidates, 
                                int *noVotes, int limit) {
    int c, i = 0, preference;

    while ((c=scanf("%d", &preference)) == 1 && *noVotes < limit) {
        // new line indicates a new voter
        if (i >= noCandidates) {
            i = 0;
            (*noVotes)++;
        }

        candidates[i].votes[*noVotes] = preference;
        // if the preference for the current candidate is 
        // 1 (first preference), increase their vote count.
        if (preference == 1) {
            candidates[i].firstPrefCount++;
        }
        i++;
    }
    (*noVotes)++;
    return 0;
}

void stageOne(candidate_t candidates[], int noCandidates, int noVotes) {
    // Stage one of the preferential voting system
    printf("Stage 1\n");
    printf("=======\n");
    printf("read %d candidates and %d votes\n", noCandidates, noVotes);
    printf("voter %d preferences...\n", noVotes);

    name_t tempArray[noCandidates];
    // Linear Reordering of candidates based on 
    // the last voter's preferences.
    for (int i = 0; i < noCandidates; i++) {
        int index = candidates[i].votes[noVotes - 1];
        strcpy(tempArray[index - 1], candidates[i].name);
    }

    // Print out the last voter's preferences
    for (int i = 0; i < noCandidates; i++) {
        printf("    rank %2d: %s\n", i + 1, tempArray[i]);
    }

    printf("\n");
}

// Elimination stage by removing candidate with lowest winning votes
// each round and redistribute the votes.
void stageTwo(candidate_t candidates[], int noCandidates, 
                int noVotes, int stage) {

    printf("Stage %d\n", stage);
    printf("=======\n");
    int elected = FALSE, round = FIRSTROUND;
    int remainingCandidates = noCandidates;
    int elimIndex = 0;

    candidate_t votingPool[remainingCandidates];
    for (int i = 0; i < remainingCandidates; i++) {
        votingPool[i] = candidates[i];
    }

    while (!elected) {
        printf("round %d...\n", round);

        // Move eliminated candidate to the end of the array
        
        if (stage == 3) {
            // Insertion Sort, sorting from highest preference to lowest
            insertionSort(votingPool, remainingCandidates);
        }
        
        elimIndex = elimination(votingPool, &remainingCandidates);
        statusReport(votingPool, remainingCandidates, elimIndex, 
                                                &elected, noVotes);
        redistributeVotes(votingPool, remainingCandidates, noVotes, 
                                                        elimIndex);
        
        // Shift the eliminated candidate to the end of the array
        // maintaining the order of the remaining candidates
        candidate_t temp = votingPool[elimIndex];
        for (int i = elimIndex; i < remainingCandidates; i++) {
            votingPool[i] = votingPool[i + 1];
        }
        votingPool[remainingCandidates] = temp;
        round++;
    }
}

void insertionSort(candidate_t votingPool[], int noCandidates) {
    // Insertion Sort structured from Geeks for Geeks
    for (int i = 1; i < noCandidates; i++) {

        candidate_t key = votingPool[i];
        int j = i - 1;

        while (votingPool[j].firstPrefCount <= 
                key.firstPrefCount && j >= 0) {
            
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
int elimination(candidate_t votingPool[], int *remainingCandidates) {
    int minVotes = votingPool[0].firstPrefCount;
    int elimIndex = 0;

    for (int i = 1; i < *remainingCandidates; i++) {
        if (votingPool[i].firstPrefCount < minVotes) {
            minVotes = votingPool[i].firstPrefCount;
            elimIndex = i;
        }
    }

    votingPool[elimIndex].eliminated = 1;
    (*remainingCandidates)--;

    return elimIndex;
}

// Print out the candidate status before elimination
// Announce the eliminated candidate or the elected candidate after elimination
void statusReport(candidate_t votingPool[], int noCandidates, int elimIndex, 
                int *elected, int noVotes) {
    
    // i <= noCandidates to include the eliminated candidate
    // Since the loop below report status pre-elimination
    for (int i = 0; i <= noCandidates; i++) {

        int voteCount = votingPool[i].firstPrefCount;
        double percent = (double)voteCount / (double)(noVotes) * 100.0;

        printf("    %-20s: %3d votes, %5.1lf%%\n",
                votingPool[i].name, voteCount, percent);
    }

    printf("    ----\n");

    // Index of the winning candidate if there is only one candidate left
    // Winner declared if they have more than 50% of the votes
    int winner = noCandidates - elimIndex;
    if ((noCandidates == 1 || noCandidates == 0) &&
        votingPool[winner].firstPrefCount > (noVotes / 2)) {

        printf("    %s is declared elected\n", 
            votingPool[winner].name);

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
void redistributeVotes(candidate_t votingPool[], 
    int noCandidates, int noVotes, int elimIndex) {

    for (int i = 0; i < noVotes; i++) {
        int pref = votingPool[elimIndex].votes[i];
        if (pref != 1) {
            continue;
        }

        // Find the next preferred candidate
        // by looking for the lowest preference number
        // among the remaining candidates.
        // <= noCandidates to include size pre-elimination
        // allowing all remaining candidates to be considered
        int incrementIndex = 0, lowestPref = MAXCANDIDATES + 1;
        for (int j = 0; j <= noCandidates;j++) {
            if (votingPool[j].eliminated == 1) {
                continue;
            }
            if (votingPool[j].votes[i] < lowestPref) {
                lowestPref = votingPool[j].votes[i];
                incrementIndex = j;
            }
        }
        votingPool[incrementIndex].firstPrefCount++;
    }
}

// algorithms are fun :)