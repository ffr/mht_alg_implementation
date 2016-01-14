#ifndef MHT_TRACKING_ALG_H
#define MHT_TRACKING_ALG_H


#define BYTE_TYPE       int
#define COORDINATE_TYPE int
typedef struct TREE_NODE
{
	COORDINATE_TYPE x, y, z;
	TREE_NODE *left, *right;
} TreeNode;

/*
**	设置 MHT 算法使用的参数
**	Point Parameters: r(vessel radius), 
					  k(vessel contrast), m(image bg intenstiy)
**	Vessel Profile Parameters: gamma(controls the steepness 
										of the profile, 
**										bigger -> steeper)
**	Tracking Parameters: stepLength
*/
void set_mht_alg_parameters( double r, double k, double m, 
							 double gamma, double stepLength );

/*
**	MHT_TRACKING_ALGRITHM
**	返回指向 seed 节点的指针，分割出的血管用链表表示
**	如果算法执行失败，返回空指针
*/
TreeNode *mht_tracking_segment( BYTE_TYPE ***imageVolume, 
								int volume_L,
								int volume_W,
								int volume_H,
								TreeNode *seed );

#endif