/* Author:      Marek Marusic, xmarus05
*  Email:       xmarus05@stud.fit.vutbr.cz
*  Purpose:     Project to ISA
*  Description: Create client for SIP protokol
*  File:        sip.cpp
*/

#include "main.h"
#include <bitset>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define MessageNum 11
std::string protocol = "SIP/2.0";
/// DONE//TODO: vypysat aj cislo portu ak nieje 5060
/// DONE//TODO: Po prijati MESSSAGE, zistit ci som spravny prijemca MSG
/// DONE//TODO: urobit timeout na registracne pakety, a poslat znova
/// DONE//TODO: ak pride odpoved na registraciu 5xx, stale posielat v intervalech register zpravy
/// DONE////TODO: udržovat klienta autentizovaného, tedy opakovaně zasílal REGISTER se zadanou hodnotou Expires před vypršením autentizace

//TODO: ked pride sprava pre niekoho ineho posli odpoved s chybou
//TODO: CSeq forbidden by mala byt rovnaka
std::string removeCR(std::string str){
    size_t CR = str.find("\r",0);
    if(CR != std::string::npos)
        return str.erase( CR, 1);
    else
        return str;
}

//Zdroj: http://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
std::string random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
                "0123456789"
                "ABCDEF";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}
//Koniec Citacie

//Vygeneruj veci co treba random
void randomizeColumns(s_packet* PACKET){
    int num = rand() % 20 + 6;

    PACKET->branchRand = random_string(num);
    PACKET->tag = ";tag="+random_string(num);
    PACKET->callId = random_string(num);
    return;
}

std::string findPattern(std::string str, std::string start, std::string end){
    size_t startPos, endPos;
    startPos = str.find(start,0);
    endPos = str.find(end,startPos+start.length());
    if( startPos != std::string::npos && endPos != std::string::npos){
        std::string output = str.substr(startPos+start.length(),endPos-(start.length() + startPos));
        return output;
    }
    else{
        return "";
    }
}

int runClient(s_options *options, std::vector<s_message> *messages) {
    using namespace std;
    struct timeval timeout;
    char strDst[INET_ADDRSTRLEN];
    struct sockaddr_in bindAddr;
    /* Specify the address family */
    bindAddr.sin_family = AF_INET;
    /* Specify the destination port */
    bindAddr.sin_port = htons(5060);
    bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    inet_ntop(AF_INET, &(bindAddr.sin_addr), strDst, INET_ADDRSTRLEN);

    getAddresses(&dstAddr,&srcAddr, options->server, options->port);

    /* Create a socket */
    if ((socketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        err(1, "Error on socket");
    }

    //Set Tiemout on sendto and rcvfrom
    timeout.tv_sec = SocketTimeout;
    timeout.tv_usec = 0;

    if (setsockopt (socketFd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                    sizeof(timeout)) < 0)
        err(1, "setsockopt failed");

    if (setsockopt (socketFd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
                    sizeof(timeout)) < 0)
        err(1, "setsockopt failed");



    socklen_t dstLength = sizeof(dstAddr);
    socklen_t srcLength = sizeof(srcAddr);

    //BIND
    //printf("binding to the port %d (%d)\n", ntohs(srcAddr.sin_port), srcAddr.sin_port);

    // binding with the port
    if (bind(socketFd, (struct sockaddr *) &bindAddr, srcLength) == -1){
        err(1, "bind() failed");
    }

    //Generate common packet
    genSIPRegisterPacketStruct(options);

    registerSip();
    packetUNREGISTER = packet;
    packet.authorization = 0;

    //POSLI SPRAVY
    if (!(*messages).empty()) {
        sendMessages(messages);
    }

    service(options);

    if(packet.authorized == 1){
        //Udrzuj spojenie a odosielaj spravne odpovede

        //if registered, unregister
        packetUNREGISTER.cseq = packet.cseq + 1;
        packetUNREGISTER.expires = 0;
        packet = packetUNREGISTER;
        registerSip();
    }

    return 0;
}

void print(std::string out){
    std::cout << out << std::endl;
}

void service(s_options *options){
    ssize_t nread;
    double timeLeft;
    time_t now;
    std::string bufferOut;
    struct sockaddr_in dstAddrReciever;
    int packetType;
    int messageForMe;
    char bufferIn[BUFFER_SIZE];
    socklen_t dstLength = sizeof(dstAddrReciever);


    while ((nread = recvfrom(socketFd, bufferIn, BUFFER_SIZE, 0, (struct sockaddr *) &dstAddrReciever, &dstLength))) {
        //print("read1 = "+std::to_string(nread));

        if(nread >= 0){
            // 0 when no response needed
            //1 - sending accepted message
            //2 - sending Bad Request
            //3 - sending Method Not Allowed
            //4 - error missing from in packet - malformed packet
            //5 - message is not for me - 403 Forbidden
            packetType = processPacket(bufferIn,&dstAddr, &srcAddr, options->username);
            if(packetType == 4)
                std::cerr << "err: missing from" << std::endl;

            if(packetType != 0 && packetType != 3 ){
                //getDstAddress
                bufferOut = getSipData(&packetResponse, packetType);
                if (getDstAddress(&dstAddrReciever, packetResponse.dstIP, stoi(packetResponse.portTo )) != 0) {
                    std::cerr << "Error during getting address of message reciever" << std::endl;
                    continue;
                }

                //Posli response packet
                if (sendto(socketFd, bufferOut.c_str(), bufferOut.length() + 1, 0, (struct sockaddr *) &dstAddrReciever, dstLength) < 0) {
                    std::cerr << "Error: on send to 2. registration packet. exitCode #" << errno << std::endl;
                }
            }
        }

        //If we are authorized, check if there is enough time left, or register again
        if(packetUNREGISTER.authorized == 1){
            //MinTimeLeft
            now = time(NULL);
            timeLeft = difftime(packetUNREGISTER.timeExpires, now);
            //print("time left = " + std::to_string(timeLeft));
            //Send REGISTRATION PACKET AGAIN
            if( timeLeft < MinTimeLeft ){
                //if registered, unregister
                packetUNREGISTER.cseq = packet.cseq + 1;
                packet = packetUNREGISTER;
                registerSip();
                packetUNREGISTER = packet;
            }
        }

    }

    return;
}

void sendMessages(std::vector<s_message> *messages) {

    //kazdu spravu posli
    for (const auto &msg : *messages) {
        struct sockaddr_in dstAddrReciever;
        ssize_t nread;
        std::string bufferOut;
        char bufferIn[BUFFER_SIZE];
        socklen_t dstLength = sizeof(dstAddrReciever);

        //std::cout << "Context:" << msg.context << " Destination: " << msg.dst << " User: " << msg.usr << std::endl;

        //Naplnit DST addres ( komu spravu posielam)
        if ( getDstAddress(&dstAddrReciever, msg.dst, msg.port ) != 0) {
            std::cerr << "Error during getting address of message reciever" << std::endl;
            continue;
        }


        //Naplnit paket
        packet.method = "MESSAGE";
        char str_dstIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(dstAddrReciever.sin_addr), str_dstIP, INET_ADDRSTRLEN);
        packet.dstIP = msg.dst;
        packet.from =  packet.username + "@" + packet.srcIP;
        packet.to =  msg.usr + "@" + packet.dstIP;
        packet.authorization = 0;
        //std::cout << packet.cseq << std::endl;
        packet.cseq += 1;
        //std::cout << packet.cseq << std::endl;
        packet.content = msg.context;
        packet.allow = "";
        packet.portTo = std::to_string(msg.port);
        randomizeColumns(&packet);


        //Poslat spravu
        bufferOut = getSipData(&packet, MessageNum);
        if (sendto(socketFd, bufferOut.c_str(), bufferOut.length(), 0, (struct sockaddr *) &dstAddrReciever, dstLength) < 0) {
            std::cerr << "Error: on send exitCode # " << errno << std::endl;
        }

        //Prijat odpoved
        if ((nread = recvfrom(socketFd, bufferIn, BUFFER_SIZE, 0, (struct sockaddr *) &dstAddrReciever, &dstLength)) >= 0) {
            processPacket(bufferIn, &dstAddr, &srcAddr, "");
        }
    }
}

void registerSip() {
    //Posli prvy registracny packet1
    int errorCode = 0;
    int counter = 10;
    ssize_t nread;
    int type = 0;
    std::string bufferOut;
    char bufferIn[BUFFER_SIZE];
    socklen_t dstLength = sizeof(dstAddr);

    bufferOut = getSipData(&packet, 0);
    if (sendto(socketFd, bufferOut.c_str(), bufferOut.length() + 1, 0, (struct sockaddr *) &dstAddr, dstLength) < 0) {
        std::cerr << "Error: on send registration packet exitCode # " << errno << std::endl;
    }


    //Prijmi paket1

    while((nread = recvfrom(socketFd, bufferIn, BUFFER_SIZE, 0, (struct sockaddr *) &dstAddr, &dstLength))) {
        if(nread >= 0){
            type = processPacket(bufferIn, &dstAddr, &srcAddr, "");

            //Stop sending packets when there is a response && response packet is not error 500
            if(type != 500 && type != 401)
                return;

            if( type == 500 ){
                counter--;
                //sleep for 5 second before retransmiting package
                sleep(5);
            }
        }
        else{
            counter--;
        }

        packet.cseq++;
        //Get the registration packet
        bufferOut = getSipData(&packet, type);
        //if packet is retransmitted more than 10 times quit contacting server
        if(counter <= 0)
            return;
        //Posli packet2
        if (sendto(socketFd, bufferOut.c_str(), bufferOut.length() + 1, 0, (struct sockaddr *) &dstAddr, dstLength) < 0) {
            std::cerr << "Error: on send to 2. registration packet. exitCode #" << errorCode << std::endl;
        }
    }

    return;
}

//ProcessPacket function
int processPacket(char *bufferIn, sockaddr_in *dstAddr, sockaddr_in *srcAddr, std::string userName) {
    int errCode = 0; // typ paketu = 1 request packet / 2 response packet
    std::istringstream BufferStream (bufferIn);
    std::string buf_protocol, buf_description, buf_linePrefix_str, buf_from, buf_to, user;
    std::string buf_message;
    std::string buf_code;
    size_t start, end;
    packetResponse = packet;
    packetResponse.content = "";

    std::string line;
    int lineCounter = 0;
    char * msg_buffer;


    //Cseq
    std::string cseqDescription;
    int buf_cseq = -1;
    //expires
    int expires = 0, content_length = 0;

    //napln stream bufferom z packetu

    std::string restOfLine;

    //Po line spracuj
    while(std::getline(BufferStream, line)){
        lineCounter++;


        //zmaz CR (\r) ak sa nachadza
        if(line == "\r" && content_length  > 0){
            msg_buffer = new char [content_length+1];
            BufferStream.get(msg_buffer, content_length+1, '\0');
            buf_message = msg_buffer;
            buf_message = removeCR(buf_message);
        }
        line = removeCR(line);

        std::istringstream iss(line);
        std::string word;
        iss >> word;

        //FIRST LINE - MUST BE REQUEST OR RESPONSE
        if(lineCounter == 1){
            //RESPONSE LINE
            if ( word == protocol ) {
                iss >> buf_code;
                iss.ignore(1);
                std::getline(iss, buf_description);
            }
            //REGUEST Line
            else{
                buf_description = word;
                iss >> word;
                user = findPattern(word, "sip:", "@");
                //std::cout << word << std::endl;
                iss >> word;
                if ( word != protocol ){
                    errCode = 2; // SEND Bad Request response
                }
                else if(buf_description != "MESSAGE" || user != userName){
                    errCode = 5;
                }
            }
        }
        /*TODO: for every line, find ".*:" and check if it is
         *     "CSeq", "Via", "From", |Call-ID:", "To",
         *     "Contact", "Allow", "Content-Length", "Content-Type"
         */
        else{
            //Cseq
            if(word == "CSeq:"){
                //std::cout << "DEBUG:" << word << std::endl;
                iss >> buf_cseq;
                iss >> cseqDescription;
                //std::cout << "DEBUG:" << buf_cseq <<  std::endl;
                packetResponse.cseq = buf_cseq;
            }
            else if(word == "From:"){
               std::getline(iss, restOfLine);
                buf_from = findPattern(restOfLine, "<sip:", ">");
                if( buf_from == ""){
                    if(errCode == 0 )
                        errCode = 2; // SEND Bad Request response
                }
                packetResponse.from = buf_from;
                packetResponse.fromName = findPattern(restOfLine, "<sip:", "@");
                packetResponse.fromIP = findPattern(restOfLine, "@", ">");
            }
            else if(word == "To:"){
                                iss >> restOfLine;

                buf_to = findPattern(restOfLine, "<sip:", ">");
                if( buf_to == ""){
                    if(errCode == 0 )
                        errCode = 2; // SEND Bad Request response
                }
                packetResponse.to = buf_to;
            }
            else if(word == "Via:"){
                iss >> restOfLine; iss >> restOfLine;
                packetResponse.viaAccept = restOfLine;
                std::string dstip = findPattern(restOfLine, "", ":");
                std::string dstport = findPattern(restOfLine, ":", ";");
                if( dstip != "" && dstport != ""){
                    packetResponse.dstIP =  dstip;
                    packetResponse.portTo = dstport;
                    packetResponse.branchRand = findPattern(restOfLine, "z9hG4bK", ";");
                }
                else
                    return 4;
            }
            else if(word == "User-Agent:"){}
            else if(word == "Expires:"){
                 iss >> expires;
                packetResponse.expires = expires;
            }
            else if(word == "Content-Length:"){
                iss >> content_length;
                packetResponse.content = "";
            }
            else if(word == "Max-Forwards:"){}
            else if(word == "Contact:"){}
            else if(word == "Call-ID:"){
                iss >> restOfLine;
                packetResponse.callId = findPattern(restOfLine, "", "@");
                packetResponse.callIdAccept = restOfLine;
            }
            else if(word == "WWW-Authenticate:") {
                packet.algorithm = ""; packet.realm = ""; packet.nonce = "";
                std::string restOfLine;
                std::string digest, alg, realm, nonce;
                iss >> digest;

                while( iss >> restOfLine ){
                    if(packet.algorithm == "")
                        packet.algorithm = findPattern(restOfLine, "algorithm=", ",");
                    if(packet.realm == "")
                        packet.realm = findPattern(restOfLine, "realm=\"", "\",");
                    if(packet.nonce == "")
                    packet.nonce = findPattern(restOfLine, "nonce=\"", "\"");
                }
                //std::cout << "AUTHENTICATION PARAMTERS alg=" << packet.algorithm << " realm:" << packet.realm << " nonce: " << packet.nonce << std::endl;
            }
        }

        //TODO: GET THE MESSAGE DATA (CONTEXT)

    }

//Vypis prijatych paketov
    std::cout << buf_cseq << " ";
    if(buf_code != "")
        std::cout << buf_code << " ";
    std::cout<< buf_description << " " << buf_from << " " << buf_to << " " << buf_message << std::endl;


    /// TU SA SPRACOVAVAJU DATA ZISTENE Z PAKETU
    if (buf_code == "401" ) {
        packet.authorization = 1;
        //Vygenerovat branch
        //packet.cseq += 1;

        //MD5
        unsigned char digest[MD5_DIGEST_LENGTH];
        char mdString[33];

        //generate HA1 and HA2 for MD5 response
        std::string HA1 = packet.username + ":" + packet.realm + ":" + packet.password;
        //std::cout << "!!!!!!!!!!!!!!!  " + packet.password << std::endl << std::endl;
        //std::cout << "HA1 before : " << HA1 << std::endl;


        MD5((unsigned char *) HA1.c_str(), HA1.length(), (unsigned char *) &digest);
        for (int i = 0; i < 16; i++)
            sprintf(&(packet.md5_HA1)[i * 2], "%02x", (unsigned int) digest[i]);
        //std::cout << packet.md5_HA1 << std::endl;

        std::string HA2 = "REGISTER:sip:" + packet.dstIP;
        //std::cout << "HA2 before : " << HA2 << std::endl;


        MD5((unsigned char *) HA2.c_str(), HA2.length(), (unsigned char *) &digest);
        for (int i = 0; i < 16; i++)
            sprintf(&(packet.md5_HA2)[i * 2], "%02x", (unsigned int) digest[i]);
        //std::cout << packet.md5_HA2 << std::endl;


        std::string response = packet.md5_HA1;
        response += ":";
        response += packet.nonce;
        response += ":";
        response += packet.md5_HA2;

        //std::cout << "RESPONSE before : " << response << std::endl;

        MD5((unsigned char *) response.c_str(), response.length(), (unsigned char *) &digest);
        for (int i = 0; i < 16; i++)
            sprintf(&(packet.response)[i * 2], "%02x", (unsigned int) digest[i]);
        //std::cout << packet.response << std::endl;

        return 401;
    }
    else if(buf_code.compare(0, 1, "4") == 0){
        return 400;
    }
    else if(buf_code == "200" ){
        packet.authorized = 1;
        packet.timeExpires = time(NULL) + packet.expires;
        return 200;
    }
    else if(buf_code == "202"){
        return 202;
    }
    else if(buf_code.compare(0,1,"5") == 0 ){
        return 500;
    }
    else if ( buf_description == "MESSAGE" && errCode != 5){
        errCode = 1; //Send Accept Packet
    }
    else if ( buf_description == "INVITE" || buf_description == "ACK" || buf_description == "CANCEL" || \
                buf_description == "OPTIONS" || buf_description == "BYE" || buf_description == "REFER" || \
                buf_description == "SUBSCRIBE" || buf_description == "NOTIFY" || buf_description == "INFO" || \
                buf_description == "PUBLISH" ){
        errCode = 3; //Method not allowed
    }


    return errCode;

}

//nastav info v packete
void genSIPRegisterPacketStruct(s_options *options) {


    packet.method = "REGISTER";

    char str_dstIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(dstAddr.sin_addr), str_dstIP, INET_ADDRSTRLEN);

    packet.dstIP = options->server;

    char str_srcIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(srcAddr.sin_addr), str_srcIP, INET_ADDRSTRLEN);
    packet.from = options->username  + "@" + packet.dstIP;
    packet.to =  options->username + "@" + packet.dstIP;
    packet.srcIP = str_srcIP;
    packet.srcPort = ntohs(srcAddr.sin_port);
    randomizeColumns(&packet);
    packet.allow = "Allow: OPTIONS,MESSAGE\n";
    packet.digest.clear();
    packet.algorithm.clear();
    packet.realm.clear();
    packet.nonce.clear();
    packet.username = options->username;
    packet.password = options->password;
    packet.cseq = 1;
    packet.expires = options->expires;
    packet.portTo = std::to_string(options->port);

    return;
}

//Create PACKET function
std::string getSipData(s_packet *packet, int responseNum) {
    std::stringstream PACKET;
    std::string authorizationTxt = "Authorization: Digest username=\"";

    ///VARIABLES
    std::string via = "Via: SIP/2.0/UDP";
    std::string from = "From: ";
    std::string to = "To: <sip:";
    std::string request;
    std::string msgContent;
    std::string expiresLine = "Expires: " + std::to_string(packet->expires)+ "\n";
    std::string maxForward = "Max-Forwards: 70\n";

    //TODO: vytvor nejaky generator pre nahodny branch
    std::string branchTxt = ";branch=z9hG4bK";

    //TODO: generate tag
    std::string tagTxt = packet->tag;
    std::string destination = "";
    std::string portTo = "";
    if( packet->portTo != "5060" && packet->portTo != ""){
        portTo = ":" + packet->portTo;
    }
    std::string fromName = "";
    std::string callIdLine = packet->callId + "@" + packet->srcIP;
    std::string ViaLine = packet->srcIP + ":" + std::to_string(packet->srcPort) + branchTxt + packet->branchRand + ";rport" ;

    //SETTING UP THE PACKET

    //Reguest line Line#1
    if(packet->method == "MESSAGE")
        destination = packet->to;
    else
        destination = packet->dstIP;

    //PRINTING THE OUTPUT ABOUT SENT MESSAGE


    if ( responseNum == 1 ){
        // RESPONSE Accepted message
        request = "202 Accepted";
        PACKET << protocol << " " << request << std::endl;
        fromName = "\""+packet->fromName+"\" ";
        tagTxt = "";
        expiresLine = "";
        maxForward = "";
        //TODO: addo contact, recieved, remove content-typ
        std::string rportAccept = portTo;
        if(rportAccept == "")
            rportAccept = "5060";

        ViaLine = packet->viaAccept+"="+rportAccept+";recieved="+packet->fromIP;
        callIdLine = packet->callIdAccept;

    }
    else if ( responseNum == 2 ){
        // RESPONSE Bad Request
        request = "400 Bad Request";
        PACKET << protocol << " " << request << std::endl;
    }
    else if ( responseNum == 3){
        request = "405 Method Not Allowed";
        PACKET << protocol << " " << request << std::endl;
    }
    else if ( responseNum == 5){
        request = "403 Forbidden";
        PACKET << protocol << " " << request << std::endl;
        fromName = "\""+packet->fromName+"\" ";
        tagTxt = "";
        expiresLine = "";
        maxForward = "";
        //TODO: addo contact, recieved, remove content-typ
        std::string rportAccept = portTo;
        if(rportAccept == "")
            rportAccept = "5060";

        ViaLine = packet->viaAccept+"="+rportAccept+";recieved="+packet->fromIP;
        callIdLine = packet->callIdAccept;
    }
    else{
        //#1 Request line
        request = packet->method;
        if(packet->expires == 0 && packet->method == "REGISTER")
            request = "UN"+request;
        PACKET << packet->method << " " << "sip:" << destination << " " << protocol << std::endl;
    }

    if(responseNum == MessageNum)
        msgContent = " " + packet->content;


    //Vypis odoslanych paketov
    std::cout << packet->cseq << " " << request << " " << packet->from << " " << packet->to << portTo << msgContent << std::endl;

    //Cseq line#2
    PACKET << "CSeq: " << packet->cseq << " " << packet->method << std::endl;
    //VIA Line#3
    PACKET << via << " " << ViaLine << std::endl;
    //U-agent line#4
    PACKET << "User-Agent: mareckov" << std::endl;

    //Authorization line 5
    if (packet->authorization == 1) {
        PACKET << authorizationTxt << packet->username << "\", uri=\"sip:" << packet->dstIP \
 << "\", algorithm=" << packet->algorithm << ", realm=\"" << packet->realm \
 << "\", nonce=\"" << packet->nonce << "\", response=\"" << packet->response \
 << "\"" << std::endl;
    }

    //From Line#6
    PACKET << from << fromName <<  "<sip:" << packet->from << ">" << tagTxt << std::endl;

    //Call-ID line#7
    PACKET << "Call-ID: " << callIdLine << std::endl;

    //To line#8
    PACKET << to << packet->to << ">" << std::endl;

    //Contact line#9
    if( responseNum != MessageNum)
        PACKET << "Contact: <sip:" << packet->username << "@" << packet->srcIP << ":" << packet->srcPort << ">" << std::endl;
    //Allow line 10
    PACKET << packet->allow;

    //Expires line#11
    PACKET << expiresLine;

    //ContentLen line#12
    PACKET << "Content-Length: " << (packet->content).length() <<"\n";

    if(responseNum == MessageNum)
        PACKET << "Content-Type: text/plain;charset=UTF-8\n";

    //Max-Forwards line#8
    PACKET << maxForward ;

    // LINE #12 endin endline
    PACKET << std::endl;

    if(responseNum == MessageNum)
        PACKET << packet->content;

    //#DEBUG
    //std::cout << std::endl << PACKET.str() << std::endl;

    return PACKET.str();
}

uint32_t IPToUInt(const std::string ip) {
    int a, b, c, d;
    uint32_t addr = 0;

    if (sscanf(ip.c_str(), "%d.%d.%d.%d", &a, &b, &c, &d) != 4)
        return 0;

    addr = a << 24;
    addr |= b << 16;
    addr |= c << 8;
    addr |= d;
    return addr;
}

bool IsIPInRange(const std::string ip, const std::string network, const std::string mask) {
    uint32_t ip_addr = IPToUInt(ip);
    uint32_t network_addr = IPToUInt(network);
    uint32_t mask_addr = IPToUInt(mask);

    uint32_t net_lower = (network_addr & mask_addr);
    uint32_t net_upper = (net_lower | (~mask_addr));

    return ip_addr >= net_lower && ip_addr <= net_upper;
}

void getAddresses(sockaddr_in *dstAddr, sockaddr_in *srcAddr, std::string server, int port){
    struct ifaddrs *ifaddr, *ifa;
    int n, count = 0, srcSet = 0;
    char strDst[INET_ADDRSTRLEN];
    char strSrc[INET_ADDRSTRLEN];
    char netMask[INET_ADDRSTRLEN];
    struct servent *sp;
    struct hostent *dstServEnt;         // network host entry required by gethostbyname()

//DEST PART
    /* Specify the address family */
    dstAddr->sin_family = AF_INET;
    /* Specify the destination port */
    dstAddr->sin_port = htons(port);


    // GET DESTINATION ADDR make DNS resolution of the first parameter using gethostbyname()
    if ((dstServEnt = gethostbyname((server).c_str())) == NULL) { // check the first parameter
        //std::cout << "!!!IP:!!!! " << (server).c_str() << std::endl;
        if ( inet_pton(AF_INET, (server).c_str(), &(dstAddr->sin_addr)) != 1) {
            err(5,"Error: can't find the host you specified");
        }
    }
    else {

        // copy the first parameter to the server.sin_addr structure
        memcpy(&(dstAddr->sin_addr), dstServEnt->h_addr, dstServEnt->h_length);


        inet_ntop(AF_INET, &(dstAddr->sin_addr), strDst, INET_ADDRSTRLEN);
        //printf("The server IP = %s\n", strDst);
    }

//SOURCE PART
    /* Specify the address family */
    srcAddr->sin_family = AF_INET;
    /* Specify the destination port */
    srcAddr->sin_port = htons(5060);

    // GET SOURCE ADDR
    if (getifaddrs(&ifaddr) == -1) {
        err(1, "Error:getifaddrs() failed");
    }

    for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
        if (ifa->ifa_addr == NULL)
            continue;

        if(ifa->ifa_addr->sa_family == AF_INET){
            //std::cout << "Name of the interface: " << ifa->ifa_name << std::endl;
            if ( getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), strSrc, INET_ADDRSTRLEN, NULL, 0, NI_NUMERICHOST) != 0)
                err(1, "Failed to get addr of interface");
            //printf("\t\taddress: <%s>\n", strSrc);
            if ( getnameinfo(ifa->ifa_netmask, sizeof(struct sockaddr_in), netMask, INET_ADDRSTRLEN, NULL, 0, 0) != 0)
                err(1, "Failed to get addr of netMask");
            //printf("\t\tSubnet: <%s>\n", netMask);
            //IF src address is in range of dst addr use that address
            count++;
            if ( IsIPInRange(strSrc, strDst, netMask) || count == 2){
                //std::cout << "LOCAL IP = " << strSrc << std::endl;
                if (inet_pton(AF_INET, strSrc, &(srcAddr->sin_addr)) != 1) {
                    err(5, "Error: can't find the host you specified");
                }
                srcSet = 1;

                //TODO: REMOVE THIS 2 LINES (ITS ASSIGNING SECOND IFACE)
                freeifaddrs(ifaddr);
                return;
            }

        }
    }
    if(srcSet == 0){
        //std::cout << "Error , using LOOPBACK" << std::endl;
        if (inet_pton(AF_INET, "127.0.0.1", &(srcAddr->sin_addr)) != 1) {
            err(5, "Error: can't find the host you specified");
        }
    }

    freeifaddrs(ifaddr);
}

int getDstAddress(sockaddr_in *addr, std::string server, int port) {
    int errorCode = 0;
    //Get SIP protocol port
    struct servent *sp;
    struct hostent *dstServEnt;         // network host entry required by gethostbyname()

    //Get SIP protocol port

    /* Specify the address family */
    addr->sin_family = AF_INET;
    /* Specify the destination port */
    addr->sin_port = htons(port);

    /* Specify the destination IP address */
    // make DNS resolution of the first parameter using gethostbyname()
    if ((dstServEnt = gethostbyname((server).c_str())) == NULL) { // check the first parameter
        errorCode = inet_pton(AF_INET, (server).c_str(), &(addr->sin_addr));
        if (errorCode != 1) {
            std::cerr << "Error: can't find the host you specified";
            return 5;
        }
    }
    else {
        // copy the first parameter to the server.sin_addr structure
        memcpy(&(addr->sin_addr), dstServEnt->h_addr, dstServEnt->h_length);
    }

    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr->sin_addr), str, INET_ADDRSTRLEN);
    //printf("The server IP = %s\n", str);

    return 0;
}

int getSrcAddress(sockaddr_in *srcAddr) {
    int port = 5060;
    char hostname[128];
    std::string SRC_ADDR;

    struct hostent *srcServEnt;         // network host entry required by gethostbyname()

    /* Specify the address family */
    srcAddr->sin_family = AF_INET;
    /* Specify the destination port */
    srcAddr->sin_port = htons(port);



    /* Specify the destination IP address */
    if ((gethostname(hostname, sizeof hostname)) == 0) {
        //std::cout << "hostname" << hostname << std::endl;


        if ((srcServEnt = gethostbyname(hostname)) != NULL) { // check the first parameter
            memcpy(&(srcAddr->sin_addr), srcServEnt->h_addr, srcServEnt->h_length);
        }
        else {
            std::cerr << "Faild to get hostname" << std::endl;
            SRC_ADDR = "127.0.0.1";
        }

    }
    else {
        std::cerr << "Faild to get hostname" << std::endl;
        SRC_ADDR = "127.0.0.1";
    }

    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(srcAddr->sin_addr), str, INET_ADDRSTRLEN);
    //printf("The local IP = %s\n", str);

    return 0;
}
