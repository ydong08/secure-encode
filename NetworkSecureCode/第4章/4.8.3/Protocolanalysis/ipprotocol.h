//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------// 
struct IP
{
	unsigned char HeaderLength : 4, Version : 4;
	unsigned char Tos;
	unsigned short Length;
	unsigned short Ident;
	unsigned short Flags_Offset;
	unsigned char TTL;
	unsigned char Protocol;
	unsigned short Checksum;
	unsigned int SourceAddr;
	unsigned int DestinationAddr;
};
struct IpHeader
{
	char Version[100];
	char HeaderLength[100];
	char Tos[100];
	char Length[100];
	char Ident[100];
	char Flags_Offset[100];
	char TTL[100];
	char Protocol[100];
	char Checksum[100];
	char SourceAddr[100];
	char DestinationAddr[100];
};
void IPOperation(const unsigned char*);
