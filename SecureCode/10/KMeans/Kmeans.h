#include "Commom.h"
#include "ClusterTree.h"

/*********************************** 全局变量 *****************************************/
ofstream outfile("Log.txt");          //日志文件
ofstream Rstfile("Result.txt");       //结果文件

int KmeansID = 0;                     //Kmeans对象ID号

/********************************** 聚类的结构 ****************************************/
struct Cluster 
{
	float Center[MAXDIMENSION];             //聚类中心
	list <strMyRecord*> MemberList;         //记录链表，用于保存属于该类的记录
	int iNumMembers;                        //记录属于该类的记录数
};

/**************************************************************************************
**********************************   类CKMeans  ***************************************
**************************************************************************************/
class CKMeans  
{
public:
	//构造函数1
	CKMeans(ClusterTree* pTree,int KmeansID,int Level,int NumDimensions)
	{
		cout<<"********** Creat a new Kmeans, ID = "<<KmeansID<<" **********"<<endl;
		outfile<<"********** Creat a new Kmeans, ID = "<<KmeansID<<" **********"<<endl;
		
		pClusterTree = pTree;
		pSelfClusterNode = pClusterTree->GetRootNode();
		
		m_iNumDimensions = NumDimensions;
		m_ClusterLevel = Level;
		m_KmeansID = KmeansID;
		m_iNumRecords = 0;
		m_iNumClusters = 0;
	};
	//构造函数2
	CKMeans(ClusterNode* pSelf,ClusterTree* pTree,int KmeansID,int Level,int NumDimensions,list <strMyRecord*>* pDataList)
	{
		cout<<"********** Creat a new Kmeans, ID = "<<KmeansID<<" **********"<<endl;
		outfile<<"********** Creat a new Kmeans, ID = "<<KmeansID<<" **********"<<endl;
		
		pClusterTree = pTree;
		pSelfClusterNode = pSelf;
		
		m_iNumDimensions = NumDimensions;
		m_ClusterLevel = Level;
		m_KmeansID = KmeansID;
		m_iNumRecords = 0;
		m_iNumClusters = 0;
		m_RecordsList = (*pDataList);
	};
	
  //读取数据预处理后，文件中的记录
  bool ReadTrainingRecords();
	// K-means算法的第一步：从n个数据对象任意选择k个对象作为初始聚类中心
	void InitClusters(unsigned int NumClusters);	
	// K-means算法的第二步：把每个对象分配给与之距离最近的聚类
	void DistributeSamples();
	// K-means算法的第三步：重新计算每个聚类的中心
	bool CalcNewClustCenters();
	// 计算指定数据对象与聚类中心的欧几里德距离	
	float CalcEucNorm(strMyRecord *pRecord, int id);
	// 找到对离给定数据象最近的一个聚类
	int  FindClosestCluster(strMyRecord *pRecord);
	//打印聚类中心
	void PrintClusters();
	// K-means算法的总体过程
	void RunKMeans(int Kvalue);
	//在KMeans算法运行之后，查询所有聚类的标签数
	void GetClustersLabel();
	//检查聚类后一类中的分类是否合理
	bool IsClusterOK(int i);
	//获得聚类i链表
	list <strMyRecord*>* GetClusterList(int i);
	//打印本CKMeans对象中一个聚类的label
	void PrintClusterLabel(int i);
	//获得本对象中某一个聚类包含不同的Label个数
	int GetDiffLabelofCluster(int i);
	//判断是否结束递归过程
	bool IsStopRecursion(int i);
	//创建聚类数节点
	void CreatClusterTreeNode(ClusterNode* pParent);
	
private:
	//判断该条记录与之前的聚类中心是否完全相同
	bool IsSameAsCluster(int i,strMyRecord* pRecord);
	//为聚类中心i赋值
  void EvaluateCluster(int i,strMyRecord* pRecord);

private:	
	FILE *pInFile;                             // 记录文件的指针
  list <strMyRecord*> m_RecordsList;         // 数据记录
	unsigned int  m_iNumClusters;				       // 聚类的类别数（即K值）
	int m_iNumRecords;				                 // 数据记录的行数
	int m_iNumDimensions;			                 // 数据记录的维数
	Cluster m_Cluster[MAXCLUSTER];             // 每一个聚类
	int m_ClusterLevel;                        // 聚类对象所处在的层次
	int m_KmeansID;                            // Kmean对象ID号
	ClusterTree* pClusterTree;                 // 聚类树的指针
	ClusterNode* pClusterNode[MAXLABEL];       // 聚类节点的指针
	ClusterNode* pSelfClusterNode;             // 与本CKmeans对象相关的Cluster对象
};


/**************************************************************************************
******************************  类ConfuseMatrix  **************************************
**************************************************************************************/
class ConfuseMatrix
{
public:
  //构造函数
  ConfuseMatrix()
  {
  	int i,j;
  	//将混淆矩阵的各项都设置为0
  	for(i=0;i<MAXLABEL;i++)
  	{
  		for(j=0;j<MAXLABEL;j++)
  		{
  			CfusMatrix[i][j] = 0;
  		}
  	}  	
  };
  
  //数值更新函数
  void UpdateValue(int iTrueLabel,int iPreLabel)
  {
  	CfusMatrix[iTrueLabel][iPreLabel]++;
  };
  
  //打印混淆矩阵
  void PrintMatrix()
  {
  	int i,j;
  	cout<<"=================================  Confusion Matrix  =================================="<<endl;
  	//打印第一行
  	cout<<setiosflags(ios::left)<<setw(5)<<"T/P";
  	for(i=0;i<MAXLABEL;i++)
  	{
  		cout<<setiosflags(ios::left)<<setw(7)<<i;
  	}
  	cout<<endl;
  	//打印矩阵内容
  	for(i=0;i<MAXLABEL;i++)
  	{
  		cout<<setiosflags(ios::left)<<setw(5)<<i;
  		for(j=0;j<MAXLABEL;j++)
  		{
  			cout<<setiosflags(ios::left)<<setw(7)<<CfusMatrix[i][j];
  		}
  		cout<<endl;
  	}    	
  };
  
  //将混淆矩阵输出到结果文件中
  void PrintMatrixToLog()
  {
  	int i,j;
  	Rstfile<<"=================================  Confusion Matrix  =================================="<<endl;
  	//打印第一行
  	Rstfile<<setiosflags(ios::left)<<setw(5)<<"T/P";
  	for(i=0;i<MAXLABEL;i++)
  	{
  		Rstfile<<setiosflags(ios::left)<<setw(7)<<i;
  	}
  	Rstfile<<endl;
  	//打印矩阵内容
  	for(i=0;i<MAXLABEL;i++)
  	{
  		Rstfile<<setiosflags(ios::left)<<setw(5)<<i;
  		for(j=0;j<MAXLABEL;j++)
  		{
  			Rstfile<<setiosflags(ios::left)<<setw(7)<<CfusMatrix[i][j];
  		}
  		Rstfile<<endl;
  	}    	
  };  
   	
private:
	int CfusMatrix[MAXLABEL][MAXLABEL];                //混淆矩阵
};

/**************************************************************************************
**********************读取数据预处理后，文件中的记录***********************************
**************************************************************************************/
bool CKMeans::ReadTrainingRecords()
{
	int iCount;                         //训练文件记录数
	
	char szBuf[MAX_BUF_SIZE];           //读文件缓存  MAX_BUF_SIZE = 512
	char szSeps[] = ",\r\n";
	string strTemp;
	strMyRecord* pRecord;               //遍历记录的指针
	
	//打印日志	
	cout<<"Start reading Records from training file ..."<<endl;
	//打开训练文件	
	if ((pInFile = fopen("kddcup.data_10_percent_datatreat", "r")) == NULL)
	{
		cout<<"Open training-data file faied !"<<endl;
		return false;
	}
	else
	{
		iCount = 0;
		while (fgets(szBuf, MAX_BUF_SIZE, pInFile) != NULL)
		{
			if(++iCount%100000 == 0)
				cout<<"----------- "<<iCount<<"  lines have read ----------"<<endl;

			pRecord = new  strMyRecord;
			//02 协议类型
			strTemp = strtok(szBuf, szSeps);
			pRecord->fProtocolType = (float)atof(strTemp.c_str());
			//03 服务类型
			strTemp = strtok(NULL, szSeps);
			pRecord->fService = (float)atof(strTemp.c_str());
			//04 状态标志
			strTemp = strtok(NULL, szSeps);
			pRecord->fStatusFlag = (float)atof(strTemp.c_str());
			//05 源到目的字节数
			strTemp = strtok(NULL, szSeps);
			pRecord->fSrcBytes = (float)atof(strTemp.c_str());
			//06 目的到源字节数
			strTemp = strtok(NULL, szSeps);
			pRecord->fDestBytes = (float)atof(strTemp.c_str());
			//11 登录失败次数
			strTemp = strtok(NULL, szSeps);
			pRecord->fFailedLogins = (float)atof(strTemp.c_str());
			//14 是否获得root用户权限不要
			strtok(NULL, szSeps);
			//16 root用户权限存取次数
			strTemp = strtok(NULL, szSeps);
			pRecord->fNumofRoot = (float)atof(strTemp.c_str());
			//22 是否是guest用户登录不要
			strtok(NULL, szSeps);
			//23 2秒内连接相同主机数目
			strTemp = strtok(NULL, szSeps);
			pRecord->fCount = (float)atof(strTemp.c_str());
			//24 2秒内连接相同端口数目
			strTemp = strtok(NULL, szSeps);
			pRecord->fSrvCount = (float)atof(strTemp.c_str());
			//27 "REJ"错误的连接数比率
			strTemp = strtok(NULL, szSeps);
			pRecord->fRerrorRate = (float)atof(strTemp.c_str());
			//29 连接到相同端口数比率
			strTemp = strtok(NULL, szSeps);
			pRecord->fSameSrvRate = (float)atof(strTemp.c_str());
			//30 连接到不同端口数比率
			strTemp = strtok(NULL, szSeps);
			pRecord->fDiffSrvRate = (float)atof(strTemp.c_str());
			//33 相同目的地相同端口连接数
			strTemp = strtok(NULL, szSeps);
			pRecord->fDstHostSrvCount = (float)atof(strTemp.c_str());
			//34 相同目的地相同端口连接数比率
			strTemp = strtok(NULL, szSeps);
			pRecord->fDstHostSameSrvRate = (float)atof(strTemp.c_str());
			//35 相同目的地不同端口连接数比率
			strTemp = strtok(NULL, szSeps);
			pRecord->fDstHostDiffSrvRate = (float)atof(strTemp.c_str());
			//36 相同目的地相同源端口连接比率
			strTemp = strtok(NULL, szSeps);
			pRecord->fDstHostSameSrcPortRate = (float)atof(strTemp.c_str());
			//37 不同主机连接相同端口比率
			strTemp = strtok(NULL, szSeps);
			pRecord->fDstHostSrvDiffHostRate = (float)atof(strTemp.c_str());
			//39 连接当前主机有S0错误的比率
			strTemp = strtok(NULL, szSeps);
			pRecord->fDstHostSrvSerrorRate = (float)atof(strTemp.c_str());
			//42 类型标签
			strTemp = strtok(NULL, szSeps);
			pRecord->iLabel = atoi(strTemp.c_str());
			
			//在链表尾部插入记录
		  m_RecordsList.push_back(pRecord);
		}
		m_iNumRecords = iCount;
		cout<<iCount<<" Records have read from training file!"<<endl;	
		return true;
	}
}

/**************************************************************************************
*********************判断该条记录与之前的聚类中心是否完全相同**************************
**************************************************************************************/
bool CKMeans::IsSameAsCluster(int i,strMyRecord* pRecord)
{
	//cout<<"In function IsSameAsCluster"<<endl;
	int j;
	bool Sameflag= false;
	
	for(j = 0;j < i;j++)
	{   

		  if(m_Cluster[j].Center[18] == pRecord->iLabel && j < m_iNumClusters)
		  {
		  	Sameflag = true;
				break;
		  }
			else if(m_Cluster[j].Center[0] != pRecord->fProtocolType)
			{
				continue;
			}
			else if(m_Cluster[j].Center[1] != pRecord->fService)
			{
				continue;
			}
			else if(m_Cluster[j].Center[2] != pRecord->fStatusFlag)
			{
				continue;
			}			
			else if(m_Cluster[j].Center[3] != pRecord->fSrcBytes)
			{
				continue;
			}
			else if(m_Cluster[j].Center[4] != pRecord->fDestBytes)
			{
				continue;
			}
			else if(m_Cluster[j].Center[5] != pRecord->fFailedLogins)
			{
				continue;
			}
			else if(m_Cluster[j].Center[6] != pRecord->fNumofRoot)
			{
				continue;
			}
			else if(m_Cluster[j].Center[7] != pRecord->fCount)
			{
				continue;
			}
			else if(m_Cluster[j].Center[8] != pRecord->fSrvCount)
			{
				continue;
			}
			else if(m_Cluster[j].Center[9] != pRecord->fRerrorRate)
			{
				continue;
			}
			else if(m_Cluster[j].Center[10] != pRecord->fSameSrvRate)
			{
				continue;
			}
			else if(m_Cluster[j].Center[11] != pRecord->fDiffSrvRate)
			{
				continue;
			}
			else if(m_Cluster[j].Center[12] != pRecord->fDstHostSrvCount)
			{
				continue;
			}
			else if(m_Cluster[j].Center[13] != pRecord->fDstHostSameSrvRate)
			{
				continue;
			}
			else if(m_Cluster[j].Center[14] != pRecord->fDstHostDiffSrvRate)
			{
				continue;
			}
			else if(m_Cluster[j].Center[15] != pRecord->fDstHostSameSrcPortRate)
			{
				continue;
			}
			else if(m_Cluster[j].Center[16] != pRecord->fDstHostSrvDiffHostRate)
			{
				continue;
			}
			else if(m_Cluster[j].Center[17] != pRecord->fDstHostSrvSerrorRate)
			{
				continue;
			}
			else     //如果已经和一个中心节点在数值上完全相等
			{
				Sameflag = true;
				break;
			}
	}	
	return Sameflag;
}

/**************************************************************************************
*********************************为聚类中心i赋值***************************************
**************************************************************************************/
void CKMeans::EvaluateCluster(int j,strMyRecord* pRecord)
{
	  //cout<<"In function EvaluateCluster"<<endl;
		//初始的聚类中心值
		m_Cluster[j].Center[0] = pRecord->fProtocolType;
		m_Cluster[j].Center[1] = pRecord->fService;
		m_Cluster[j].Center[2] = pRecord->fStatusFlag;		
		m_Cluster[j].Center[3] = pRecord->fSrcBytes;
    m_Cluster[j].Center[4] = pRecord->fDestBytes;
		m_Cluster[j].Center[5] = pRecord->fFailedLogins;
		m_Cluster[j].Center[6] = pRecord->fNumofRoot;
		m_Cluster[j].Center[7] = pRecord->fCount;
		m_Cluster[j].Center[8] = pRecord->fSrvCount;
		m_Cluster[j].Center[9] = pRecord->fRerrorRate;
		m_Cluster[j].Center[10]= pRecord->fSameSrvRate;
		m_Cluster[j].Center[11]= pRecord->fDiffSrvRate;
		m_Cluster[j].Center[12]= pRecord->fDstHostSrvCount;
		m_Cluster[j].Center[13]= pRecord->fDstHostSameSrvRate;
    m_Cluster[j].Center[14]= pRecord->fDstHostDiffSrvRate;
    m_Cluster[j].Center[15]= pRecord->fDstHostSameSrcPortRate;
    m_Cluster[j].Center[16]= pRecord->fDstHostSrvDiffHostRate;
    m_Cluster[j].Center[17]= pRecord->fDstHostSrvSerrorRate;
    m_Cluster[j].Center[18]= pRecord->iLabel;
}

/**************************************************************************************
*********K-means算法的第一步：从n个数据对象任意选择k个对象作为初始聚类中心*************
**************************************************************************************/
void CKMeans::InitClusters(unsigned int NumClusters)
{	
	//cout<<"In function InitClusters"<<endl;
	int i;
	strMyRecord* pRecord;                             //遍历记录的指针
	list<strMyRecord*>::iterator RecdListIter;        //记录链表的迭代器
	
	m_iNumClusters = NumClusters;	                    //对预测的聚类数k进行赋值
	RecdListIter = m_RecordsList.begin();
	
	//初始化m_iNumClusters个类的中心点
	for(i=0; i < m_iNumClusters; i++)
	{
		//cout<<"i ="<<i<<endl;
		
		pRecord = (*RecdListIter);
		
		//在记录链表中查找出一个与之前的聚类中心数值不同的记录作为中心
    while(IsSameAsCluster(i,pRecord))
    {
    	RecdListIter++;
    	pRecord = (*RecdListIter);
    }
    
    //将找到的记录作为一个新的聚类中心保存起来
    m_Cluster[i].MemberList.push_back(pRecord);       //将记录插入该类的成员链表
		m_Cluster[i].iNumMembers = 1;                     //将类的成员数设置为1
    EvaluateCluster(i,pRecord);
	}	
}

/**************************************************************************************
**************************************打印聚类中心*************************************
**************************************************************************************/
void CKMeans::PrintClusters()
{
	int i;
	outfile<<"================================================"<<endl;
	outfile<<"There are totally "<<m_iNumClusters<<" Clusters."<<endl;
	for(i=0; i < m_iNumClusters; i++)
	{
		outfile<<"+++++++++++++ Cluster Center "<<i<<" +++++++++++++"<<endl;
		
		outfile<<"fProtocolType = "<<m_Cluster[i].Center[0]<<endl;
		outfile<<"fService = "<<m_Cluster[i].Center[1]<<endl;
		outfile<<"fStatusFlag = "<<m_Cluster[i].Center[2]<<endl;
		outfile<<"fSrcBytes = "<<m_Cluster[i].Center[3]<<endl;
		outfile<<"fDestBytes = "<<m_Cluster[i].Center[4]<<endl;
		outfile<<"fFailedLogins = "<<m_Cluster[i].Center[5]<<endl;
		outfile<<"fNumofRoot = "<<m_Cluster[i].Center[6]<<endl;
		outfile<<"fCount = "<<m_Cluster[i].Center[7]<<endl;
		outfile<<"fSrvCount = "<<m_Cluster[i].Center[8]<<endl;
		outfile<<"fRerrorRate = "<<m_Cluster[i].Center[9]<<endl;
		outfile<<"fSameSrvRate = "<<m_Cluster[i].Center[10]<<endl;
		outfile<<"fDiffSrvRate = "<<m_Cluster[i].Center[11]<<endl;
		outfile<<"fDstHostSrvCount = "<<m_Cluster[i].Center[12]<<endl; 
		outfile<<"fDstHostSameSrvRate = "<<m_Cluster[i].Center[13]<<endl;
		outfile<<"fDstHostDiffSrvRate = "<<m_Cluster[i].Center[14]<<endl;
		outfile<<"fDstHostSameSrcPortRate = "<<m_Cluster[i].Center[15]<<endl;
		outfile<<"fDstHostSrvDiffHostRate = "<<m_Cluster[i].Center[16]<<endl;
		outfile<<"fDstHostSrvSerrorRate = "<<m_Cluster[i].Center[17]<<endl;
		outfile<<"iLabel = "<<m_Cluster[i].Center[18]<<endl;
		
		outfile<<"----------------------------------------------------"<<endl;
		outfile<<"Number of members in Cluster "<<i<<" = "<<m_Cluster[i].MemberList.size()<<endl;	
		outfile<<"iNumMembers = "<<m_Cluster[i].iNumMembers<<endl;
	}
}

/**************************************************************************************
***********************计算指定数据对象与聚类中心的欧几里德距离************************
**************************************************************************************/
float CKMeans::CalcEucNorm(strMyRecord *pRecord, int id)
{
	double fDist;
	fDist = 0;
	/*
	fDist += pow((pRecord->fProtocolType - m_Cluster[id].Center[0]), 2);
	fDist += pow((pRecord->fService - m_Cluster[id].Center[1]), 2);
	fDist += pow((pRecord->fStatusFlag - m_Cluster[id].Center[2]), 2);
	*/
	fDist += pow((pRecord->fSrcBytes - m_Cluster[id].Center[3]), 2);
	fDist += pow((pRecord->fDestBytes - m_Cluster[id].Center[4]), 2);
	fDist += pow((pRecord->fFailedLogins - m_Cluster[id].Center[5]), 2);
	fDist += pow((pRecord->fNumofRoot - m_Cluster[id].Center[6]), 2);
	fDist += pow((pRecord->fCount - m_Cluster[id].Center[7]), 2);
	fDist += pow((pRecord->fSrvCount - m_Cluster[id].Center[8]), 2);
	fDist += pow((pRecord->fRerrorRate - m_Cluster[id].Center[9]), 2);
	fDist += pow((pRecord->fSameSrvRate - m_Cluster[id].Center[10]), 2);
	fDist += pow((pRecord->fDiffSrvRate - m_Cluster[id].Center[11]), 2);
	fDist += pow((pRecord->fDstHostSrvCount - m_Cluster[id].Center[12]), 2);
	fDist += pow((pRecord->fDstHostSameSrvRate - m_Cluster[id].Center[13]), 2);
	fDist += pow((pRecord->fDstHostDiffSrvRate - m_Cluster[id].Center[14]), 2);
	fDist += pow((pRecord->fDstHostSameSrcPortRate - m_Cluster[id].Center[15]), 2);
	fDist += pow((pRecord->fDstHostSrvDiffHostRate - m_Cluster[id].Center[16]), 2);
	fDist += pow((pRecord->fDstHostSrvSerrorRate - m_Cluster[id].Center[17]), 2);

	return float(fDist);	
}

/**************************************************************************************
****************************找到对离给定数据象最近的一个聚类***************************
**************************************************************************************/
int CKMeans::FindClosestCluster(strMyRecord *pRecord)
{
	int iClusterId;
	float fMinDist, temp;

	fMinDist = CalcEucNorm(pRecord, 0);
	iClusterId = 0;

	for(int i=1; i < m_iNumClusters; i++)
	{
		temp = CalcEucNorm(pRecord, i);
		if(temp < fMinDist)
		{
			fMinDist = temp;
			iClusterId = i;
		}
	}
	return iClusterId;	
}

/**************************************************************************************
****************************找到对离给定数据象最近的一个聚类***************************
**************************************************************************************/
void CKMeans::DistributeSamples()
{
	int iClusterId,i;
	//清空每一个聚类链表中的所有记录
	for (i=0; i < m_iNumClusters; i++)
	{
		m_Cluster[i].MemberList.clear();
		m_Cluster[i].iNumMembers=0;
	}
	//遍历记录链表，将每一条记录根据与聚类中心的距离进行分类
	strMyRecord* pRecord;                              //遍历记录的指针
	list<strMyRecord*>::iterator RecdListIter;         //记录链表的迭代器
				
	for (RecdListIter = m_RecordsList.begin(); RecdListIter != m_RecordsList.end(); RecdListIter++)
	{
		pRecord = (*RecdListIter);
		iClusterId = FindClosestCluster(pRecord);
		m_Cluster[iClusterId].MemberList.push_back(pRecord);
		m_Cluster[iClusterId].iNumMembers++;
	}
}

/**************************************************************************************
********************** K-means算法的第三步：重新计算每个聚类的中心*********************
**************************************************************************************/
bool CKMeans::CalcNewClustCenters()
{
	bool  IsNewRound;
	int i,j;
	float TempCenter[MAXDIMENSION];
	strMyRecord* pRecord;                              //遍历记录的指针
	list<strMyRecord*>::iterator ClusterListIter;         //记录链表的迭代器
	
	IsNewRound = true;
	
	//遍历聚类
	for(i=0; i < m_iNumClusters; i++)
	{
		//遍历每一个属性值
		for(j=0; j<m_iNumDimensions; j++)
		{
			TempCenter[j] = 0.0;
		}
    for(ClusterListIter = m_Cluster[i].MemberList.begin(); ClusterListIter != m_Cluster[i].MemberList.end(); ClusterListIter++)
	  {
	  	pRecord = (*ClusterListIter);
 			
			TempCenter[0] += pRecord->fProtocolType;
		  TempCenter[1] += pRecord->fService;
		  TempCenter[2] += pRecord->fStatusFlag;		
		  TempCenter[3] += pRecord->fSrcBytes;
      TempCenter[4] += pRecord->fDestBytes;
		  TempCenter[5] += pRecord->fFailedLogins;
		  TempCenter[6] += pRecord->fNumofRoot;
		  TempCenter[7] += pRecord->fCount;
		  TempCenter[8] += pRecord->fSrvCount;
		  TempCenter[9] += pRecord->fRerrorRate;
		  TempCenter[10]+= pRecord->fSameSrvRate;
		  TempCenter[11]+= pRecord->fDiffSrvRate;
		  TempCenter[12]+= pRecord->fDstHostSrvCount;
		  TempCenter[13]+= pRecord->fDstHostSameSrvRate;
      TempCenter[14]+= pRecord->fDstHostDiffSrvRate;
      TempCenter[15]+= pRecord->fDstHostSameSrcPortRate;
      TempCenter[16]+= pRecord->fDstHostSrvDiffHostRate;
      TempCenter[17]+= pRecord->fDstHostSrvSerrorRate;
      //TempCenter[18]+= pRecord->iLabel;	
    }

		for(j=3; j<(m_iNumDimensions-1); j++)
		{
			TempCenter[j] = TempCenter[j]/m_Cluster[i].iNumMembers;
			if(TempCenter[j] != m_Cluster[i].Center[j])
			{
				IsNewRound = false;
			}
			m_Cluster[i].Center[j] = TempCenter[j];
		}
	}
	return IsNewRound;	
}

/**************************************************************************************
********************************** 获得每一个聚类label的统计信息 **********************
**************************************************************************************/
void CKMeans::GetClustersLabel()
{
	int i,k,max;
	int LabelCount[5];
	strMyRecord* pRecord;                                 //遍历记录的指针
	list<strMyRecord*>::iterator ClusterListIter;         //记录链表的迭代器
	

	cout<<"************** Cluster Level"<<m_ClusterLevel<<" **************"<<endl;
	outfile<<"************** Cluster Level"<<m_ClusterLevel<<" **************"<<endl;
	
	for(i=0; i < m_iNumClusters; i++)
	{
	  for(k=0;k < 5;k++)
	  {
		  LabelCount[k] = 0;
		}
		
		cout<<"------------- Kmeans"<<m_KmeansID<<" Cluster"<<i<<" -------------"<<endl;
		outfile<<"------------- Kmeans"<<m_KmeansID<<" Cluster"<<i<<" -------------"<<endl;
		
		for(ClusterListIter = m_Cluster[i].MemberList.begin(); ClusterListIter != m_Cluster[i].MemberList.end(); ClusterListIter++)
		{
			 pRecord = (*ClusterListIter);
			 switch(pRecord->iLabel)
			 {
			 	 case 0:
			 	 	LabelCount[0]++;
			 	 	break;
			 	 case 1:
			 	 	LabelCount[1]++;
			 	 	break;
			 	 case 2:
			 	 	LabelCount[2]++;
			 	 	break;
			 	 case 3:
			 	 	LabelCount[3]++;
			 	 	break;
			 	 case 4:
			 	 	LabelCount[4]++;
			 	 	break;
			 	 default:
			 	 	break;
			 }
		}
		
		//将标签数赋值给节点
	  for(k=0;k < 5;k++)
		{
			pClusterNode[i]->iLabelNum[k] = LabelCount[k];
		}
		
	 //找出标签的最大值
		for(k=0,max=0;k < 5;k++)
	  {
	  	if(LabelCount[max] < LabelCount[k])
	  	{
	  		max = k;
	  	}
		}
		//将记录数最多的标签作为该聚类的标签
		pClusterNode[i]->fCenter[18] = max;
		
		cout<<"normal = "<<LabelCount[0]<<endl;
		cout<<"dos = "<<LabelCount[1]<<endl;
		cout<<"probe = "<<LabelCount[2]<<endl;
		cout<<"u2r = "<<LabelCount[3]<<endl;
		cout<<"r2l = "<<LabelCount[4]<<endl;
		
		outfile<<"normal = "<<LabelCount[0]<<endl;
		outfile<<"dos = "<<LabelCount[1]<<endl;
		outfile<<"probe = "<<LabelCount[2]<<endl;
		outfile<<"u2r = "<<LabelCount[3]<<endl;
		outfile<<"r2l = "<<LabelCount[4]<<endl;
	}
	cout<<"********************************************"<<endl;
	outfile<<"*******************************************"<<endl;
}
/**************************************************************************************
**********************检查聚类后一类中的分类是否合理***********************************
**************************************************************************************/
bool CKMeans::IsClusterOK(int i)
{
	  int j,k,sum,max;
		int LabelCount[5];
		float Result;                                        //计算结果
	  strMyRecord* pRecord;                                 //遍历记录的指针
	  list<strMyRecord*>::iterator ClusterListIter;         //记录链表的迭代器    
	  
    //将初始值设置为0
	  for(k=0;k < 5;k++)
	  {
		  LabelCount[k] = 0;
		}
		//统计每一种标签的个数    
		for(ClusterListIter = m_Cluster[i].MemberList.begin(); ClusterListIter != m_Cluster[i].MemberList.end(); ClusterListIter++)
		{
			 pRecord = (*ClusterListIter);
			 switch(pRecord->iLabel)
			 {
			 	 case 0:
			 	 	LabelCount[0]++;
			 	 	break;
			 	 case 1:
			 	 	LabelCount[1]++;
			 	 	break;
			 	 case 2:
			 	 	LabelCount[2]++;
			 	 	break;
			 	 case 3:
			 	 	LabelCount[3]++;
			 	 	break;
			 	 case 4:
			 	 	LabelCount[4]++;
			 	 	break;
			 	 default:
			 	 	break;
			 }
		}
		//判断聚类是否合理
		//找出标签的最大值
		for(k=0,max=0;k < 5;k++)
	  {
	  	if(LabelCount[max] < LabelCount[k])
	  	{
	  		max = k;
	  	}
		}
		//计算其余各个标签的数据和
		for(k=0,sum=0;k < 5;k++)
		{
			if(k != max)
				sum += LabelCount[k];
		}
		
		Result = float(sum)/float(LabelCount[max]);	
		pClusterNode[i]->ClusterResult = Result;
				

    if(m_ClusterLevel <= INTERLEVEL)       //m_ClusterLevel <= 3
    {
    	if(sum >100)                         //其它标签的数目大于100，直接要求再分
    	{
    		cout<<sum<<" > 100"<<endl;
    		outfile<<sum<<" > 100"<<endl;
    		return false;
    	}
    	else
    	{
	    	if(Result < CLUSTER_PRECISION)
	    	{
					cout<<0<<" <= "<<Result<<" < "<<CLUSTER_PRECISION<<endl;
					outfile<<0<<" <= "<<Result<<" < "<<CLUSTER_PRECISION<<endl;
					pClusterNode[i]->IsClusterOK = true;
					pClusterNode[i]->IsLeaf = 1;                               //正常叶子节点
	    		return true;
	    	}
	    	else
	    	{
					cout<<Result<<" > "<<CLUSTER_PRECISION<<endl;
					outfile<<Result<<" > "<<CLUSTER_PRECISION<<endl;
					pClusterNode[i]->IsClusterOK = false;
	    		return false;
	    	}
      }
    }
    else                                   //m_ClusterLevel > 3
    { 
    	if(sum >500)                         //其它标签的数目大于500，直接要求再分
    	{
    		cout<<sum<<" > 500"<<endl;
    		outfile<<sum<<" > 500"<<endl;
    		return false;
    	}
    	else
	    {
	    	if(Result < (m_ClusterLevel-INTERLEVEL)*CLUSTER_PRECISION)
	    	{
	    		for(k=1;k <= (m_ClusterLevel-INTERLEVEL);k++)
	    		{
	    			 if(Result < k*CLUSTER_PRECISION)
	    			 {
				 				cout<<((k-1)*CLUSTER_PRECISION)<<" <= "<<Result<<" < "<<(k*CLUSTER_PRECISION)<<endl;
								outfile<<((k-1)*CLUSTER_PRECISION)<<" <= "<<Result<<" < "<<(k*CLUSTER_PRECISION)<<endl;
								pClusterNode[i]->IsLeaf = 1;                               //正常叶子节点
								pClusterNode[i]->IsClusterOK = true;
				    		return true;   			 	
	    			 }
	    		}
	    	}
	    	else
	    	{
				 	cout<<Result<<" > "<<(m_ClusterLevel-INTERLEVEL)*CLUSTER_PRECISION<<endl;
					outfile<<Result<<" > "<<(m_ClusterLevel-INTERLEVEL)*CLUSTER_PRECISION<<endl;
					pClusterNode[i]->IsClusterOK = false;
	    		return false;
	    	}
      }
    }
}

/**************************************************************************************
********************************** 获得聚类i的记录链表 ********************************
*************************************************************************************/
list <strMyRecord*>* CKMeans::GetClusterList(int i)
{
	list <strMyRecord*>* pClustList;
	pClustList = &(m_Cluster[i].MemberList);
	return pClustList;
}

/**************************************************************************************
****************************** 打印本类中一个聚类的label ******************************
*************************************************************************************/
void CKMeans::PrintClusterLabel(int i)
{
	int k;
	int LabelCount[5];
	strMyRecord* pRecord;                                 //遍历记录的指针
	list<strMyRecord*>::iterator ClusterListIter;         //记录链表的迭代器  
	  	
	cout<<"Cluster "<<m_ClusterLevel<<"."<<i<<endl;
	for(k=0;k < 5;k++)
	{
		 LabelCount[k] = 0;
	}
	for(ClusterListIter = m_Cluster[i].MemberList.begin(); ClusterListIter != m_Cluster[i].MemberList.end(); ClusterListIter++)
	{
		pRecord = (*ClusterListIter);
		switch(pRecord->iLabel)
		{
			case 0:
			 	LabelCount[0]++;
			 	break;
			case 1:
			 	LabelCount[1]++;
			 	break;
			case 2:
			 	LabelCount[2]++;
			 	break;
			case 3:
			 	LabelCount[3]++;
			 	break;
			case 4:
			 	LabelCount[4]++;
			 	break;
			default:
			 	break;
		}
	}
	cout<<"------------------------------"<<endl;
	cout<<"normal = "<<LabelCount[0]<<endl;
	cout<<"dos = "<<LabelCount[1]<<endl;
	cout<<"probe = "<<LabelCount[2]<<endl;
	cout<<"u2r = "<<LabelCount[3]<<endl;
	cout<<"r2l = "<<LabelCount[4]<<endl;	
	
	outfile<<"------------------------------"<<endl;
	outfile<<"normal = "<<LabelCount[0]<<endl;
	outfile<<"dos = "<<LabelCount[1]<<endl;
	outfile<<"probe = "<<LabelCount[2]<<endl;
	outfile<<"u2r = "<<LabelCount[3]<<endl;
	outfile<<"r2l = "<<LabelCount[4]<<endl;
}

/**************************************************************************************
************************获得本对象中某一个聚类包含不同的Label个数**********************
**************************************************************************************/
int CKMeans::GetDiffLabelofCluster(int i)
{
	int k,DiffLabel;
	int LabelCount[5];
	strMyRecord* pRecord;                                 //遍历记录的指针
	list<strMyRecord*>::iterator ClusterListIter;         //记录链表的迭代器 
		
	for(k=0;k < 5;k++)
	{
		 LabelCount[k] = 0;
	}
	for(ClusterListIter = m_Cluster[i].MemberList.begin(); ClusterListIter != m_Cluster[i].MemberList.end(); ClusterListIter++)
	{
		pRecord = (*ClusterListIter);
		switch(pRecord->iLabel)
		{
			case 0:
			 	LabelCount[0]++;
			 	break;
			case 1:
			 	LabelCount[1]++;
			 	break;
			case 2:
			 	LabelCount[2]++;
			 	break;
			case 3:
			 	LabelCount[3]++;
			 	break;
			case 4:
			 	LabelCount[4]++;
			 	break;
			default:
			 	break;
			}
	}
	for(k=0,DiffLabel=0;k<5;k++)
	{
		if(LabelCount[k] > 0)
			DiffLabel++;
	}
  return DiffLabel;	
}

/**************************************************************************************
********************************** 判断是否结束递归过程 *******************************
**************************************************************************************/
bool CKMeans::IsStopRecursion(int i)
{
	int k,DiffLabel,NextLevel,sum,max;
	int LabelCount[5];
	float Result;    
	strMyRecord* pRecord;                                 //遍历记录的指针
	list<strMyRecord*>::iterator ClusterListIter;         //记录链表的迭代器 
		
	NextLevel = m_ClusterLevel+1;                         //下一次聚类将处在的层次

	if(NextLevel <= INTERLEVEL)                     //NextLevel <= 3
	{
		pSelfClusterNode->IsLeaf = 0;                    //非叶子节点
		return false;
	}
	else                                                  //NextLevel > 3
	{
		if(NextLevel <= MAXLEVEL)                           //NextLevel <= 8
		{
			//统计各类标签的个数
			for(k=0;k < 5;k++)
			{
				 LabelCount[k] = 0;
			}
			for(ClusterListIter = m_Cluster[i].MemberList.begin(); ClusterListIter != m_Cluster[i].MemberList.end(); ClusterListIter++)
			{
				pRecord = (*ClusterListIter);
				switch(pRecord->iLabel)
				{
					case 0:
					 	LabelCount[0]++;
					 	break;
					case 1:
					 	LabelCount[1]++;
					 	break;
					case 2:
					 	LabelCount[2]++;
					 	break;
					case 3:
					 	LabelCount[3]++;
					 	break;
					case 4:
					 	LabelCount[4]++;
					 	break;
					default:
					 	break;
					}
			}			
			//找出标签的最大值
			for(k=0,max=0;k < 5;k++)
		  {
		  	if(LabelCount[max] < LabelCount[k])
		  	{
		  		max = k;
		  	}
			}
			//计算其余各个标签的数据和
			for(k=0,sum=0;k < 5;k++)
			{
				if(k != max)
					sum += LabelCount[k];
			}
			//计算聚合比率
			Result = float(sum)/float(LabelCount[max]);
			
      if(LabelCount[max] > 200)                                                   //如果还剩200条以上的记录，继续
      {
      	pSelfClusterNode->IsLeaf = 0;                                             //非叶子节点
      	return false;
      }
      else if(LabelCount[max] > 100 && LabelCount[max] <= 200 && Result > 0.33)   //如果还剩100到200条的记录，并且比率大于0.33，继续
      {
	      cout<<"100 < Max Label <=200 and Result = "<<Result<<" > 0.33"<<endl;
	      outfile<<"100 < Max Label <=200 and Result = "<<Result<<" > 0.33"<<endl;
	      pSelfClusterNode->IsLeaf = 0;                    //非叶子节点
      	return false;
      }
      else if(LabelCount[max] > 0 && LabelCount[max] <= 100 && Result > 0.5)      //如果还剩不到100条的记录，并且比率大于0.5，继续
      {
	      cout<<"0 < Max Label <=200 and Result = "<<Result<<" > 0.5"<<endl;
	      outfile<<"0 < Max Label <=200 and Result = "<<Result<<" > 0.5"<<endl;
	      pSelfClusterNode->IsLeaf = 0;                    //非叶子节点
      	return false;
      }					
		  else                                               //其它情况
		  {
		  	pSelfClusterNode->IsLeaf = 2;                    //异常叶子节点
				return true;
		  }
		}
		else
	  {
	  	pSelfClusterNode->IsLeaf = 2;                      //异常叶子节点
			return true;
	  }	
	}
}

/**************************************************************************************
************************************ 为聚类结果创建节点 *******************************
**************************************************************************************/
void CKMeans::CreatClusterTreeNode(ClusterNode* pParent)
{
	int i,k;
	
	//为每一个聚类结果创建节点
	for(i=0; i < m_iNumClusters; i++)
	{
		//动态创建一个新的节点
		pClusterNode[i] = new ClusterNode();
		
		//为这个节点赋值
		for(k=3; k<(m_iNumDimensions-1); k++)
		{
			pClusterNode[i]->fCenter[k] = m_Cluster[i].Center[k];
	  }
		//将这个节点插入到聚类树中
		pClusterTree->InsertNode(pParent,pClusterNode[i],i);
  }	
}


/**************************************************************************************
********************************** K-means算法的总体过程 ******************************
**************************************************************************************/
void CKMeans::RunKMeans(int Kvalue)
{
	int i;
	int NextKvalue;                        //下一步聚类的K 值
	int CircleNum;                         //Kmeans 算法循环的次数
	bool IsFinish;                         //判断是Kmeans算法计算过程是否结束
	int NextLevel;                         //下一个聚类层次
	list <strMyRecord*>* pTmpClustList;    //数据链表指针
	
	IsFinish = false;
	CircleNum = 0;
		
	 
	//初始化聚类
	InitClusters(Kvalue);
	
	//PrintClusters();
	
  //进入聚类循环
  cout<<"Start clustering proccess !"<<endl;
	outfile<<"Start clustering proccess !"<<endl;
	while(!IsFinish)
	{
		++CircleNum;

		cout<<setiosflags(ios::left)<<"----------- Cirle "<<setw(2)<<CircleNum<<" ------------"<<endl; 
		outfile<<setiosflags(ios::left)<<"----------- Cirle "<<setw(2)<<CircleNum<<" ------------"<<endl;
		
		DistributeSamples();                             //将样本记录分配给最近的聚类中心
		IsFinish = CalcNewClustCenters();		             //重新计算聚类中心
	}
	cout<<"The Proccess of clustering is finished !"<<endl;
	outfile<<"The Proccess of clustering is finished !"<<endl;
	//PrintClusters();
	//打印每一个Cluster的标签统计数
	cout<<"Print clustering result ...."<<endl;
	outfile<<"Print clustering result ...."<<endl;	
	CreatClusterTreeNode(pSelfClusterNode);            //创建聚类节点
	GetClustersLabel();
	
	//判断聚类的结果是否合格
	for(i=0;i < Kvalue;i++)
	{
		cout<<"Check Kmeans"<<m_KmeansID<<" Cluster"<<i<<endl;
		outfile<<"Check Kmeans"<<m_KmeansID<<" Cluster"<<i<<endl;
		
		if(IsClusterOK(i))                               //合格
		{
			cout<<"Level"<<m_ClusterLevel<<" Kmeans"<<m_KmeansID<<" Cluster"<<i<<" OK !"<<endl;
			cout<<"------------------------------"<<endl;
			
			outfile<<"Level"<<m_ClusterLevel<<" Kmeans"<<m_KmeansID<<" Cluster"<<i<<" OK !"<<endl;
			outfile<<"------------------------------"<<endl;			
		}
		else                                            //不合格，继续聚类
		{
			cout<<"Level"<<m_ClusterLevel<<" Kmeans"<<m_KmeansID<<" Cluster"<<i<<" need go on !"<<endl;
			outfile<<"Level"<<m_ClusterLevel<<" Kmeans"<<m_KmeansID<<" Cluster"<<i<<" need go on !"<<endl;			
			PrintClusterLabel(i);
			NextKvalue = GetDiffLabelofCluster(i);
			
			cout<<"------------------------------"<<endl;	
			cout<<"Set K = "<<NextKvalue<<endl;

			outfile<<"------------------------------"<<endl;	
			outfile<<"Set K = "<<NextKvalue<<endl;

			//如果不停止递归过程,继续创建新的Kmeans对象进行聚类
			if(!IsStopRecursion(i))     
			{
				pTmpClustList = GetClusterList(i);     //获得该Cluster数据链表
				NextLevel = m_ClusterLevel+1;		       //计算下一个Kmeans的层次Level
				//创建一个新的CKMeans对象
				CKMeans* pKMeans;
				pKMeans = new CKMeans(pClusterNode[i],pClusterTree,++KmeansID,NextLevel,19,pTmpClustList);
				pKMeans->RunKMeans(NextKvalue);		
			}
		  else
		  {
		  	cout<<"------------------------------"<<endl;	
		  	cout<<"Stop Recursion! Go back !"<<endl;
		  	outfile<<"------------------------------"<<endl;
		  	outfile<<"Stop Recursion! Go back !"<<endl;
		  }
		}
	}	
}

