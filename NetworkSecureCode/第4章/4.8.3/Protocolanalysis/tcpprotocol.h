//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
struct TCP
{
	unsigned short SrcPort; 			
	unsigned short DstPort; 			 
	unsigned int SequenceNum;   			 
	unsigned int Acknowledgment;   			 
	unsigned char Zero : 4, 			  
					HdrLen : 4;   			
	unsigned char Flags;
	unsigned short  AdvertisedWindow;   		
	unsigned short  Checksum;   			
	unsigned short  UrgPtr;   			  
};
struct TcpHeader
{
	char SrcPort[100];			  
	char DstPort[100];			  
	char SequenceNum[100];  		
	char Acknowledgment[100];  		
	char Zero[100];   			
	char HdrLen[100];  			  
	char Flags[100];
	char AdvertisedWindow[100];  		
	char Checksum[100];  			 
	char UrgPtr[100];  			  
};
int TCPOperation(const unsigned char* packetdata);
