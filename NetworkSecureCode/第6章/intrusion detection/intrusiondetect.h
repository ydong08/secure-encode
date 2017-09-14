//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//

struct ids
{

	int protocoltype;

	int port;
};
struct IntrusionInfor
{
	char time[100];
	char attackip[100];

	char information[1024];
};

int IntrusionDetect(void);