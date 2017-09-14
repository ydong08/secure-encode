#ifndef COMMOM_H 
#define COMMOM_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <list>
#include <string>
#include <valarray>                   // for valarray
#include <cmath>                      // for sqrt() and pow()
#include <sstream>


using namespace std;

#define  MAX_BUF_SIZE 512             //字符最大缓存树
#define	 MAXDIMENSION	20              //聚类最大维数
#define	 MAXCLUSTER		10              //最大聚类数
#define  MAXLABEL  5                  //最大标签数
#define  CLUSTER_PRECISION  0.1       //聚类精度设置为10%
#define  MAXLEVEL  8                  //最大的聚类层数
#define  INTERLEVEL  3                //最大的聚类层数

/********************** 记录的结构 *************************/
struct strMyRecord
{
	float fProtocolType;                //02 协议类型
	float fService;                     //03 服务类型
	float fStatusFlag;                  //04 状态标志
	float fSrcBytes;                    //05 源到目的字节数
	float fDestBytes;                   //06 目的到源字节数
	float fFailedLogins;                //11 登录失败次数
	float fNumofRoot;                   //16 root用户权限存取次数
	float fCount;                       //23 2秒内连接相同主机数目
	float fSrvCount;                    //24 2秒内连接相同端口数目
	float fRerrorRate;                  //27 "REJ"错误的连接数比率
	float fSameSrvRate;                 //29 连接到相同端口数比率
	float fDiffSrvRate;                 //30 连接到不同端口数比率
	float fDstHostSrvCount;             //33 相同目的地相同端口连接数
	float fDstHostSameSrvRate;          //34 相同目的地相同端口连接数比率
	float fDstHostDiffSrvRate;          //35 相同目的地不同端口连接数比率
	float fDstHostSameSrcPortRate;      //36 相同目的地相同源端口连接比率
	float fDstHostSrvDiffHostRate;      //37 不同主机连接相同端口比率
	float fDstHostSrvSerrorRate;        //39 连接当前主机有S0错误的比率
	int   iLabel;                       //42 类型标签
};

#endif
