Scaner:Scaner.o TCPConnectScan.o TCPSYNScan.o TCPFINScan.o UDPScan.o
	g++ -lpthread -o Scaner Scaner.o TCPConnectScan.o TCPSYNScan.o TCPFINScan.o UDPScan.o
Scaner.o:Scaner.cpp
	g++ -c Scaner.cpp
TCPConnectScan.o:TCPConnectScan.cpp
	g++ -c TCPConnectScan.cpp
TCPSYNScan.o:TCPSYNScan.cpp
	g++ -c TCPSYNScan.cpp
TCPFINScan.o:TCPFINScan.cpp
	g++ -c TCPFINScan.cpp
UDPScan.o:UDPScan.cpp
	g++ -c UDPScan.cpp
clean:
	rm Scaner
	rm Scaner.o
	rm TCPConnectScan.o
	rm TCPSYNScan.o
	rm TCPFINScan.o
	rm UDPScan.o
