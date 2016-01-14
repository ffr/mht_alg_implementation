#ifndef MHT_ALG_TREE_H
#define MHT_ALG_TREE_H

#define COORDINATE_TYPE int
#define BOOL int

typedef struct SEARCHTREE_NODE
{
	COORDINATE_TYPE 	x, y, z;
	double 				score;
	SEARCHTREE_NODE     *left, *right;
	SEARCHTREE_NODE     *parent;	
} SearchTreeNode;


/*
**	输入：指针，指向一个包含 x,y,z 坐标信息的节点（包含 x,y,z 字段的结构）
**	返回：指针，指向一个 searchTree 格式的节点，
*/
SearchTreeNode *searchTree_node( void *node_with_xyz_accord_info );

/*
**	插入一个节点
*/
BOOL searchTree_insert( SearchTreeNode *node );

/*
**	删除这个节点所在分支
*/
BOOL searchTree_delete_thisBranch( SearchTreeNode *node );

/*
**	当前分支的深度
*/
BOOL searchTree_this_branch_depth( SearchTreeNode *node );

/*
**	返回最后一个插入的节点
*/
SearchTreeNode *searchTree_this_leaf();

/*
**	返回所有叶子节点
*/
SearchTreeNode **searchTree_leaves();

/*
**	把 searchTree 置空，free 所有节点
*/
BOOL searchTree_setEmpty();

/*
**	查询 searchTree 是否为空
*/
BOOL searchTree_isEmpty();

#endif