//==================================
//头文件
//==================================
#include "md5.h"
#include <iostream>

using namespace std;

//==================================
//功能函数
//==================================

//==================================
//Main函数
//==================================
int main(int argc,char* argv[])
{
	  char* pFilePath;                     //需要进行md5计算的文件路径
	  char* pMd5FilePath;                  //存放md5摘要的.md5文件路径
	  char  Md5Digest[33];                 //md5摘要，用于存放手动输入的md5摘要信息
	  char  Md5Record[50];                 //.md5文件中的一行记录
	  string strTmp;
	  
	  //======================================================================
	  //字符串定义
		char* pHelpMsg =    {"-h"};          // 帮助信息
		char* pTestMsg =    {"-t"};          // md5测试程序的应用信息
		char* pCompute =    {"-c"};          // 计算指定文件的md5摘要
		char* pMValidate =  {"-mv"};         // 手动对文件进行md5认证
		char* pfValidate =  {"-fv"};		     // 通过比较对文件的md5摘要进行认证
		char* pSpace = {" "};                // 定义空格
		
		//=======================================================================
		//参数检测
		if(argc<2 || argc>4)
		{
			cout<<"Parameter Error !"<<endl;
			return -1;
		}
		
		//=======================================================================
		//显示帮助信息
		if((argc == 2)&&(!strcmp(pHelpMsg,argv[1])))
		{
			cout<<"MD5 usage:  [-h]  --help information"<<endl;
			cout<<"            [-t]  --test MD5 application"<<endl;
			cout<<"            [-c]  [file path of the file computed]"<<endl;
			cout<<"                  --compute MD5 of the given file"<<endl;
			cout<<"            [-mv] [file path of the file validated]"<<endl;
			cout<<"                  --validate the integrality of a given file by manual input MD5 value"<<endl;
			cout<<"            [-fv] [file path of the file validated]  [file path of the .md5 file]"<<endl;
			cout<<"                  --validate the integrality of a given file by read MD5 value from .md5 file"<<endl;
		}
		
		//=======================================================================
		//显示md5应用程序的测试信息
		if((argc == 2)&&(!strcmp(pTestMsg,argv[1])))
		{
			cout<<"MD5(\"\") = "<<MD5("").toString()<<endl;
			cout<<"MD5(\"a\") = "<<MD5("a").toString()<<endl;
			cout<<"MD5(\"abc\") = "<<MD5("abc").toString()<<endl;
			cout<<"MD5(\"message digest\") = "<<MD5("message digest").toString()<<endl;
			cout<<"MD5(\"abcdefghijklmnopqrstuvwxyz\") = "<<MD5("abcdefghijklmnopqrstuvwxyz").toString()<<endl;
			cout<<"MD5(\"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\")"<<endl;
			cout<<"    = "<<MD5("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789").toString()<<endl;
			cout<<"MD5(\"12345678901234567890123456789012345678901234567890123456789012345678901234567890\")"<<endl;
			cout<<"    = "<<MD5("12345678901234567890123456789012345678901234567890123456789012345678901234567890").toString()<<endl;
		}
		
		//=======================================================================
		//计算指定文件的md5摘要，并显示出来
		if((argc == 3)&&(!strcmp(pCompute,argv[1])))
		{
			//如果没有文件路径，则参数出错
			if(argv[2] == NULL)
			{
				cout<<"Parameter Error ! Please input file path !"<<endl;
				return -1;				
			}
			else
			{
				pFilePath = argv[2];
			}
			//打开指定的文件
			ifstream File_1(pFilePath);
			//声明md5对象,并进行计算
			MD5 md5_obj1(File_1);
			//输出计算结果
			cout<<"MD5(\""<<argv[2]<<"\") = "<<md5_obj1.toString()<<endl;
		}
		
		//=======================================================================
		//手动进行文件完整性检测
		if((argc == 3)&&(!strcmp(pMValidate,argv[1])))
		{
			//如果没有文件路径，则参数出错
			if(argv[2] == NULL)
			{
				cout<<"Parameter Error ! Please input file path !"<<endl;
				return -1;				
			}
			else
			{
				pFilePath = argv[2];
			}
      //手动输入了被测文件的MD5摘要
			cout<<"Please input the MD5 value of file(\""<<pFilePath<<"\")..."<<endl;
     	cin>>Md5Digest;
     	//在摘要的字符串末尾加上结束符
     	Md5Digest[32] = '\0';
     	
			//打开指定的文件
			ifstream File_2(pFilePath);
			//声明md5对象,并进行计算
			//MD5 md5_obj2(File_2);
			MD5 md5_obj2;
			md5_obj2.reset();
			md5_obj2.update(File_2);
			
			//读取文件内容并计算MD5摘要
			strTmp = md5_obj2.toString();
			const char* pMd5Digest = strTmp.c_str();
			
			//输出两个摘要
			cout<<"The MD5 digest of file(\""<<pFilePath<<"\") which you input is: "<<endl;
			cout<<Md5Digest<<endl;
			cout<<"The MD5 digest of file(\""<<pFilePath<<"\") which calculate by program is: "<<endl;
			cout<<strTmp<<endl;
			
		  //比较摘要的结果是否相同
			if (strcmp(pMd5Digest,Md5Digest))
			{
				cout<<"Match Error! The file is not integrated!"<<endl;
			}
			else
			{
				cout<<"Match Successfully! The file is integrated!"<<endl;
			}
		}
		//=======================================================================
		//通过.md5文件进行文件完整性检测
		if((argc == 4)&&(!strcmp(pfValidate,argv[1])))
		{
			//如果没有文件路径，则参数出错
			if((argv[2] == NULL)||(argv[3] == NULL))
			{
				cout<<"Parameter Error ! Please input file path !"<<endl;
				return -1;				
			}
			else
			{
				pFilePath = argv[2];
				pMd5FilePath = argv[3];
			}
			
			//打开.md5文件
			ifstream File_3(pMd5FilePath);
			//读取.md5文件中的记录
			File_3.getline(Md5Record,50);
			
	    //以空格为标记,获得.MD5文件中的MD5值与对应文件名
	    char* pMd5Digest_f = strtok(Md5Record,pSpace);
	    char* pFileName_f = strtok(NULL,pSpace);
	   
		  //打开被测文件
		  ifstream File_4(pFilePath);
			//声明md5对象,并进行计算
			//MD5 md5_obj3(File_4);
			MD5 md5_obj3;
			md5_obj3.reset();
			md5_obj3.update(File_4);
					  
			//读取文件内容并计算MD5摘要
			strTmp = md5_obj3.toString();
			const char* pMd5Digest_c = strTmp.c_str();	   
			
			//输出两个摘要
			cout<<"The MD5 digest of file(\""<<pFileName_f<<"\") which is in file(\""<<pMd5FilePath<<"\") is: "<<endl;
			cout<<pMd5Digest_f<<endl;
			cout<<"The MD5 digest of file(\""<<pFilePath<<"\") which calculate by programme is: "<<endl;
			cout<<strTmp<<endl;
			
     //比较摘要，进行验证
	    if (strcmp(pMd5Digest_c,pMd5Digest_f)) 
			{
				cout<<"Match Error! The file is not integrated!"<<endl;
			}
			else
			{
				cout<<"Match Successfully! The file is integrated!"<<endl;
			}				
		}
    //函数返回
		return 0;
}
