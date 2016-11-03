/* Author:      Marek Marusic, xmarus05
*  Email:       xmarus05@stud.fit.vutbr.cz
*  Purpose:     Project to ISA
*  Description: Create client for SIP protokol
*  File:        main.cpp
*/
//My libs
#include "main.h"

int socketFd = -1;
s_packet packet;
s_packet packetUNREGISTER;
s_packet packetResponse;

struct sockaddr_in dstAddr;
struct sockaddr_in srcAddr;

void sigint(int a)
{
    //Odregistruj sa
    if(packet.authorized == 1){
        packetUNREGISTER.cseq = packet.cseq + 1;
        packetUNREGISTER.expires = 0;
        packet = packetUNREGISTER;
        registerSip();
        //std::cout << "Unregistering" << std::endl;
    } else
        //std::cout << "NOT REGISTERED"<< std::endl;

    //ZAVRI SOKET A UKONCI
    if (socketFd != -1) {
        /* Close the socket */
        close(socketFd);
    }
    //std::cout << "EXITING" << std::endl;
    exit(0);
}

void printHelp(void){
    std::cout << "sipklient - Klient pre pripojenie na server a posielanie sprav" << std::endl << std::endl
    << "-h, --help" << std::endl
    << "\tVypis tuto napovedu." << std::endl << std::endl
    << "-p options.txt" << std::endl
    << "\tSpecifikuje meno suboru, v ktorom sa nachadzaju udaje" << std::endl
    << "\tpotrebne na pripojenie k SIP serveru." << std::endl << std::endl

    << "-m messages.txt" << std::endl
    << "\tSpecifikuje meno suboru, v ktorom sa nachadzaju udaje" << std::endl
    << "\tpotrebne pre odoslanie sprav." << std::endl
    << "\tSubor musi mat nasledujuci format:" << std::endl
    << "\t\tmeno@server:port TELO SPRAVY." << std::endl;
}

int main(int argc, char *argv[]) {
    using namespace std;

    signal(SIGTERM, sigint);
    signal(SIGQUIT, sigint);
    signal(SIGINT, sigint);

    //f_options: contains name of file with options to be processed
    string f_options;
    s_options newOptions;
    //f_messages: contains name of file with messages to use
    string f_messages;
    //Vector of messages, contains message reciever and message context
    vector<s_message> messages;

    //opt_flag = do we have parameter with options file ?
    //msg_flag = do we have parameter with messages file ?
    //opt = getopt int value
    int opt_flag = 0, msg_flag = 0, opt = 0, err = 0, help_flag = 0;

    while ((opt = getopt(argc, argv, "hp:m:")) != -1) {
        switch (opt) {
            case 'p':
                opt_flag = 1;
                f_options = optarg;
                break;
            case 'm':
                msg_flag = 1;
                f_messages = optarg;
                break;
            case 'h':
                help_flag = 1;
                break;
            default: /* '?' */
                cerr << "Error wrong parameters!" << endl;
                fprintf(stderr, "Usage: %s -p f_options.txt [-m f_messages.txt]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    //cout << "opt_flag=" << opt_flag << " msg=" << f_messages << " f_options=" << f_options << " msg_flag=" << msg_flag << endl;
    //cout << "optind=" << optind << " argc=" << argc << endl;

    if (optind != argc) {
        fprintf(stderr, "Too much arguments\n");
        exit(EXIT_FAILURE);
    }
    if(help_flag == 1){
        printHelp();
        return 0;
    }

    //Read options from file
    if (opt_flag != 1) {
        cerr << "Error no options file specified!" << endl;
        fprintf(stderr, "Usage: %s -p f_options.txt [-m f_messages.txt]\n", argv[0]);
        exit(EXIT_FAILURE);

    }
    err = readOptions(f_options, &newOptions);
    if (err != 0) {
        return 1;
    }

    //Read messages from file
    if (msg_flag == 1) {
        err = readMessages(f_messages, &messages);
        if (err != 0) {
            return 1;
        }
    }


    err = runClient(&newOptions, &messages);
    //TODO: najskor sa zaregistruje + vypis na vystup
    //Po uspesnej registracii posle spravy z messages.txt + vypis na vystup
    //Prijme spravu >> vystup
    //Odregistruje sa (rovnako ako sa registruje) >> Unregister

    exit(EXIT_SUCCESS);
}


