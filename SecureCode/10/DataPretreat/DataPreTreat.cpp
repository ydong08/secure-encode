#include "DataPreTreat.h"

int main(int argc,char* argv[])
{
	/************************************** 变量声明 ********************************************/
	
	const char* Datatreat = "_datatreat";                     //预处理后的文件后缀
	char* InputFileName,*OutputFileName;                      //预处理输入文件名，输出文件名
	FILE *pInFile, *pOutFile;                                 //输入文件与输出文件的指针
	
	int i;
  int iRcdCount = 0;                                        //数据记录的数目	

	char szBuf[MAX_BUF_SIZE];                                 //MAX_BUF_SIZE = 512	
	strMyRecord* pRecord;                                     //遍历数据记录的指针
	
	list <strMyRecord*> RecordList;                           //数据记录链表
	list<strMyRecord*>::iterator RecdIter;                    //遍历记录链表的迭代器
	
	string strTmp,strTmp2;
	string StrRecord, StrTemp;
	char szSeps[] = ",\r\n";
	
	
	//判断main函数参数是否正确
	if (argc != 2) 
	{
		cout<<"Parameter error !"<<endl;
		return -1;
	}
	
	/********************************* 打开预处理输入文件 **************************************/
	InputFileName = new char[30];
	strcpy(InputFileName,argv[1]);
	
	if ((pInFile = fopen(InputFileName, "r")) == NULL)              //kddcup99.data_10000
	{
		cout<<"Open training-data file faied !"<<endl;
		return -1;
	}
	
	/********************************* 打开预处理输出文件 **************************************/
	//拼写预处理工作完成后的输出文件名，即在打开的输入文件后加上后缀_datatreat
	OutputFileName = strcat(InputFileName,Datatreat);

	if ((pOutFile = fopen(OutputFileName, "w")) == NULL)   //kddcup99.data_10000_datatreat
	{
		fclose(pInFile);
		cout<<"Creat Data-preterat file failed!"<<endl;
		return -1;
	}
	
	/*********************** 第一部分 循环读取数据,并进行过滤************************************
	1、读取文件中的每一条记录并插入链表
	2、选择对聚类有益的属性，放弃不重要的属性
	*********************************************************************************************/
  cout<<"Start reading records from "<<argv[1]<<"..."<<endl;
  
	while (fgets(szBuf, MAX_BUF_SIZE, pInFile) != NULL)
	{
		//每隔1000 行显示工作进度
		if (++iRcdCount%10000 == 0)
			cout<<setiosflags(ios::left)<<"---------- "<<setw(8)<<iRcdCount<<" lines have read ----------"<<endl;
		
		//创建新的记录节点
		pRecord = new strMyRecord;
		
		/********* 保留选出的20列，其中百分比数值全部转化为百分数，标签列合并为5类 ***********/
		
		//第1列不要
		strtok(szBuf, szSeps);
		
		//第2列：协议类型
		strTmp = strtok(NULL, szSeps);
		//将Protocol属性的符号类型字段改为枚举类型字段
		if(strTmp == "icmp")
			pRecord->iProtocolType = 1;
		else if(strTmp == "tcp")
			pRecord->iProtocolType = 2;
		else if(strTmp == "udp")
			pRecord->iProtocolType = 3;
		else
			pRecord->iProtocolType = 4;
		
		
		//第3列：服务类型
		strTmp = strtok(NULL, szSeps);
		//将Service属性的符号类型字段改为枚举类型字段
		if(strTmp == "domain_u")
			pRecord->iService = 1;
		else if(strTmp == "ecr_i")
			pRecord->iService = 2;
		else if(strTmp == "eco_i")
			pRecord->iService = 3;
		else if(strTmp == "finger")
			pRecord->iService = 4;
		else if(strTmp == "ftp_data")
			pRecord->iService = 5;
		else if(strTmp == "ftp")
			pRecord->iService = 6;
		else if(strTmp == "http")
			pRecord->iService = 7;
		else if(strTmp == "hostnames")
			pRecord->iService = 8;
		else if(strTmp == "imap4")
			pRecord->iService = 9;
		else if(strTmp == "login")
			pRecord->iService = 10;
		else if(strTmp == "mtp")
			pRecord->iService = 11;
		else if(strTmp == "netstat")
			pRecord->iService = 12;
		else if(strTmp == "other")
			pRecord->iService = 13;
		else if(strTmp == "private")
			pRecord->iService = 14;
		else if(strTmp == "smtp")
			pRecord->iService = 15;
		else if(strTmp == "systat")
			pRecord->iService = 16;
		else if(strTmp == "telnet")
			pRecord->iService = 17;
		else if(strTmp == "time")
			pRecord->iService = 18;
		else if(strTmp == "uucp")
			pRecord->iService = 19;
		else
			pRecord->iService = 20;

    		
		//第4列：状态标志
		strTmp = strtok(NULL, szSeps);
		//将StatusFlag属性的符号类型字段改为枚举类型字段
		if(strTmp == "REJ")
			pRecord->iStatusFlag = 1;
		else if(strTmp == "RSTO")
			pRecord->iStatusFlag = 2;
		else if(strTmp == "RSTR")
			pRecord->iStatusFlag = 3;
		else if(strTmp == "S0")
			pRecord->iStatusFlag = 4;
		else if(strTmp == "S3")
			pRecord->iStatusFlag = 5;
		else if(strTmp == "SF")
			pRecord->iStatusFlag = 6;
		else if(strTmp == "SH")
			pRecord->iStatusFlag = 7;
		else
			pRecord->iStatusFlag = 8;
		

		//第5列：源到目的字节数
		strTmp = strtok(NULL, szSeps);
		pRecord->iSrcBytes = atoi(strTmp.c_str());
		//第6列：目的到源字节数
		strTmp = strtok(NULL, szSeps);
		pRecord->iDestBytes = atoi(strTmp.c_str());
		//第7、8、9、10列不要
		for (i=0; i<4; i++)
			strtok(NULL, szSeps); // 7-10
		//第11列：登录失败次数
		strTmp = strtok(NULL, szSeps);
		pRecord->iFailedLogins = atoi(strTmp.c_str());
		//第12、13列不要
		strtok(NULL, szSeps); // 12
		strtok(NULL, szSeps); // 13
		//第14列：是否获得root用户权限
		strTmp = strtok(NULL, szSeps);
		pRecord->bRootShell = atoi(strTmp.c_str());
		//第15列不要
		strtok(NULL, szSeps); // 15
		//第16列：root用户权限存取次数
		strTmp = strtok(NULL, szSeps);
		pRecord->iNumofRoot = atoi(strTmp.c_str());
		//第17、18、19、20、21列不要
		for (i=0; i< 5; i++)
			strtok(NULL, szSeps); // 17-21 
		//第22列：是否是guest用户登录
		strTmp = strtok(NULL, szSeps);
		pRecord->bGuestLogin = atoi(strTmp.c_str());
		//第23列：2秒内连接相同主机数目
		strTmp = strtok(NULL, szSeps);
		pRecord->iCount = atoi(strTmp.c_str());
		//第24列：2秒内连接相同端口数目
		strTmp = strtok(NULL, szSeps);
		pRecord->iSrvCount = atoi(strTmp.c_str());
		//第25、26列不要
		strtok(NULL, szSeps); // 25
		strtok(NULL, szSeps); // 26
		//第27列："REJ"错误的连接数比率
		strTmp = strtok(NULL, szSeps);
		//把0~1的小数，转换成0到100之间的整数
		strTmp[1] = strTmp[2]; 
		strTmp[2] = strTmp[3];
		strTmp[3] = '\0';
		pRecord->iRerrorRate = atoi(strTmp.c_str());

		//第28列不要
		strtok(NULL, szSeps); // 28
		//第29列：连接到相同端口数比率
		strTmp = strtok(NULL, szSeps);	
		strTmp[1] = strTmp[2]; 
		strTmp[2] = strTmp[3];
		strTmp[3] = '\0';
		pRecord->iSameSrvRate = atoi(strTmp.c_str());

		
		//第30列：连接到不同端口数比率
		strTmp = strtok(NULL, szSeps);
		strTmp[1] = strTmp[2]; 
		strTmp[2] = strTmp[3];
		strTmp[3] = '\0';
		pRecord->iDiffSrvRate = atoi(strTmp.c_str());
		
		//第31、32列不要
		strtok(NULL, szSeps); // 31
		strtok(NULL, szSeps); // 32
		//第33列：相同目的地相同端口连接数
		strTmp = strtok(NULL, szSeps);
		pRecord->iDstHostSrvCount = atoi(strTmp.c_str());
		//第34列：相同目的地相同端口连接数比率
		strTmp = strtok(NULL, szSeps);				
		strTmp[1] = strTmp[2]; 
		strTmp[2] = strTmp[3];
		strTmp[3] = '\0';
		pRecord->iDstHostSameSrvRate = atoi(strTmp.c_str());

		
		//第35列：相同目的地不同端口连接数比率
		strTmp = strtok(NULL, szSeps);		
		strTmp[1] = strTmp[2]; 
		strTmp[2] = strTmp[3];
		strTmp[3] = '\0';
		pRecord->iDstHostDiffSrvRate = atoi(strTmp.c_str());		
		
		//第36列：相同目的地相同源端口连接比率
		strTmp = strtok(NULL, szSeps);	  
	  strTmp[1] = strTmp[2]; 
		strTmp[2] = strTmp[3];
		strTmp[3] = '\0';
		pRecord->iDstHostSameSrcPortRate = atoi(strTmp.c_str());
		
		//第37列：不同主机连接相同端口比率
		strTmp = strtok(NULL, szSeps);		
	  strTmp[1] = strTmp[2]; 
		strTmp[2] = strTmp[3];
		strTmp[3] = '\0';
		pRecord->iDstHostSrvDiffHostRate = atoi(strTmp.c_str());	
				
		//第38列不要
		strtok(NULL, szSeps); // 38
		//第39列：连接当前主机有S0错误的比率
		strTmp = strtok(NULL, szSeps);		
	  strTmp[1] = strTmp[2]; 
		strTmp[2] = strTmp[3];
		strTmp[3] = '\0';
		pRecord->iDstHostSrvSerrorRate = atoi(strTmp.c_str());
		
		//第40、41列不要
		strtok(NULL, szSeps); // 40
		strtok(NULL, szSeps); // 41
		//第42列：类型标签 归为5类
		strTmp.clear();
		strTmp = strtok(NULL, szSeps);
				
		if (strTmp == "normal.")
			pRecord->iLabel = 0; //normal
		else if ((strTmp == "smurf.") || (strTmp == "neptune.") || (strTmp == "back.") || (strTmp == "teardrop.") || (strTmp == "pod.") || (strTmp == "land."))
			pRecord->iLabel = 1; //dos
		else if ((strTmp == "satan.") || (strTmp == "ipsweep.") || (strTmp == "portsweep.") || (strTmp == "nmap."))
			pRecord->iLabel = 2; //probe
		else if ((strTmp == "buffer_overflow.") || (strTmp == "rootkit.") || (strTmp == "loadmodule.") || (strTmp == "perl."))
				pRecord->iLabel = 3; //u2r
		else if ((strTmp == "ftp_write.") || (strTmp == "guess_passwd.") || (strTmp == "imap.") || (strTmp == "multihop.") || (strTmp == "phf.") || (strTmp == "spy.") || (strTmp == "warezclient.") || (strTmp == "warezmaster."))
				pRecord->iLabel = 4; //r2l
		    
		//在链表尾部插入记录
		RecordList.push_back(pRecord);	
	}
	
	//文件读取完毕，显示记录数	
	cout<<"All Records have read ! Total "<<iRcdCount<<" records !"<<endl;
	

         
  /*********************** 第二部分 遍历链表，将记录写入输出文件中 ************************************/
  cout<<"Start writing records into "<<OutputFileName;
  cout<<" ..."<<endl;
  
	//遍历链表，将每一个条处理后的记录都写入预处理后的文件
	for (i=0,RecdIter = RecordList.begin(); RecdIter != RecordList.end(); RecdIter++)
	{
		if (++i%10000 == 0) //&& (IsPrintLog == true))
			cout<<setiosflags(ios::left)<<"---------- "<<setw(8)<<i<<"lines have written ----------"<<endl;
		
		//第1列（原2）：协议类型
		StrTemp = intToString(int((*RecdIter)->iProtocolType));
		StrRecord += StrTemp;
		StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();
		
		//第2列（原3）：服务类型
		StrTemp = intToString(int((*RecdIter)->iService));
		StrRecord += StrTemp;
		StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();	
		
		//第3列（原4）：状态标志
		StrTemp = intToString(int((*RecdIter)->iStatusFlag));
		StrRecord += StrTemp;
		StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();			
		
		//第4列（原5）：源到目的字节数
		StrTemp = intToString((*RecdIter)->iSrcBytes);
		StrRecord += StrTemp;
		StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();
		
		//第5列（原6）：目的到源字节数
		StrTemp = intToString((*RecdIter)->iDestBytes);
		StrRecord += StrTemp;
		StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();
		
		//第6列（原11）：登录失败次数
		StrTemp = intToString((*RecdIter)->iFailedLogins);
		StrRecord += StrTemp;
		StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();		
		
		//第7列（原14）：是否获得root用户权限
		StrTemp = intToString(int((*RecdIter)->bRootShell));
		StrRecord += StrTemp;
		StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();
		
		//第8列（原16）：root用户权限存取次数
		StrTemp = intToString((*RecdIter)->iNumofRoot);
		StrRecord += StrTemp;
		StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();

		//第9列（原22）：是否是guest用户登录
		StrTemp = intToString(int((*RecdIter)->bGuestLogin));
		StrRecord += StrTemp;
		StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();

		//第10列（原23）：2秒内连接相同主机数目
		StrTemp = intToString((*RecdIter)->iCount);
		StrRecord += StrTemp;
		StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();

		//第11列（原24）：2秒内连接相同端口数目
		StrTemp = intToString((*RecdIter)->iSrvCount);
		StrRecord += StrTemp;
		StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();
		
		//第12列（原27）："REJ"错误的连接数比率
		StrTemp = intToString(int((*RecdIter)->iRerrorRate));
		StrRecord += StrTemp;
		StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();
		
		//第13列（原29）：连接到相同端口数比率
		StrTemp = intToString(int((*RecdIter)->iSameSrvRate));
		StrRecord += StrTemp;
		StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();
		
		//第14列（原30）：连接到不同端口数比率
		StrTemp = intToString(int((*RecdIter)->iDiffSrvRate));
		StrRecord += StrTemp;
		StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();

		//第15列（原33）：相同目的地相同端口连接数
		StrTemp = intToString((*RecdIter)->iDstHostSrvCount);
		StrRecord += StrTemp;
		StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();
		
		//第16列（原34）：相同目的地相同端口连接数比率
		StrTemp = intToString(int((*RecdIter)->iDstHostSameSrvRate));
		StrRecord += StrTemp;
		StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();
		
		//第17列（原35）：相同目的地不同端口连接数比率
		StrTemp = intToString(int((*RecdIter)->iDstHostDiffSrvRate));
		StrRecord += StrTemp;
		StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();
		
		//第18列（原36）：相同目的地相同源端口连接比率
		StrTemp = intToString(int((*RecdIter)->iDstHostSameSrcPortRate));
		StrRecord += StrTemp;
		StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();
		
		//第19列（原37）：不同主机连接相同端口比率
		StrTemp = intToString(int((*RecdIter)->iDstHostSrvDiffHostRate));
		StrRecord += StrTemp;
		StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();
		
		//第20列（原39）：连接当前主机有S0错误的比率
		StrTemp = intToString(int((*RecdIter)->iDstHostSrvSerrorRate));
		StrRecord += StrTemp;
		StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();		

		//第21列（原42）：类型标签 归为5类
		StrTemp = intToString(int((*RecdIter)->iLabel));
		StrRecord += StrTemp;
		//StrRecord += ",";
		//cout<<"Record: "<<StrRecord<<endl;
		StrTemp.clear();		
    
    StrRecord += "\n";
    //cout<<"Record: "<<StrRecord<<endl;
    
    //将记录写入预处理后的文件中
    fputs(StrRecord.c_str(),pOutFile);
    
		//将StrRecord字符串清空
		StrRecord.clear();		
	}
	
	//文件读取完毕，显示记录数	
	cout<<"All Records have written !"<<endl;
}

