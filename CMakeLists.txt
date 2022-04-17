cmake_minimum_required(VERSION 3.5)
project(AlliInOne)

set(CMAKE_CXX_STANDARD 11)

add_executable(AlliInOne main.cpp)

add_executable(ClientA clientA.cpp util/transferItem.cpp util/transferItem.h
        util/fileOperation.cpp util/fileOperation.h
        util/tcpHelper.cpp util/tcpHelper.h util/udpHelper.cpp
        util/udpHelper.h)

add_executable(ClientB clientB.cpp util/transferItem.cpp util/transferItem.h
        util/fileOperation.cpp util/fileOperation.h
        util/tcpHelper.cpp util/tcpHelper.h util/udpHelper.cpp
        util/udpHelper.h)

#add_executable(serverM serverM.cpp util/transferItem.cpp util/transferItem.h
#        util/fileOperation.cpp util/fileOperation.h
#        util/tcpHelper.cpp util/tcpHelper.h util/udpHelper.cpp
#        util/udpHelper.h util/stasInfo.cpp util/stasInfo.h)
add_executable(serverM serverM.cpp util/transferItem.cpp util/transferItem.h
        util/fileOperation.cpp util/fileOperation.h
        util/tcpHelper.cpp util/tcpHelper.h util/udpHelper.cpp
        util/stasInfo.cpp util/stasInfo.h)

add_executable(serverA serverA.cpp  util/transferItem.cpp util/transferItem.h
        util/fileOperation.cpp util/fileOperation.h
        util/tcpHelper.cpp util/tcpHelper.h util/udpHelper.cpp
        util/udpHelper.h util/stasInfo.h)

add_executable(serverB serverB.cpp  util/transferItem.cpp util/transferItem.h
        util/fileOperation.cpp util/fileOperation.h
        util/tcpHelper.cpp util/tcpHelper.h util/udpHelper.cpp
        util/udpHelper.h util/stasInfo.h)

add_executable(serverC serverC.cpp  util/transferItem.cpp util/transferItem.h
        util/fileOperation.cpp util/fileOperation.h
        util/tcpHelper.cpp util/tcpHelper.h util/udpHelper.cpp
        util/udpHelper.h util/stasInfo.h)