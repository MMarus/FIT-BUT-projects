/* Author : Marek Marusic, xmarus05
*  Email:  xmarus05@stud.fit.vutbr.cz
*  purpose: Projekt1 to IPK
*  description : Create client-server protokol
*  file: client.cpp
*/


#include <iostream>
#include <unistd.h>
#include <vector>
#include <algorithm> // for copy
#include <iterator> // for ostream_iterator
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <queue>
#include <climits>


//TODO: osetrit ked neni ziaden z -LUGNHS

int main(int argc, char **argv)
{
  int ch;
  bool l = false;
  bool u = false;

  int port = 0;
  std::string options;
  options.clear();
  std::queue<std::string> logins;
  std::string hostname;

  while ((ch = getopt(argc, argv, "h:p:l:u:LUGNHS")) != -1) {
    switch (ch) {
      case 'h':{
        hostname = optarg;
        break;
      }
      case 'p':{
        char *end;
        port = strtol(optarg,&end,10);
        if( *end != 0 ){
          std::cerr << "Wrong arguments entered, your port is not numeric" << std::endl;
          return 1;
        }
        if( port < 1024 || port > 65535 ){
          std::cerr <<"Port is too big or too small, use number from interval 1024 to 65535" << std::endl;
          return 1;
        }
        break;
      }
      case 'l':{
        if( u == true || l == true){
          u = false;
          //Empty queue of logins
          while(!logins.empty()) logins.pop();
        }
        l = true;
        //login += "l=";
        optind--;
        for( ;optind < argc && *argv[optind] != '-'; optind++){
          logins.push(argv[optind]);
        }
        break;
      }
      case 'u':{
        if( u == true || l == true ){
          l = false;
          //Empty queue of logins
          while(!logins.empty()) logins.pop();
        }
        u = true;
        //login += "u=";
        optind--;
        for( ;optind < argc && *argv[optind] != '-'; optind++){
          logins.push(argv[optind]);
        }

        break;
      }
      case 'L':{
        options += "L";
        break;
      }
      case 'U':{
        options += "U";
        break;
      }
      case 'G':{
        options += "G";
        break;
      }
      case 'N':{
        options += "N";
        break;
      }
      case 'H':{
        options += "H";
        break;
      }
      case 'S':{
        options += "S";
        break;
      }
      default:
        std::cerr << "Wrong arguments entered, use ./server -p <number_of_port>" << std::endl;
        return 1;
        break;
    }
  }

  if( u == false && l == false){
    std::cerr << "Chyba: You didn't entered any uid nor login" << std::endl;
    return 1;
  }

  if ( port == 0 ){
    std::cerr << "Chyba: No port entered" << std::endl;
    return 1;
  }

  if( hostname.empty()){
    std::cerr << "Chyba: No hostname entered" << std::endl;
    return 1;
  }

  if(options.empty()){
    options = "LUGNHS";
  }

  int i;
  int sockfd;
  int bytes;
  unsigned int num;
  struct sockaddr_in cli_addr;
  struct hostent *hptr;
  char msg[1000];
  std::string startMsg = "";

  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0 ) ) < 0) { /* create socket*/
    std::cerr << "Error on socket" << std::endl;
    return -1;
  }

  cli_addr.sin_family = AF_INET;              /*set protocol family to Internet */
  cli_addr.sin_port = htons(port);  /* set port no. */
  if ( (hptr =  gethostbyname(hostname.c_str()) ) == NULL){
    std::cerr << "gethostname error: " << hostname << std::endl;
    return -1;
   }
  memcpy( &cli_addr.sin_addr, hptr->h_addr, hptr->h_length);

  if (connect (sockfd, (struct sockaddr *)&cli_addr, sizeof(cli_addr) ) < 0 ){
    std::cerr << "Error on Connect" << std::endl;
    return -1;
  }

  //Precitame z clienta data do bufferu, buffer zvecsime o xx znakov,
  //na zaciatok pridame <<xmarus05-start>>
  //a na koniec spravy pridame <<xmarus05-end>>
  std::string startProtocol = "<?xmarus05-start?>";
  std::string endProtocol = "<?xmarus05-end?>";


//While we have Logins or UIDs
  while(! logins.empty()){
    usleep(100000);
    std::string login = logins.front();
    logins.pop();
    std::string data = startProtocol;



    if( l == true){
      if( login.length() > 32){
        std::cerr << "Error: Login too long, unix systems support up to 32chars in login" << std::endl;
        continue;
      }
      data += "l=";
    }
    else if (u == true){
      char *end;
      num = strtol(login.c_str(),&end,10);
      if( *end != 0 ){
      std::cerr <<" Wrong arguments entered, your UID is not numeric" << std::endl;
      continue;
      }
      if(num >= INT_MAX-2){
        std::cerr <<" Wrong arguments entered, your UID is too big" << std::endl;
        continue;
      }
      data += "u=";
    }
    else{
      std::cerr << "Error: no Logins or Uids entered" << std::endl;
    }

    data += login;
    data += ",";
    data += options;
    data += endProtocol;

    if ( write(sockfd, data.c_str(), data.length() +1) < 0 ) {  /* send message to server */
      std::cerr << "error on write" << std::endl;
      return -1; /*  write error */
    }

    if ( ( bytes = read(sockfd, msg, sizeof(msg) ) ) <0) {  /* read message from server */
      std::cerr << "error on read" << std::endl;
      return -1; /*  read error */
    }

    startMsg.clear();
    for(i=0; i < 6 && msg[i] != '\0'; i++){
      startMsg += msg[i];
    }
    if( startMsg.compare("Chyba:") == 0)
      std::cerr << msg;
    else
      std::cout << msg;

    if( logins.empty() ){
      data = startProtocol;
      data += "end";
      data += endProtocol;
      if ( write(sockfd, data.c_str(), data.length() +1) < 0 ) {  /* send message to server */
        std::cerr << "error on write" << std::endl;
        return -1; /*  write error */
      }
    }
  }
//END of While we have Logins or UIDs


  /* close connection, clean up socket */
  if (close(sockfd) < 0) {
    std::cerr << "error on close" << std::endl;   /* close error */
    return -1;
  }



  return 0;
}
