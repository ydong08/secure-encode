#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <list>
#include <stdlib.h>

using namespace std;

#define MAX_BUF_SIZE 512
typedef unsigned char BYTE;

/********************** 全局函数intToString ******************
//将一个Int类型的数转换成一个string
*************************************************************/
string intToString(int i)
{
	stringstream s;
	s << i;
	return s.str();
}

/***************** 数据记录的结构体定义 **********************/
struct strMyRecord
{
	BYTE iProtocolType;	                 //02 协议类型：				符号变量 3类：icmp tcp udp
	BYTE iService;		                   //03 服务类型：				符号变量 66类
	BYTE iStatusFlag;	                   //04 状态标志：				符号变量 11类
	int  iSrcBytes;		                   //05 源到目的字节数：		连续变量
	int  iDestBytes;	                   //06 目的到源字节数：		连续变量
	int  iFailedLogins;	                 //11 登录失败次数：			连续变量 //6个：0 ~ 5 
	BYTE bRootShell;	                   //14 是否获得root用户权限：	二值变量
	int  iNumofRoot;	                   //16 root用户权限存取次数：	连续变量
	BYTE bGuestLogin;	                   //22 是否是guest用户登录：	二值变量
	int  iCount;		                     //23 2秒内连接相同主机数目：连续变量
	int  iSrvCount;		                   //24 2秒内连接相同端口数目：连续变量
	BYTE iRerrorRate;	                   //27 "REJ"错误的连接数比率：连续变量：0 ~ 100
	BYTE iSameSrvRate;	                 //29 连接到相同端口数比率： 连续变量：0 ~ 100
	BYTE iDiffSrvRate;	                 //30 连接到不同端口数比率： 连续变量：0 ~ 100
	int  iDstHostSrvCount;			         //33 相同目的地相同端口连接数：		连续变量
	BYTE iDstHostSameSrvRate;		         //34 相同目的地相同端口连接数比率：	连续变量：0 ~ 100
	BYTE iDstHostDiffSrvRate;		         //35 相同目的地不同端口连接数比率：	连续变量：0 ~ 100
	BYTE iDstHostSameSrcPortRate;	       //36 相同目的地相同源端口连接比率：	连续变量：0 ~ 100
	BYTE iDstHostSrvDiffHostRate;	       //37 不同主机连接相同端口比率：		连续变量：0 ~ 100
	BYTE iDstHostSrvSerrorRate;		       //39 连接当前主机有S0错误的比率：	连续变量：0 ~ 100
	BYTE iLabel;		                     //42 类型标签：符号变量 5类
};
