/* Author:      Marek Marusic, xmarus05
*  Email:       xmarus05@stud.fit.vutbr.cz
*  Purpose:     Project to ISA
*  Description: Create client for SIP protokol
*  File:        file_io.cpp
*/

#include "main.h"
#include <openssl/md5.h>

int readOptions(std::string filename, s_options *newOptions) {

    //Get informations from file
    std::ifstream file(filename.c_str());

    //Parsing lines
    std::string delimiter = "=";
    size_t pos = 0;
    std::string optionName, optionValue;


    if (!file.is_open()) {
        std::cerr << "Error: Can't open file: " << filename << std::endl;
        return 1;
    }

    for (std::string line; getline(file, line);) {
        pos = 0;
        newOptions->expires = 0;
        optionName = "";
        optionValue = "";
        pos = line.find(delimiter);
        optionName = line.substr(0, pos);
        optionValue = line.substr(pos + 1, line.length());

        if (optionName == "server") {
            //std::cout << optionName << " = " << optionValue << std::endl;

            size_t pos2 = optionValue.find(":");
            if(pos2 != std::string::npos){

                try {
                    newOptions->port = stoi(optionValue.substr(pos2 + 1, optionValue.length() - pos2));
                }
                catch(std::invalid_argument& e){
                    // if no conversion could be performed
                    std::cerr << "Error: Unexpected format of file - malformed port" << std::endl;
                    return 1;
                }
                catch(std::out_of_range& e){
                    // if the converted value would fall out of the range of the result type
                    // or if the underlying function (std::strtol or std::strtoull) sets errno
                    // to ERANGE.
                    std::cerr << "Error: Unexpected format of file - port out of range" << std::endl;
                    return 1;
                }
                catch(...) {
                    // everything else
                    std::cerr << "Error: Unexpected format of file" << std::endl;
                    return 1;
                }
                if(newOptions->port > 65535 || newOptions->port < 0){
                    std::cerr << "Error: Unexpected format of file - port out of range" << std::endl;
                    return 1;
                }

                newOptions->server = optionValue.substr(0,pos2);
            }
            else{
                newOptions->port = 5060;
                newOptions->server = optionValue;
            }
        }
        else if (optionName == "username") {
            //std::cout << optionName << " = " << optionValue << std::endl;
            newOptions->username = optionValue;
        }
        else if (optionName == "password") {
            //std::cout << optionName << " = " << optionValue << std::endl;
            newOptions->password = optionValue;
            //Create md5 hash of password
        }
        else if (optionName == "expires") {
            //std::cout << optionName << " = " << optionValue << std::endl;
            //To int
            newOptions->expires = std::stoi(optionValue);
            if(newOptions->expires < MinTimeLeft )
                newOptions->expires = 3*SocketTimeout;
        }
        else {
            std::cerr << "Error: Unexpected format of file" << std::endl;
            return 1;
        }
    }

    if( newOptions->username == "" || newOptions->password == "" || newOptions->server == "" || newOptions->expires == 0){
        std::cerr << "Error: Unexpected format of file" << std::endl;
        return 1;
    }

    file.close();

    return 0;

}


int readMessages(std::string filename, std::vector<s_message> *messages) {

    //Get informations from file
    std::ifstream file(filename.c_str());

    //Parsing lines
    std::string delimiter = " ";
    std::string delimiter2 = "@";
    std::string msgUsrAddr = "";
    size_t pos;
    s_message msg;     //contains dst , context

    if (!file.is_open()) {
        std::cerr << "Error: Can't open file: " << filename << std::endl;
        return 1;
    }


    for (std::string line; getline(file, line);) {
        //vynulujeme
        pos = std::string::npos;
        msg.dst = "";
        msg.context = "";
        msg.usr = "";
        msg.port = 5060;
        msgUsrAddr = "";
        pos = line.find(delimiter);

        if (pos == std::string::npos) {
            std::cerr << "Error: File \"" << filename << "\" is in wrong format!!" << std::endl;
            return 1;
        }
        msg.context = line.substr(pos + 1, line.length());

        // bob@10.10.10.1:5060 rozdelime na bob a IP a port
        msgUsrAddr = line.substr(0, pos);
        pos = msgUsrAddr.find(delimiter2);

        if (pos == std::string::npos) {
            std::cerr << "Error: File \"" << filename << "\" is in wrong format!!" << std::endl;
            return 1;
        }

        msg.usr = msgUsrAddr.substr(0, pos);
        msg.dst = msgUsrAddr.substr(pos + 1, msgUsrAddr.length());

        pos = msg.dst.find(":");
        if(pos != std::string::npos){

            try {
                msg.port = stoi(msg.dst.substr(pos + 1, msg.dst.length() - pos));
            }
            catch(std::invalid_argument& e){
                // if no conversion could be performed
                std::cerr << "Error: Unexpected format of file - malformed port" << std::endl;
                return 1;
            }
            catch(std::out_of_range& e){
                // if the converted value would fall out of the range of the result type
                // or if the underlying function (std::strtol or std::strtoull) sets errno
                // to ERANGE.
                std::cerr << "Error: Unexpected format of file - port out of range" << std::endl;
                return 1;
            }
            catch(...) {
                // everything else
                std::cerr << "Error: Unexpected format of file" << std::endl;
                return 1;
            }
            if(msg.port > 65535 || msg.port < 0){
                std::cerr << "Error: Unexpected format of file - port out of range" << std::endl;
                return 1;
            }
            msg.dst = msg.dst.substr(0,pos);
        }

        messages->push_back(msg);
        //std::cout << msg.usr << msg.dst << " " << msg.context << std::endl;
    }

    file.close();

    return 0;


}
