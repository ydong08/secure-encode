#include "ClusterTree.h"
/**************************************************************************************
********************* 全局函数：将一个Int类型的数转换成一个string *********************
**************************************************************************************/
string intToString(int i)
{
	stringstream s;
	s << i;
	return s.str();
}

/**************************************************************************************
************************************* 类  ClusterNode *********************************
**************************************************************************************/

//***********************计算该条记录到该节点中心的距离********************************
float ClusterNode::CalCenterDistance(strMyRecord* pRecord)
{
	double fDist;
	fDist = 0;
	/*
	fDist += pow((pRecord->fProtocolType - fCenter[0]), 2);
	fDist += pow((pRecord->fService - fCenter[1]), 2);
	fDist += pow((pRecord->fStatusFlag - fCenter[2]), 2);
	*/
	fDist += pow((pRecord->fSrcBytes - fCenter[3]), 2);
	fDist += pow((pRecord->fDestBytes - fCenter[4]), 2);
	fDist += pow((pRecord->fFailedLogins - fCenter[5]), 2);
	fDist += pow((pRecord->fNumofRoot - fCenter[6]), 2);
	fDist += pow((pRecord->fCount - fCenter[7]), 2);
	fDist += pow((pRecord->fSrvCount - fCenter[8]), 2);
	fDist += pow((pRecord->fRerrorRate - fCenter[9]), 2);
	fDist += pow((pRecord->fSameSrvRate - fCenter[10]), 2);
	fDist += pow((pRecord->fDiffSrvRate - fCenter[11]), 2);
	fDist += pow((pRecord->fDstHostSrvCount - fCenter[12]), 2);
	fDist += pow((pRecord->fDstHostSameSrvRate - fCenter[13]), 2);
	fDist += pow((pRecord->fDstHostDiffSrvRate - fCenter[14]), 2);
	fDist += pow((pRecord->fDstHostSameSrcPortRate - fCenter[15]), 2);
	fDist += pow((pRecord->fDstHostSrvDiffHostRate - fCenter[16]), 2);
	fDist += pow((pRecord->fDstHostSrvSerrorRate - fCenter[17]), 2);

	return float(fDist);	
}

//******************************* 获得孩子i的指针 **************************************
ClusterNode* ClusterNode::GetChildNode(int i)
{
	return pChildNode[i];
}

//**************************** 获得本节点的聚类标签 *************************************
int ClusterNode::GetClusterNodeLabel()
{
	return int(fCenter[18]);
}

//*********** 递归函数，在以该节点为根的子树中，获得与数据记录最近的聚类节点 ************
ClusterNode* ClusterNode::GetNearestCluster(strMyRecord* pRecord)
{
	//cout<<"In the GetNearestCluster"<<endl;
	//cout<<"Path = "<<strPath<<endl;
	//outfile<<"Path = "<<strPath<<endl;
	
	int i;
	float MinDistance,TmpDistance;        //最短距离
	ClusterNode* pNearestNode;            //最近节点
	ClusterNode* pTmpNode;                //临时节点
	
	if(IsLeaf > 0)
	{
		pNearestNode = this;
  }
  else
  {
  	pNearestNode = this;
  	//计算本节点与数据的距离d1
	  MinDistance = CalCenterDistance(pRecord);

		//判断是否有孩子节点，如果有，调用递归函数，获得孩子节点所在的子树中离数据距离最近的节点指针
		for(i=0;i<MAXLABEL;i++)
		{
			if(pChildNode[i] != NULL)
			{
				pTmpNode = pChildNode[i]->GetNearestCluster(pRecord);
			}
		  //计算最近子节点与数据的距离d2
		  TmpDistance = pTmpNode->CalCenterDistance(pRecord);
		  //如果d2 < d1，返回本节点的指针，否则，返回子节点指针
		  if(TmpDistance < MinDistance)
		  {
		  	pNearestNode = pTmpNode;
		  	MinDistance = TmpDistance;
		  }
		}  	
  }


	//返回最近节点的指针
	return pNearestNode;
}

// *******************************  打印聚类节点  **************************************
void ClusterNode::Print()
{
	int i;
	
	cout<<"Cluster "<<strPath<<endl;
	//先打印自己的内容
	/*
	cout<<"fProtocolType = "<<fCenter[0]<<endl;
	cout<<"fService = "<<fCenter[1]<<endl;
	cout<<"fStatusFlag = "<<fCenter[2]<<endl;
	*/
	cout<<"------------------ Center Information -----------------"<<endl;
	cout<<"fSrcBytes = "<<fCenter[3]<<endl;
	cout<<"fDestBytes = "<<fCenter[4]<<endl;
	cout<<"fFailedLogins = "<<fCenter[5]<<endl;
	cout<<"fNumofRoot = "<<fCenter[6]<<endl;
	cout<<"fCount = "<<fCenter[7]<<endl;
	cout<<"fSrvCount = "<<fCenter[8]<<endl;
	cout<<"fRerrorRate = "<<fCenter[9]<<endl;
	cout<<"fSameSrvRate = "<<fCenter[10]<<endl;
	cout<<"fDiffSrvRate = "<<fCenter[11]<<endl;
	cout<<"fDstHostSrvCount = "<<fCenter[12]<<endl; 
	cout<<"fDstHostSameSrvRate = "<<fCenter[13]<<endl;
	cout<<"fDstHostDiffSrvRate = "<<fCenter[14]<<endl;
	cout<<"fDstHostSameSrcPortRate = "<<fCenter[15]<<endl;
	cout<<"fDstHostSrvDiffHostRate = "<<fCenter[16]<<endl;
	cout<<"fDstHostSrvSerrorRate = "<<fCenter[17]<<endl;
	cout<<"iLabel = "<<fCenter[18]<<endl;
	cout<<"--------------------- Cluster Param ---------------------"<<endl;
	cout<<"IsClusterOK = "<<IsClusterOK<<endl;
	cout<<"IsLeaf = "<<IsLeaf<<endl;
	cout<<"ClusterResult = "<<ClusterResult<<endl;
	cout<<"---------------------- Label Number ---------------------"<<endl;
	cout<<"normal = "<<iLabelNum[0]<<endl;
	cout<<"dos = "<<iLabelNum[1]<<endl;
	cout<<"probe = "<<iLabelNum[2]<<endl;
	cout<<"u2r = "<<iLabelNum[3]<<endl;
	cout<<"r2l = "<<iLabelNum[4]<<endl;
	cout<<"============================================================"<<endl;	
	
	//如果孩子节点的指针不为空，就打印孩子节点的内容
	for(i=0;i<MAXLABEL;i++)
	{
		if(pChildNode[i] != NULL)
		{
			pChildNode[i]->Print();
		}
	}
}

// ************************  在Log文件中输出聚类节点  ****************************
void ClusterNode::PrintLog()
{
	int i;
	
	outfile<<"Cluster "<<strPath<<endl;
	//先打印自己的内容
	/*
	outfile<<"fProtocolType = "<<fCenter[0]<<endl;
	outfile<<"fService = "<<fCenter[1]<<endl;
	outfile<<"fStatusFlag = "<<fCenter[2]<<endl;
	*/
	outfile<<"------------------ Center Information -----------------"<<endl;
	outfile<<"fSrcBytes = "<<fCenter[3]<<endl;
	outfile<<"fDestBytes = "<<fCenter[4]<<endl;
	outfile<<"fFailedLogins = "<<fCenter[5]<<endl;
	outfile<<"fNumofRoot = "<<fCenter[6]<<endl;
	outfile<<"fCount = "<<fCenter[7]<<endl;
	outfile<<"fSrvCount = "<<fCenter[8]<<endl;
	outfile<<"fRerrorRate = "<<fCenter[9]<<endl;
	outfile<<"fSameSrvRate = "<<fCenter[10]<<endl;
	outfile<<"fDiffSrvRate = "<<fCenter[11]<<endl;
	outfile<<"fDstHostSrvCount = "<<fCenter[12]<<endl; 
	outfile<<"fDstHostSameSrvRate = "<<fCenter[13]<<endl;
	outfile<<"fDstHostDiffSrvRate = "<<fCenter[14]<<endl;
	outfile<<"fDstHostSameSrcPortRate = "<<fCenter[15]<<endl;
	outfile<<"fDstHostSrvDiffHostRate = "<<fCenter[16]<<endl;
	outfile<<"fDstHostSrvSerrorRate = "<<fCenter[17]<<endl;
	outfile<<"iLabel = "<<fCenter[18]<<endl;
	outfile<<"IsClusterOK = "<<IsClusterOK<<endl;
	outfile<<"IsLeaf = "<<IsLeaf<<endl;
	outfile<<"---------------------- Label Number ---------------------"<<endl;
	outfile<<"normal = "<<iLabelNum[0]<<endl;
	outfile<<"dos = "<<iLabelNum[1]<<endl;
	outfile<<"probe = "<<iLabelNum[2]<<endl;
	outfile<<"u2r = "<<iLabelNum[3]<<endl;
	outfile<<"r2l = "<<iLabelNum[4]<<endl;
	outfile<<"============================================================"<<endl;	
	
	//如果孩子节点的指针不为空，就打印孩子节点的内容
	for(i=0;i<MAXLABEL;i++)
	{
		if(pChildNode[i] != NULL)
		{
			pChildNode[i]->PrintLog();
		}
	}
}

/**************************************************************************************
************************************* 类  ClusterTree *********************************
**************************************************************************************/

//********************************** 获得根节点 *************************************
ClusterNode* ClusterTree::GetRootNode()
{
	return pRootNode;
}

//**********************************  插入节点 **************************************
void ClusterTree::InsertNode(ClusterNode* pParent,ClusterNode* pNode,int i)
{
	string strTmp1, strTmp2;
	
	//子节点的父亲指针指向父亲
	pNode->pParentNode = pParent;
	//父节点的子指针i指向第i个子节点
	pParent->pChildNode[i] = pNode;
	//为值集结点的路径赋值
	strTmp1 = pParent->strPath;
	strTmp2 = intToString(i);
	strTmp1 += ".";
	strTmp1 += strTmp2;
	pNode->strPath = strTmp1;
}

//************************* 找到与给定记录距离最近的聚类 ****************************
ClusterNode* ClusterTree::FindNearestCluster(strMyRecord* pRecord)
{
	ClusterNode* pNearestNode;
  pNearestNode = pRootNode->GetNearestCluster(pRecord);
	return pNearestNode;
}

// *******************************  打印聚类树  **************************************
void ClusterTree::Print()
{
	cout<<"Start printing ClusterTree ..."<<endl;
	outfile<<"Start printing ClusterTree ..."<<endl;
	pRootNode->Print();
	cout<<"Printing ClusterTree finished !"<<endl;
	outfile<<"Printing ClusterTree finished !"<<endl;
}
// *************************  将聚类树输出到Log文件中  ********************************
void ClusterTree::PrintLog()
{
	cout<<"Start printing ClusterTree in Log ..."<<endl;
	outfile<<"Start printing ClusterTree in Log ..."<<endl;
	pRootNode->PrintLog();
	cout<<"Printing ClusterTree in Log finished !"<<endl;
	outfile<<"Printing ClusterTree in Log finished !"<<endl;
}

