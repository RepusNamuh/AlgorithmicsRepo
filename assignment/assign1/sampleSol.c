/* Program to implement Australian House of Representatives preferential
   voting and determine election outcomes from vote preference orderings.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2025, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.
   All included code is (c) Copyright University of Melbourne, 2025

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been
   developed by another student, or by me in collaboration with other
   students, or by non-students as a result of request, solicitation, or
   payment, may not be submitted for assessment in this subject.  I
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
   the University and/or inciting others to commit Academic Misconduct).  I
   understand that an allegation of Student General Misconduct may arise
   regardless of whether or not I personally make use of such solutions or
   sought benefit from such actions.

   Signed by: [Enter your full name and student number here before submission]
   Dated:     [Enter the date that you "signed" the declaration]

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>

/* SOLUTION-START */

#define DEBUG 0             //  set to 1 if debugging output wanted */

#define NAME_LEN   20       // max length of candidate name
#define MAX_CANDS  20       // maximum number of candidates
#define MAX_VOTES 999       // maximum number of electors

#define STAGE1      1
#define STAGE2      2
#define STAGE3      3

// add any further constants here

#define NO_WINNER   0
// add any typedefs and struct declarations (if you use them) here

// each candidate has a name, a current number of votes allocated,
// a computed percentage of the total, and a flag to indicate
// whether or not this candidate has been eliminated yet
//

typedef char word_t[NAME_LEN+1];

typedef struct {
	word_t name;
	int    votes;
	double pcent;
	int    eliminated;
} candidate_t;

// each voter has an array of preferences, and an additional
// variable used to indicate which one is the "current"
// top preference for this voter
//

typedef struct {
	int prefs[MAX_CANDS];
	int nextup;
} vote_t;

/* SOLUTION-END */
///////////////////////////////////////////////////////////////////////

int getword(char W[], int limit);

// add your other function prototypes here

/* SOLUTION-START */
void sort_candidates(candidate_t candidates[], int ncands, int *perm);
int  tabulate_votes(candidate_t candidates[], int ncands,
                vote_t votes[], int nvoters);
void redistribute_votes(candidate_t candidates[], int ncands,
                vote_t votes[], int nvoters,
                int eliminate);
void print_distribution(candidate_t candidates[], int ncands, int round,
                int perm[], int nvotes, int winner);
int least_frequent(candidate_t candidates[], int ncands);
int read_candidates(candidate_t candidates[]);
int read_votes(vote_t votes[], int ncands);
void stage1(candidate_t candidates[], int ncands, vote_t votes[], int nvotes);
void stage2_and_3(int stagenum, candidate_t candidates[], int ncands,
		vote_t votes[], int nvotes);
void print_stage(int stagenum);
/* SOLUTION-END */

///////////////////////////////////////////////////////////////////////

// main program provides traffic control

int
main(int argc, char *argv[]) {

	// write your main program here

/* SOLUTION-START */
        candidate_t candidates[MAX_CANDS+1];
		// note, candidate numbers start at 1
        int ncands;

        vote_t votes[MAX_VOTES];
        int nvotes;

	// fetch the candidate information
        ncands = read_candidates(candidates);

	// and then the votes information
        nvotes = read_votes(votes, ncands);

	// ready to go, here come the three stages...
	stage1(candidates, ncands, votes, nvotes);

	// stage 2 and 3 differ only in whether output lists
	// of candidates are to be in input order (stage 2) or
	// sorted by decreasing vote count (stage 3), and that's
	// accomplished through the use of a parameter to say
	// which stage we are doing

	// this is stage 2
	stage2_and_3(STAGE2, candidates, ncands, votes, nvotes);

	// and this is stage 3!
	stage2_and_3(STAGE3, candidates, ncands, votes, nvotes);

/* SOLUTION-END */

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
	if (c==EOF) {
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

///////////////////////////////////////////////////////////////////////

// add your other functions here

/* SOLUTION-START */

///////////////////////////////////////////////////////////////////////

// read the set of candidate names, knowing that the very first
// thing in the input is a integer to say how many candidates there
// are

int
read_candidates(candidate_t candidates[]) {

	int cand, ncands;

	// first read the number of candidate names that appear
	if (scanf("%d", &ncands) != 1) {
		fprintf(stderr, "data appears to be invalid\n");
		exit(EXIT_FAILURE);
	}

	// and then read that many strings as candidate names,
	// noting that candidate numbers start at 1, leaving the
	// first position in the array unused
	for (cand=1; cand <= ncands; cand++) {
		getword(candidates[cand].name, NAME_LEN);
	}

	// finally, tell the calling function how many candidates
	// there are in the array, 1..ncands
	return ncands;
}

///////////////////////////////////////////////////////////////////////

// read all the votes in, using a resizable array; each vote has
// the same number of preferences associated with it, one per
// candidate, permuted as they are read so that a list of candidate
// identifiers in preference order is generated
//

int
read_votes(vote_t votes[], int ncands) {
	int nvotes, pref, cand;

	nvotes = 0;
	cand = 1;

	// read all the preferences, one at a time, single loop is ok
	// because every line has the same number of votes on it
	while (scanf("%d", &pref) == 1) {

		// fill in the the next preference element for this voter,
		// keeping candidate numeric labels in decreasing preference
		// order so that later operations are easier
		votes[nvotes].prefs[pref] = cand;
		
		// prepare for next vote for this voter
		cand += 1;

		// but need to check, are we at the end for this voter?
		if (cand > ncands) {
			// end of the votes for this voter, prepare to
			// start reading for next voter
			cand = 1;
			nvotes++;
		}
	}
	return nvotes;
}

///////////////////////////////////////////////////////////////////////

// this function handles all of the stage 1 output requirements

void
stage1(candidate_t candidates[], int ncands, vote_t votes[], int nvotes) {

	int pref, cand, last=nvotes-1;

	print_stage(STAGE1);

        printf("read %d candidates", ncands);
        printf(" and %d votes\n", nvotes);


	printf("voter %d preferences...\n", nvotes);
	for (pref=1; pref <= ncands; pref++) {
		cand = votes[last].prefs[pref];
		printf("    rank %2d: %s\n", pref, candidates[cand].name);
	}
	
	return; 
}

///////////////////////////////////////////////////////////////////////

void
stage2_and_3(int stagenum, candidate_t candidates[], int ncands,
		vote_t votes[], int nvotes) {

	int cand, active, vote;
	int winner=NO_WINNER;
		// remember, valid candidates are >= 1

        int perm[MAX_CANDS+1];

	print_stage(stagenum);

	// set up an identity permutation on the candidates
	// as a starting point
	for (cand=1; cand <= ncands; cand++) {
		perm[cand] = cand;
		// and mark the candidate as not yet eliminated
		candidates[cand].eliminated = 0;
	}
	active = ncands;

	// and set all of the vote "nextup" counters to refer to
	// the first preference candidate for that voter
	for (vote=0; vote < nvotes; vote++) {
		votes[vote].nextup = 1;
	}

	// now run the election
	while (active > 0) {

		winner = tabulate_votes(candidates, ncands, votes, nvotes);

		// only print the candidates in permuted order in stage 3
		if (stagenum == 3) {
			sort_candidates(candidates, ncands, perm);
		}
		print_distribution(candidates, ncands, active,
				perm, nvotes, winner);
		if (winner != NO_WINNER) {
			// woohoo, a winner has emerged
			printf("    ----\n    %s is declared elected\n\n",
				candidates[winner].name);
			break;
		}
		// no winner yet, need to do another redistribution
		cand = least_frequent(candidates, ncands);
		printf("    ----\n    %s is eliminated",
			candidates[cand].name);
		printf(" and votes distributed\n\n");
		redistribute_votes(candidates, ncands,
				votes, nvotes, cand);
		active -= 1;
	}

	// and the winner has been found...
	return;
}

///////////////////////////////////////////////////////////////////////

// count through the votes and assign each one to the next
// non-eliminated candidate in that voter's preference list
//

int
tabulate_votes(candidate_t candidates[], int ncands, 
		vote_t votes[], int nvotes) {

	int vote;
	int cand;
	int pref;
	int winner=NO_WINNER;

	// set all candidate counters to zero
	for (cand=1; cand <= ncands; cand++) {
		candidates[cand].votes = 0;
	}

	// count all of the current votes
	for (vote=0; vote < nvotes; vote++) {
		pref = votes[vote].nextup;
		cand = votes[vote].prefs[pref];
		candidates[cand].votes++;
	}

	// compute the percentages
	for (cand=1; cand <= ncands; cand++) {
		candidates[cand].pcent = 100.0*
			candidates[cand].votes/nvotes;
		if (candidates[cand].votes * 2 > nvotes) {
			winner = cand;
		}
	}

	// mission accomplished
	return winner;
}

///////////////////////////////////////////////////////////////////////

// generate one round of the output, with the ordering of the
// candidates determined by the permutation vector "perm"
//
// for Stage 2 output, perm is always the identity mapping

void
print_distribution(candidate_t candidates[], int ncands,
		int active, int perm[], int nvotes, int winner) {

	int permuted;
	int cand;

	printf("round %d...\n", ncands - active + 1);

	for (permuted=1; permuted <= ncands; permuted++) {
		cand = perm[permuted];
		if (candidates[cand].eliminated) {
			/* don't print eliminated candidates */
		} else {
			printf("    %-*s: ", NAME_LEN, candidates[cand].name);
			printf("%3d votes, ", candidates[cand].votes);
			printf("%5.1f%%", candidates[cand].pcent);
			printf("\n");
		}
	}
	return;
}

///////////////////////////////////////////////////////////////////////

int
least_frequent(candidate_t candidates[], int ncands) {

	int cand, min;

	// to get started, need to find first candidate that has not
	// been eliminated
	for (cand=1; cand <= ncands; cand++) {
		if (!candidates[cand].eliminated) {
			min = cand;
			break;
		}
	}

	// can now check all other candidates from there on, comparing
	// to the current minimum one
	for ( ; cand <= ncands; cand++) {
		if (candidates[cand].eliminated) {
			// can't use this one
		} else if (candidates[cand].votes < candidates[min].votes) {
			// that's a new minimum, strictly smaller
			min = cand;
		}
	}
	
	// and now announce the eliminated candidate
	return min;
}

///////////////////////////////////////////////////////////////////////

// remove one candidate from contention, and adjust the nextup
// components in all votes that were until now supporting that
// candidate

void
redistribute_votes(candidate_t candidates[], int ncands,
		vote_t votes[], int nvotes,
		int mincand) {

	int vote;
	int pref;

	// set this candidate to "eliminated"
	candidates[mincand].eliminated = 1;

	// and now check the "nextup" link for every vote, and adjust
	// to bypass all currently eliminated candidates
	for (vote=0; vote < nvotes; vote++) {

		pref = votes[vote].nextup;
		while (candidates[votes[vote].prefs[pref]].eliminated) {
			// need to step this vote forwards in
			// preference list
			votes[vote].nextup++;
			pref = votes[vote].nextup;
		}
	}

	// mission accomplished 
	return;
}

///////////////////////////////////////////////////////////////////////

// creates a permutation vector to indicate ordering of candidates,
// in decreasing order of vote count and with ties broken by alpha
// of candidate names
//

void
sort_candidates(candidate_t candidates[], int ncands, int perm[]) {

	int i, j, t, voteorder;

	// assign initial permutation vector
	for (i=1; i<=ncands; i++) {
		perm[i] = i;
	}

	// ok to use quadratic-time sorting algorithm, since the number of
	// candidates is small compared to number of votes, and execution
	// time isn't an issue
	for (i=1; i<=ncands; i++) {
		for (j=1; j<=ncands-1; j++) {
			voteorder = candidates[perm[j]].votes -
				candidates[perm[j+1]].votes;
			if (voteorder<0 || ((voteorder==0) &&
					strcmp(candidates[perm[j]].name,
					candidates[perm[j+1]].name)>0)) {
				t = perm[j];
				perm[j] = perm[j+1];
				perm[j+1] = t;
			}
		}
	}

	// perm[i] now indicates the i'th candidate in decreasing vote
	// count order
	return;
}

///////////////////////////////////////////////////////////////////////

void
print_stage(int stagenum) {
	printf("\nStage %d\n=======\n", stagenum);
}

///////////////////////////////////////////////////////////////////////
/* SOLUTION-END */

