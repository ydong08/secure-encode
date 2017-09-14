//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
struct ARP
{
	unsigned short Hardware;  	 
	unsigned short Protocol;  	
	unsigned char HardwareLength;   
	unsigned char ProtocolLength;   
	unsigned short OperationCode;   
};
struct arpheader
{
	char Hardware[100];
	char Protocol[100];
	char HardwareLength[100];
	char ProtocolLength[100];
	char OperationCode[100];
	char OperationInformatin[100];
};
void ARPOperation(const unsigned char* packetdata);
 