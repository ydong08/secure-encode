#include "Commom.h"

extern ofstream outfile;

/**************************************************************************************
************************************* 类  ClusterNode *********************************
**************************************************************************************/

class ClusterNode
{
public:
	//构造函数
	ClusterNode()
	{
		int i;
		//父指针设为NULL
		pParentNode = NULL;
		for(i=0;i < MAXLABEL;i++)
		{
			pChildNode[i] = NULL;         //孩子指针设为NULL
			iLabelNum[i] = 0;             //标签数目设置为0
		}
		for(i=0;i<MAXDIMENSION;i++)
		{
			fCenter[i] = 0;               //聚类中心点均设为0
		}
		ClusterResult = 0;              //聚合度设置为0
		IsClusterOK = false;            //设为不符合聚类标准
		IsLeaf = 0;                     //设置为非叶子节点
	};
	
	//计算该条记录到该节点中心的距离
	float CalCenterDistance(strMyRecord* pRecord);
	//获得孩子i的指针
	ClusterNode* GetChildNode(int i);
	//获得本节点的聚类标签
	int GetClusterNodeLabel();
	//递归函数，在以该节点为根的子树中，获得与数据记录最近的聚类节点
	ClusterNode* GetNearestCluster(strMyRecord* pRecord);
	//递归函数，打印节点信息
	void Print();
	//将聚类树输出到日志文件中
	void PrintLog();
	
	
public:
	//聚类节点的中心
	float fCenter[MAXDIMENSION];
	//聚类节点的路径
	string strPath;
	//指向这个聚类节点的父节点的指针
	ClusterNode* pParentNode;
	//指向这个聚类节点的孩子节点的指针
	ClusterNode* pChildNode[MAXLABEL];	
	//聚合度
	float ClusterResult;
	//聚合结果是否符合标准
	bool IsClusterOK;
	//判断是否为叶子节点，并判断是否是在聚类正常的情况下终止的叶子
	//0: 非叶子节点、 1：叶子节点且聚类正常结束 ； 2、叶子节点且聚类非正常结束
	int IsLeaf;
	//记录各种分类标签的数目
	int iLabelNum[MAXLABEL];
};

/**************************************************************************************
************************************* 类  ClusterTree *********************************
**************************************************************************************/
class ClusterTree
{
public:
	//构造函数
	ClusterTree()
	{
		pRootNode = new ClusterNode();
		pRootNode->strPath = "0";
	}
	
	//插入操作
	void InsertNode(ClusterNode* pParent,ClusterNode* pNode,int i);
	//找到与给定记录距离最近的聚类
	ClusterNode* FindNearestCluster(strMyRecord* pRecord);
	//获得根节点
	ClusterNode* GetRootNode();	
  //打印聚类树
	void Print();
	//将聚类树输出到日志文件中
	void PrintLog();
		
private:
	//根节点
	ClusterNode* pRootNode;
};

