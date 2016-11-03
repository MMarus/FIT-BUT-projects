/* Author : Marek Marusic, xmarus05
*  Email:  xmarus05@stud.fit.vutbr.cz
*  purpose: Projekt1 to IPK
*  description : Create client-server protokol
*  file: server.cpp
*/


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <iostream>
#include <pwd.h>
#include <sstream>
#include <algorithm>

#define MaxBufferLength 76

void printHelp();
void doprocessing(int sock);
std::string getMSG( int i, char *buffer, struct passwd * userInfo);

int main(int argc, const char *argv[])
{
  int port;
  if(argc == 2 && ( (strcmp(argv[1],"-help") == 0) ||  (strcmp(argv[1],"--help") == 0) ) )
    printHelp();
  else if( argc == 3 && (strcmp(argv[1],"-p") == 0) ){
    char* end = NULL;

    port = strtol(argv[2],&end,10);
    if( *end != 0 ){
      std::cerr <<" Wrong arguments entered, your port is not numeric" << std::endl;
      return 1;
    }
    if( port < 1024 || port > 65535 ){
      std::cerr <<"Port is too big or too small, use number from interval 1024 to 65535" << std::endl;
      return 1;
    }
    std::cout << "Entered port = " << port << std::endl;
  }
  else {
    std::cerr << "Wrong arguments entered, use ./server -p <number_of_port>" << std::endl;
    return 1;
  }

  int sockfd, newSockfd, pid;
  //struct for address
  struct sockaddr_in serverAddr;

  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0 ) ) < 0) { /* create socket*/
    std::cerr << "Error on socket" << std::endl;
    return -1;
  }

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port); /*Port from args*/
  serverAddr.sin_addr.s_addr  = INADDR_ANY;   /*use MY IP */

  if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr) ) < 0 ) {
    std::cerr << "Error on bind" << std::endl;
    return -1;
  }
  if (listen(sockfd, 5)) {
    std::cerr << "Error on listen" << std::endl;
    return -1;
  }

  unsigned int serverAddrLen= sizeof(serverAddr);
  //spusti server
  while (1){

    newSockfd = accept(sockfd, (struct sockaddr *) &serverAddr, &serverAddrLen);
    if (newSockfd < 0){
      std::cerr << "Error on accept" << std::endl;
      return -1;
    }

    pid = fork();
    if (pid < 0){
      std::cerr << "Error on Fork" << std::endl;
      return -1;
    }

    if (pid == 0){
      /* This is the client process */
      close(sockfd);
      doprocessing(newSockfd);
      exit(0);
    }
    else{
      close(newSockfd);
    }

  } /* end of while */



  return 0;
}

void printHelp(){
  std::cout << "Author: Marek Marusic, xmarus05" << std::endl << "Use -p <number of port>" << std::endl;
}

void doprocessing(int sock){
  //Name max = 32 chars, options = max 7
  int n;
  char buffer[MaxBufferLength];
  std::string login;
  std::string startProtocol = "<?xmarus05-start?>";
  std::string endProtocol = "<?xmarus05-end?>";

  //Vyunulujeme buffer a login string
  bzero(buffer,MaxBufferLength);
  login.clear();

  //Precitame z clienta data do bufferu, buffer zvecsime o xx znakov,
  //na zaciatok pridame <<xmarus05-start>>
  //a na koniec spravy pridame <<xmarus05-end>>
  n = read(sock,buffer,MaxBufferLength);
  if (n < 0){
    std::cerr << "Error on read" << std::endl;
    exit(-1);
  }

// Dokym nedostane end alebo prazdny buffer ??
  while( buffer[0] != '\0' && strcmp("<?xmarus05-start?>end<?xmarus05-end?>",buffer) != 0 ){

    //DEBUG vytlac data co som dostal
    printf("Here is the message: %s\n",buffer);

    //Ak je chyba nevypisuj ziadne z -LUGNHS
    bool error = false;
    //Specifies whether Login or UID
    char Type;
    int i = 0;

    std::string startMsg = "";
    while(buffer[i] != '>'){
      startMsg += buffer[i];
      i++;
    }
    startMsg += buffer[i];
    i++;
    std::cout << "start msg = " << startMsg << std::endl;
    if(startMsg.compare(startProtocol) == 0){
      //Get uid / login, delimieter = ,


      //struktura pre info z passwd suboru
      struct passwd * userInfo;
      std::string msg = "";
      Type = buffer[i];
      i += 2;
      std::cout << "Type = " << Type << std::endl;

      while(buffer[i] != ','){
        login += buffer[i];
        i++;
      }
      std::cout << "login = " << login << std::endl;

      // Vyber z passwd bud U alebo L
      if( Type == 'l' ){
       //daj to na male pismenka
        std::transform(login.begin(), login.end(), login.begin(), ::tolower);

        setpwent();
        while ( ( userInfo = getpwent()) != NULL){
          std::string pw_name(userInfo->pw_name);
          std::transform(pw_name.begin(), pw_name.end(), pw_name.begin(), ::tolower);
          if( pw_name.compare(login) == 0 ){
            std::cerr << "error =" << error << std::endl;
            if( error == false){
              std::cout << "FOUND SOMETHING" << std::endl;
              msg += getMSG(i,buffer,userInfo);
              std::cout << msg << std::endl;
            }
          }
        }
        endpwent();

        if( msg.empty() ){
          std::cerr << "Chyba: neznamy login " << login << std::endl;
          msg += "Chyba: neznamy login ";
          msg += login;
          msg += "\n";
          n = write(sock,msg.c_str(),msg.length()+1);
          error = true;
        }
      }


      else if( Type == 'u' ){
        char *end;
        uid_t uid;
        uid = strtol(login.c_str(),&end,10);
        if( *end != 0 ){
          std::cerr <<" Wrong arguments entered, your UID is not numeric" << std::endl;
          continue;
        }
//        uid = std::stoi(login);

        setpwent();
        while ( ( userInfo = getpwent()) != NULL){
          if( uid == userInfo->pw_uid ){
            if( error == false){
              msg += getMSG(i,buffer,userInfo);
            }
          }
        }
        endpwent();

//        userInfo = getpwuid(uid);
        if( msg.empty() ){
          std::cerr << "Chyba: neznamy uid " << uid << std::endl;
          msg += "Chyba: neznamy uid ";
          msg += login;
          msg += "\n";
          n = write(sock,msg.c_str(),msg.length()+1);
          error = true;
        }
      }
      else {
        std::cerr << "Chyba: ani UID ani login nezadany" << std::endl;
        error = true;
      }

      //Do msg pridaj data podla argumentov

      if (error == false){    
        //Posli msg clientovi
        n = write(sock,msg.c_str(),msg.length()+1);
        if (n < 0){
          perror("ERROR writing to socket");
          //exit(1);
        }
      }
   

    }
    //Vynulujeme buffer a login
    bzero(buffer,MaxBufferLength);
    login.clear();

    //Znova si do bufferu nove data nacitame
    n = read(sock,buffer,MaxBufferLength);
    if (n < 0){
      std::cerr << "Error on read" << std::endl;
      exit(-1);
    }
  }
/// End of while read is not end
}


std::string getMSG( int i, char *buffer, struct passwd * userInfo){
  std::string msg;
  std::stringstream ss;

  while( buffer[++i] != '<'){
    std::cout << buffer[i] << std::endl;
    switch (buffer[i]) {
      //Login
      case 'L':
        msg += userInfo->pw_name;
        msg += " ";
        break;
      //UID
      case 'U':
        ss.str(std::string());
        ss << userInfo->pw_uid;
        msg += ss.str();
        msg += " ";
        break;
      //GID
      case 'G':
        ss.str(std::string());
        ss << userInfo->pw_gid;
        msg += ss.str();
        msg += " ";
        break;
      case 'N':
        msg += userInfo->pw_gecos;
        msg += " ";
        break;
      case 'H':
        msg += userInfo->pw_dir;
        msg += " ";
        break;
      case 'S':
        msg += userInfo->pw_shell;
        msg += " ";
        break;
    }
  }

  //Delete poslednu medzeru
  msg.pop_back();
  msg += '\n';
  return msg;
}

struct passwd * FindUserName(char * name){
  std::string Name(name);
  struct passwd * pw;
  setpwent();

  while ((pw = getpwent()) != NULL){
    std::string pw_name(pw->pw_name);
    std::transform(pw_name.begin(), pw_name.end(), pw_name.begin(), ::tolower);
    if( pw_name.compare(Name) == 0 ){
      endpwent();
      return pw;
    }
  }

  endpwent();
  return NULL;
}
