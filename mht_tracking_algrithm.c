#include "mht_tracking_algrithm.h"
#include "mht_alg_basetools.h"
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <malloc.h>

static BYTE_TYPE ***g_imageVolume;
static int g_volume_L;
static int g_volume_W;
static int g_volume_H;


static double g_r           = 0.75; 
static double g_k           = 0;
static double g_m           = 0;
static double g_gamma       = 8; 
static double g_stepLength  = 1.5*r;
static double g_searchDepth = 3;

extern SearchTreeNode;


/*
**	help function, 
**	反复调用 build_searchTree() && eval_searchTree() 直至完成算法
**	完成返回 TRUE
**	失败返回 FALSE
*/
static BOOL do_mht_tracking( TreeNode *node );

/*
**	help function，
**	根据传入的节点，搜索下一步可能节点，采用多假设策略，
**	所有的可能节点（>=1），一起用于下一步的搜索，直到达到搜索深度
*/
static BOOL build_searchTree( SearchTreeNode *node );

/*
**	help function,
**	评价 build 建立的 searchTree，
**	修剪，分支检测
**	
*/
static BOOL eval_searchTree( SearchTreeNode *node );


void
set_mht_alg_parameters( double r, double k, double m, 
							 double gamma, double stepLength )
{
	;
}

TreeNode *
mht_tracking_segment( BYTE_TYPE ***imageVolume, 								
								int volume_L,
								int volume_W,
								int volume_H,
								TreeNode *seed )
{
	/*
	**	检查参数有效性
	*/
	assert( imageVolume != NULL && 
			*imageVolume != NULL && 
			**imageVolume != NULL )
	assert( volume_L > 0 && volume_W > 0 && volume_H > 0 );
	assert( seed != NULL &&
			seed->x > 0 && seed->x < volume_L &&
			seed->y > 0 && seed->x < volume_W &&
			seed->z > 0 && seed->x < volume_H );

	g_imageVolume = imageVolume;
	g_volume_L = volume_L;
	g_volume_W = volume_W;
	g_volume_H = volume_H;

	/*
	**	-----—- TRACKING ------
	**	从 seed 节点提供的坐标开始
	**	寻找下一个满足条件的坐标
	**	直到在一次搜索中没有出现满足条件的坐标
	**	Tracking 结束，返回 True,
	**	任何错误中断，返回 False
	*/
	SearchTreeNode *node = searchTree_node( seed );
	assert( node != NULL );

	if ( do_mht_tracking( node ) ) {
		return seed;
	}

	return NULL;
}


/*
** ---------------- 私有函数部分 ----------------
*/

static BOOL
do_mht_tracking( SearchTreeNode *node )
{	
	assert( node != NULL );
	
	/*
	**	从给定的点开始，建立一个 search tree
	*/
	if ( !build_searchTree( node ) ) {
		return FALSE;
	} 

	/*
	**	对 search tree 进行修剪，返回数组指针，数组里的元素指向每个分支的叶子节点
	*/
	if ( !eval_searchTree( node ) )
	{
		return FALSE;
	}

	/*
	**	如果 furcation 指向的数组不只包含 NULL，说明 search tree 有效
	**	把 search tree 作为子树添加到 vesselTree
	**	把 search tree 置空，准备下一次 build
	**	依次对当前分支节点再进行 tracking
	*/
	SearchTreeNode *(*furcation)[] = searchTree_leaves();

	for ( ; *furcation != NULL; furcation++ ) {
		vesselTree_insert_subTree( node );
		searchTree_setEmpty();
		do_mht_tracking( *furcation );
	}

	return TRUE;
}

static BOOL
build_searchTree( SearchTreeNode *node )
{

	assert( node != NULL );

	/*
	**	search tree 的根节点
	*/
	if ( searchTree_isEmpty() ) {
		searchTree_insert( node );
	}
	
	/*
	**	寻找下一步范围内符合条件的点
	*/
	SearchTreeNode *(*predictions)[] = step_prediction( node );
	find_localMaxima( predictions );
	fit_parameters( predictions );
	prune_with_threshold( predictions );

	int num_survivor = length( predictions, SearchTreeNode );
	assert( num_survivor <= 2 );

	/*
	**	把符合条件的点从左到右插入 search tree
	**	直到最右边的分支达到 search depth 
	*/
	for ( ; *predictions != NULL; predictions++) {
		searchTree_insert( *predictions );
		if ( searchTree_this_branch_depth() < g_searchDepth ) {
			build_search_tree( searchTree_this_leaf() );
		}
	}
	
}

static BOOL
eval_searchTree( SearchTreeNode *node )
{
	assert( !searchTree_isEmpty() );

	/*
	**	计算每个分支的均值，结果保留在叶子节点的 score 里（可以使用层次遍历）
	**	对每个分支按阈值裁剪
	*/
	cal_avg_score_of_each_branch( node );
	prune_search_tree( node );

	/*
	**	如果没有被剪光，对剩下的分支进行分叉检验，返回一个数组指针，
	**  数组内包含一个或两个节点指针，
	*/
	if ( !searchTree_isEmpty() ) {
		return bifurcation_detection( searchTree_leaves() )
	}

	return FALSE;
}
