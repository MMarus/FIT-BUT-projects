/* Author:      Marek Marusic, xmarus05
*  Email:       xmarus05@stud.fit.vutbr.cz
*  Purpose:     Project to ISA
*  Description: Create client for SIP protokol
*  File:        sip.h
*/


//RegisterSIP
int runClient(s_options *options, std::vector<s_message> *message);

int getDstAddress(sockaddr_in *dstAddr, std::string server, int port);

void getAddresses(sockaddr_in *dstAddr, sockaddr_in *srcAddr, std::string server, int port);

std::string getSipData(s_packet *packet, int responseNum);

int processPacket(char *buffer, sockaddr_in *dstAddr, sockaddr_in *srcAddr, std::string userName);

void genSIPRegisterPacketStruct(s_options *options);

void registerSip();

void sendMessages(std::vector<s_message> *messages);

void service(s_options *options);

int getSrcAddress(sockaddr_in *srcAddr);