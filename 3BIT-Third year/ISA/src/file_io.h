/* Author:      Marek Marusic, xmarus05
*  Email:       xmarus05@stud.fit.vutbr.cz
*  Purpose:     Project to ISA
*  Description: Create client for SIP protokol
*  File:        file_io.h
*/

int readOptions(std::string filename, s_options* newOptions);
int readMessages(std::string filename, std::vector<s_message>* messages);

