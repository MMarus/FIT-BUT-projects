/* Author:      Marek Marusic, xmarus05
*  Email:       xmarus05@stud.fit.vutbr.cz
*  Purpose:     Project to ISA
*  Description: Create client for SIP protokol
*  File:        main.h
*/

#ifndef MAIN_H
#define MAIN_H

//Standard libs
#include <iostream>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <openssl/md5.h>
#include <string>
#include <sstream>
#include <signal.h>
#include <ifaddrs.h>
#include <err.h>
#include <algorithm>
#include <sys/time.h>


//My structs
struct s_packet {
    std::string method;
    std::string dstIP;
    std::string from;
    std::string to;
    std::string srcIP;
    std::string allow;
    int srcPort;
    std::string portTo;
    std::string branchRand;
    std::string tag;
    std::string digest;
    std::string algorithm;
    int authorization = 0;
    int authorized = 0;
    std::string realm;
    std::string nonce;
    std::string username;
    std::string password;
    std::string callId;
    int cseq;
    int expires;
    std::string content;
    char response[33];
    char md5_HA1[33];
    char md5_HA2[33];
    time_t timeExpires;
    std::string fromName;
    std::string fromIP;
    std::string callIdAccept;
    std::string viaAccept;
};

struct s_options{
    std::string server;
    int port;
    std::string username;
    std::string password;
    int expires;
};

struct s_message{
    std::string dst;
    int port;
    std::string usr;
    std::string context;
};



//My vars
extern int socketFd;
extern s_packet packet, packetResponse, packetUNREGISTER;
extern struct sockaddr_in dstAddr;
extern struct sockaddr_in srcAddr;

#define MinTimeLeft 10
#define SocketTimeout 5

//My libs
#include "file_io.h"
#include "sip.h"

#endif
