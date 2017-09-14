//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    // 
//--------------------------------------------------------------------//
struct ETHERNET
{
	unsigned char DestinationMac[6];
	unsigned char SourceMac[6];
	unsigned short NetType;
};
struct EtherHeader
{
	char DestinationMac[1024];
	char SourceMac[1024];
	char NetType[1024];
};
int EthernetOperation(const unsigned char* p);





