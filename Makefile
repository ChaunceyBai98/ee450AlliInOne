all:
	g++ -std=c++11 -o serverA serverA.cpp util/tcpHelper.cpp util/fileOperation.cpp util/stasInfo.cpp util/transferItem.cpp util/udpHelper.cpp
	g++ -std=c++11 -o serverB serverB.cpp util/tcpHelper.cpp util/fileOperation.cpp util/stasInfo.cpp util/transferItem.cpp util/udpHelper.cpp
	g++ -std=c++11 -o serverC serverC.cpp util/tcpHelper.cpp util/fileOperation.cpp util/stasInfo.cpp util/transferItem.cpp util/udpHelper.cpp
	g++ -std=c++11 -o serverM serverM.cpp util/tcpHelper.cpp util/fileOperation.cpp util/stasInfo.cpp util/transferItem.cpp util/udpHelper.cpp
	g++ -std=c++11 -o clientA clientA.cpp util/tcpHelper.cpp util/fileOperation.cpp util/stasInfo.cpp util/transferItem.cpp util/udpHelper.cpp
	g++ -std=c++11 -o clientB clientB.cpp util/tcpHelper.cpp util/fileOperation.cpp util/stasInfo.cpp util/transferItem.cpp util/udpHelper.cpp

