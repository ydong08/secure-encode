//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------// 
#include "stdafx.h"
#include "ProtocolanalysisDlg.h"
#include "sniffer.h"
#include "ethernetprotocol.h"
#include "devicedialog.h"
int network_device;
char network_device_name[1024][1024];
int network_device_number;
bool gSaveDumpFile=false;
bool gShowPacketData=true;
typedef void (*pcap_func_t)(unsigned char*, const struct pcap_pkthdr*, const u_char*);
extern char PacketFilter[1024];
void ProcessProtocolPacket(unsigned char* user, const struct pcap_pkthdr* h,
						   const unsigned char* packetdata)
{	
	if(gSaveDumpFile)
	{
		pcap_dump(user, h, packetdata);
	}
	if(gShowPacketData)
	{
		EthernetOperation(packetdata);
	}
}

void ProcessProtocolPacketFromDumpFile(unsigned char* user, const struct pcap_pkthdr* h,
									   const unsigned char* packetdata)
{	
	EthernetOperation(packetdata);
}
PacketOperation()
{
	pcap_if_t *NetworkDevice;
	pcap_if_t *DeviceIndex;
	int number = -1;  
	pcap_t* PcapHandle;
	u_int DeviceId;
	char Error[PCAP_ERRBUF_SIZE];
	char CaptureFilter[1024];
	pcap_handler Handler;
	bpf_u_int32 SubNet, NetMask;
	struct bpf_program FilterCode;	
	pcap_dumper_t *PcapFile;
	strcpy(CaptureFilter, PacketFilter);
	if (pcap_findalldevs(&NetworkDevice, Error) == -1)
	{
		AfxMessageBox(Error);
		exit(1);
	}
	int i=0;
	network_device_number=0;
	for(DeviceIndex=NetworkDevice; DeviceIndex; DeviceIndex=DeviceIndex->next)
    {
		sprintf(network_device_name[i],"%s",DeviceIndex->name);
		++i;
		network_device_number++;
    }
	if (i == 0)
	{
		return -1;
	}
	CDeviceDialog dia;
	int result =dia.DoModal ();
	if(result==IDOK)
	{	
	}
	else
	{
		return -1;
	}
	DeviceId = network_device;
	if (DeviceId <1 || DeviceId> i)
	{
		pcap_freealldevs(NetworkDevice);
		return -1;
	}
	for (DeviceIndex = NetworkDevice, i = 0; i < DeviceId - 1 ; DeviceIndex = DeviceIndex->next, i++)
		;
	if ((PcapHandle = pcap_open_live(DeviceIndex->name, 65536, 1, 1000, Error)) == NULL)
	{
		return -1;
	}
	char* Interface = NULL;
    PcapFile = pcap_dump_open(PcapHandle, "d:\\test1.pcap");
	if (CaptureFilter != NULL)
	{
		if (Interface != NULL)
		{
			if (pcap_lookupnet(Interface, &SubNet, &NetMask, Error) < 0)
			{
				return 0;
			}
		}
		else
			NetMask = 0xffffff;
		if (pcap_compile(PcapHandle, &FilterCode, CaptureFilter, 1, NetMask) < 0)
		{
			return 0;
		} 
		if (pcap_setfilter(PcapHandle, &FilterCode) < 0)
		{
			return 0;
		}
	}
	Handler = (pcap_func_t) ProcessProtocolPacket;
	pcap_loop(PcapHandle, number, Handler, (unsigned char *)PcapFile);
	return 0;
}

DumpFileOperation()
{
	int number = -1;  
	pcap_t* PcapHandle;
	u_int i = 0;
	char Error[PCAP_ERRBUF_SIZE];
	char* Interface = NULL;
	char CapFilter[1024];
	strcpy(CapFilter, PacketFilter);
	pcap_handler Handler;
	bpf_u_int32 SubNet, NetMask;
	struct bpf_program FilterCode;
	CString pcap_file_name;
	LPCTSTR szTypes =_T("tcpdump Files (*.pcap)|*.pcap|")
		_T("libpcap Files (*.cap)|*.cap|")
		_T("All Files (*.*)|*.*||");
	CFileDialog dlg( TRUE, _T("pcap"), NULL, OFN_HIDEREADONLY, szTypes );
	int result=dlg.DoModal ();
	if(result==IDOK)
	{
		pcap_file_name=dlg.GetFileName ();
	}
	else
	{
		return -1;
	}
	PcapHandle=pcap_open_offline(pcap_file_name,Error);
	if(PcapHandle==NULL)
	{
		AfxMessageBox(Error);
		return -1;
	}
	if (CapFilter != NULL)
	{
		if (Interface != NULL)
		{
			if (pcap_lookupnet(Interface, &SubNet, &NetMask, Error) < 0)
			{
				return 0;
			}
		}
		else
			NetMask = 0xffffff;
		if (pcap_compile(PcapHandle, &FilterCode, CapFilter, 1, NetMask) < 0)
		{
			return 0;
		} 
		if (pcap_setfilter(PcapHandle, &FilterCode) < 0)
		{
			return 0;
		}
	}
	Handler = (pcap_func_t) ProcessProtocolPacketFromDumpFile;
	pcap_loop(PcapHandle, number, Handler, NULL);
	return 0;
}
