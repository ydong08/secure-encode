#include "Kmeans.h"

/*****************************************************************************************
********************************* 读取测试文件函数 ***************************************
*****************************************************************************************/
bool ReadTestFile(list <strMyRecord*>* pRecordList)
{
	FILE *pInFile;                                     //记录文件的指针
	int iTestCount;                                    //检测文件中的数据数目
	char szBuf[MAX_BUF_SIZE];                          //读文件缓存  MAX_BUF_SIZE = 512
	strMyRecord* pRecord;                              //遍历记录的指针

	string strTemp;
	char szSeps[] = ",\r\n";
	
	cout<<"Start reading records from test file!"<<endl;
	outfile<<"Start reading records from test file!"<<endl;	
	
	if ((pInFile = fopen("corrected_datatreat", "r")) == NULL)
	{
		cout<<"Open Test file faied !"<<endl;
		return false;
	}
	else
	{
		iTestCount = 0;
		while (fgets(szBuf, MAX_BUF_SIZE, pInFile) != NULL)
		{
			if(++iTestCount%10000 == 0)
				cout<<setiosflags(ios::left)<<setw(8)<<iTestCount<<"  lines have read ..."<<endl;

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
		  pRecordList->push_back(pRecord);
		}
		
		cout<<iTestCount<<" Records have read from test file!"<<endl;			
		outfile<<iTestCount<<" Records have read from test file!"<<endl;		
		return true;
	}
}

/*****************************************************************************************
**************************** 将Label从int类型转换为string类型 ****************************
*****************************************************************************************/
string LabelInttoStr(int i)
{
	 string strLabel;
   switch(i)
   {
    	case 0:
    		strLabel = "normal";
    		break;
    	case 1:
    		strLabel = "dos";
    		break;
    	case 2:
    		strLabel = "probe";
    		break;
    	case 3:
    		strLabel = "ur2";
    		break;
    	case 4:
    		strLabel = "r2l";
    		break;
    	default:
    		strLabel = "!!ERROR!!";
    		break;    		
    }
    return strLabel;	
}

/*****************************************************************************************
**************************************** Main函数 **************************************** 
*****************************************************************************************/
int main()
{		
	int Kvalue;                                        //K值
	int iRightRcdNum;                                  //记录分类正确的记录数
	int TestRcdNum;                                    //参加检测的记录总数
	string strTrueLabel,strPreLabel;                   //真实标签与预测标签
	
	strMyRecord* pRecord;                              //遍历记录的指针
	list <strMyRecord*>* pTestRcdList;                 //测试数据链表
	list<strMyRecord*>::iterator TestListIter;         //记录链表的迭代器

	ClusterNode* pClusterNode;	                       //聚类节点指针
	ClusterTree* pClusterTree;                         //聚类树
	
	/*************************************************************************************
	*********** PART1 利用K-Means算法，对训练数据集进行迭代聚类，并创建聚类树 ************
	*************************************************************************************/
	
	outfile<<"Start K-means clustering proccess ..."<<endl;
	//创建聚类树
	pClusterTree = new ClusterTree();
	//创建KMeans聚类对象
	CKMeans m_CKMeans(pClusterTree,++KmeansID,1,19);
	//读取训练集中的记录
	if(!m_CKMeans.ReadTrainingRecords())
	{
		cout<<"Open training file failed !"<<endl;
		outfile<<"Open training file failed !"<<endl;
		exit(0);
	}
	else
	{	
		cout<<"----------------------------------------------"<<endl;
		cout<<"Read training file successfully !"<<endl;
		outfile<<"--------------------------------------"<<endl;
		outfile<<"Read training file successfully !"<<endl;
				
    /*用户输入K值
	  cout<<"Please input K value:"<<endl;
	  cin>>Kvalue;
	  */
	  //将K值直接设置成5
	  Kvalue = MAXLABEL;
	  
	  //将K值记录到日志文件中	  
	  outfile<<"Init K-value ="<<Kvalue<<endl;
	  
	  //运行Kmeans算法 
    m_CKMeans.RunKMeans(Kvalue);
    
    cout<<"****************** Total Clustering process finished ! *******************"<<endl;
    outfile<<"****************** Total Clustering process finished ! *******************"<<endl;
    
    //打印聚类树
    //pClusterTree->Print();
    
    //将聚类书打印至日志文件中
    pClusterTree->PrintLog();
  }
  
	/****************************************************************************************
	******* PART2 利用聚类树 对测试数据集中的数据进行类型预测，并计算出准确率和混淆矩阵******
	****************************************************************************************/  
  
  cout<<"************* Start classify the test records **************"<<endl;
  outfile<<"************* Start classify the test records **************"<<endl;
  
  //读取测试文件
  pTestRcdList = new list <strMyRecord*>;
  if(!ReadTestFile(pTestRcdList))
  {
		cout<<"Open Test file failed !"<<endl;
		outfile<<"Open Test file failed !"<<endl;
		exit(0);  	
  }
  else
  {
  	cout<<"*****************************************************************"<<endl;
  	cout<<"Start classifying test records ... "<<endl;
  	outfile<<"*****************************************************************"<<endl;
  	outfile<<"Start classifying test records ... "<<endl;
  	Rstfile<<"********************** Classification Result **************************"<<endl;
  	//创建一个混淆矩阵
  	ConfuseMatrix m_CfsMatrix;
  	//预测正确的记录数设置为0
  	iRightRcdNum = 0;
  	//将检测记录数设置为0
  	TestRcdNum = 0;
    //遍历测试数据链表，为每一个数据找到聚类中心
    for(TestListIter = pTestRcdList->begin();TestListIter != pTestRcdList->end();TestListIter++)
    {
    	pRecord = (*TestListIter);
    	pClusterNode = pClusterTree->FindNearestCluster(pRecord);
    	
    	//统计分类正确的记录数
    	if(pRecord->iLabel == pClusterNode->GetClusterNodeLabel())
    	{
    		iRightRcdNum++;
    	}
    	
    	//更新混淆矩阵
    	m_CfsMatrix.UpdateValue(pRecord->iLabel,pClusterNode->GetClusterNodeLabel());
    	
    	//将分类记录写到result.txt文件中
    	strTrueLabel = LabelInttoStr(pRecord->iLabel);
    	strPreLabel = LabelInttoStr(pClusterNode->GetClusterNodeLabel());
    	Rstfile<<setiosflags(ios::left)<<"True Label = "<<setw(8)<<strTrueLabel<<" Pre Label = "<<setw(8)<<strPreLabel<<" Cluster Path = "<<pClusterNode->strPath<<endl;
      
      //打印屏显
      if((++TestRcdNum)%10000 == 0)
      {
      	cout<<TestRcdNum<<" records have been done ..."<<endl;
      	outfile<<TestRcdNum<<" records have been done ..."<<endl;
      }
    }
    
    cout<<"****** The process of classifying test records  finished ! ******"<<endl;
    outfile<<"****** The process of classifying test records  finished ! ******"<<endl;
        
    //计算正确分类率
    cout<<"================================== Classify Result ===================================="<<endl;
    Rstfile<<"================================== Classify Result ===================================="<<endl;
    cout<<"Total Test Records = "<<TestRcdNum<<"  Right_Label Records = "<<iRightRcdNum<<" Right Rate = "<<(float)(iRightRcdNum)/TestRcdNum<<endl;
    Rstfile<<"Total Test Records = "<<TestRcdNum<<"  Right_Label Records = "<<iRightRcdNum<<" Right Rate = "<<(float)(iRightRcdNum)/TestRcdNum<<endl;
    
    //打印混淆矩阵    
    m_CfsMatrix.PrintMatrix();
    m_CfsMatrix.PrintMatrixToLog();
  }
}
