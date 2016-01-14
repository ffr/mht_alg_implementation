#ifndef MHT_ALG_BASETOOLS_H
#define MHT_ALG_BASETOOLS_H

#include "mht_alg_tree.h"

#define BOOL int

extern SearchTreeNode;

/*
**	DES: Produce a number of predictions of the next vessel segment, 
**			and score them.
**	PAR: this vessel segment
**	RET: Pointer to the predictions
*/
SearchTreeNode **step_prediction( SearchTreeNode *node );


/*
**	DES: Find the local maxima in the score pattern
**	PAR: the predictions
**	RET: the num of localMaxima, if failed, return -1
*/
int find_localMaxima( SearchTreeNode **predictions );

/*
**	DES: Fit the vessel model parameters to the image data
**	PAR: the localMaxima
**	RET: whether succeed or failed 
*/
BOOL fit_parameters( SearchTreeNode **predictions );

/*
**	DES: Discard models for which the scores 
**			do not survive a pruning threshold
**	PAR: the localMaxima
**	RET: the num of survivors, return -1 if failed
*/	
int prune_with_threshold( SearchTreeNode **predictions );

/*
**	DES: cal how many prediction in the predictions
**	PAR: array pointer
**	RET: the num of elements, return -1 if failed
*/
int length( SearchTreeNode **predictions );

/*
**	DES: Average the scores of the model segments 
**			leading to each leaf of the search tree
**	PAR: search tree's root
**	RET: whether succeed or failed 
*/
BOOL cal_avg_score_of_each_branch( SearchTreeNode *node );

/*
**	DES: Discard leaves for which the average scores 
**			do not survive a termination threshold.
**	PAR: search tree's root
**	RET: the num of survivors, return -1 if failed 
*/
int prune_search_tree( SearchTreeNode *node );

/*
**	DES: bifurcation detection
**	PAR: canidates
**	RET: TRUE or FALSE
*/
BOOL bifurcation_detection( SearchTreeNode **node );


#endif

