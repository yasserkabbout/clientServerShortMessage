 
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Socket API headers */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "iniparser/src/iniparser.h" // For ini files
#include <time.h>
#include <dirent.h> 
/* Definations */
#define DEFAULT_BUFLEN 512
  void make_user_folder();
void access_server_ini(char * ini_name);
void do_job(int fd);
 int port;//Port number taken from access_server_ini function
 char *serverMsg; 
 char serverM[100];
  char errorAuth[100];
 int correct_auth=0;
  //Authniction Data
  int authentication_processing(char * ini_nam);
  char *user; 
  char userId[5];//recieved it from the client.c
  char *pass; 
  char userPass[100];//recieved it from the client.c
                  char userNeed[5];
                 char toWhichUser[10];
                 char msgToBeStored[100];
                 char confirmSend[2];
                              char rORd[2];
             char chFd[2];
             char msgToBeDeleted[100];


int main()
{
int server, client;
struct sockaddr_in local_addr;
struct sockaddr_in remote_addr;
int length,fd,rcnt,optval;
pid_t pid;
 access_server_ini("server.ini"); 
/* Open socket descriptor */
if ((server = socket( AF_INET, SOCK_STREAM, 0)) < 0 ) { 
    perror("Can't create socket!");
    return(1);
}
strcpy(errorAuth,"Error");

/* Fill local and remote address structure with zero */
memset( &local_addr, 0, sizeof(local_addr) );
memset( &remote_addr, 0, sizeof(remote_addr) );

/* Set values to local_addr structure */
local_addr.sin_family = AF_INET;
local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
local_addr.sin_port = htons(port);

// set SO_REUSEADDR on a socket to true (1):
optval = 1;
setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

if ( bind( server, (struct sockaddr *)&local_addr, sizeof(local_addr) ) < 0 )
{
    /* could not start server */
    perror("Bind error");
    return(1);
}

if ( listen( server, SOMAXCONN ) < 0 ) {
        perror("listen");
        exit(1);
}

printf("Concurrent  socket server now starting on port %d\n",port);
printf("Wait for connection\n");

while(1) {  // main accept() loop
    length = sizeof remote_addr;
    if ((fd = accept(server, (struct sockaddr *)&remote_addr, \
          &length)) == -1) {
          perror("Accept Problem!");
          continue;
    }

    printf("Server: got connection from %s\n", \
            inet_ntoa(remote_addr.sin_addr));

    /* If fork create Child, take control over child and close on server side */
    if ((pid=fork()) == 0) {
        close(server);
        do_job(fd);
        char userIdd[100];
        strncpy(userIdd,userId,strlen(userId)-1);
        printf("%s End his connection Successfully  \n",userIdd);
        close(fd);
        exit(0);
    }

}

// Final Cleanup
close(server);

}
void do_job(int fd) {
int length,rcnt,n,authFlag=-1;;
char recvbuf[DEFAULT_BUFLEN],bmsg[DEFAULT_BUFLEN];
int  recvbuflen = DEFAULT_BUFLEN;

    // Receive until the peer shuts down the connection
    do {
   
        //sending Greetings message

         
         n=strlen(serverM)+1;
       send( fd, serverM, n, 0 );
           
            //end of sending greetings message 
           
            while(authFlag==-1)
            {
       
            /*optaining the authniction Data from the user*/
          
         n = recv(fd, recvbuf, recvbuflen, 0);
        if (recv > 0) 
          strcpy(userId, recvbuf); 
            printf("UserId: %s\n", userId);
           // userId=recvbuf;
         
          //   userPass=recvbuf;
      n = recv(fd, recvbuf, recvbuflen, 0);
        if (recv > 0) 
        strcpy(userPass, recvbuf); 
               printf("UserPass: %s\n", userPass);
           // userId=recvbuf;
            
        if (rcnt == 0)
            printf("Connection closing...\n");
  
       
        int authnictionResult=authentication_processing("server.ini");
        if(authnictionResult==-1)
        {
         //sending error message
      
         printf("\nError in Authnication \n");
  
         strcpy(errorAuth,"-ERR Either UserID or Password is wrong.");
         n=strlen(errorAuth)+1;
         send( fd, errorAuth, n, 0 );
          authFlag=-1;
         correct_auth=-1;
     
        

        }
        else{
         printf("\nCorrect in Authnication \n");
         make_user_folder();
         strcpy(errorAuth,"+OK UserID and password okay go ahead");
         n=strlen(errorAuth)+1;
         send( fd, errorAuth, n, 0 );
              authFlag=0;//The provided Authnication data is correct
              correct_auth=1;

            }
printf("\ncorrect_auth=%d\n",correct_auth);
                            
                                        FILE *auth_attemp;
         auth_attemp=fopen("Authentication_Attempts.txt", "a");
            if(correct_auth==1)
            {

          fprintf(auth_attemp, "\n");
   fprintf(auth_attemp, "\n+GOOD attempt to authenticate from UserId=%s , Password=%s \n",userId,userPass);//wrote good attempt
   fclose(auth_attemp);
            }
        if(correct_auth==-1)
            {

           fprintf(auth_attemp, "\n");
   fprintf(auth_attemp, "\n-Bad attempt to authenticate from UserId=%s , Password=%s \n",userId,userPass);//wrote good attempt
   fclose(auth_attemp);
            }
            }
            /*End Authniction */
              //which option user select 
                while(userNeed[0]!='4'){
     
                  n = recv(fd, recvbuf, recvbuflen, 0);
                 if (recv > 0) 
                 {
                 userNeed[0]=recvbuf[0];
                 }
             //end of understandding the user request
             
            //Handle the client request
                 if (recv > 0) 
                 {
                 userNeed[0]=recvbuf[0];
                 }

            if(userNeed[0]=='1')
              {
            
                   DIR           *d;
                 struct dirent *dir;
                 char storedPath[300]="data/home/20121604/server/";
                 strncat(storedPath,userId,strlen(userId)-1);
                d = opendir(storedPath);
                 if (d)
                  {
              while ((dir = readdir(d)) != NULL)
                {
                    n=strlen(dir->d_name)+1;
                send( fd, dir->d_name, n, 0 );
                  printf("%s\n", dir->d_name);
                 }
                 }
                 if((dir = readdir(d)) == NULL)
                 {
                  n=strlen("finish")+1;
                send( fd, "finish", n, 0 );
              
                 }
                   
              n = recv(fd, recvbuf, recvbuflen, 0);
              if (recv > 0) 
              {
              rORd[0]=recvbuf[0];
               printf("%s",rORd);
               }
               if(rORd[0]=='R'||rORd[0]=='r')
              {
              n = recv(fd, recvbuf, recvbuflen, 0);
              if (recv > 0) 
              {
               strcpy(msgToBeDeleted, recvbuf);
              printf("msg to be read:%s\n",msgToBeDeleted);
                 strcat(storedPath,"/");
              strncat(storedPath,msgToBeDeleted,strlen(storedPath)+strlen(msgToBeDeleted)-2);
               printf("msg to be read:%s\n",storedPath);
                  FILE *fp;
             char readedMsg[100];
               fp = fopen(storedPath, "r");
                fgets(readedMsg, 500, (FILE*)fp);
                printf("readed msg %s\n", readedMsg );
                      n=strlen(readedMsg)+1;
                send( fd,readedMsg , n, 0 );
                fclose(fp);
              }
              }
              if(rORd[0]=='D'||rORd[0]=='d')
              {
              n = recv(fd, recvbuf, recvbuflen, 0);
              if (recv > 0) {
              chFd[0]=recvbuf[0];
              printf("%s",chFd);
              }
              if(chFd[0]=='y'||chFd[0]=='Y')
              {
               printf("we will delete");
               n = recv(fd, recvbuf, recvbuflen, 0);
              if (recv > 0) 
              {
               strcpy(msgToBeDeleted, recvbuf);
              printf("msg to be delet:%s\n",msgToBeDeleted);
              strcat(storedPath,"/");
              strncat(storedPath,msgToBeDeleted,strlen(storedPath)+strlen(msgToBeDeleted)-2);
               printf("msg to be delet:%s\n",storedPath);
              
                  if( remove(storedPath) != 0 )
               perror( "Error deleting file" );
                   else
               printf( "File successfully deleted" );


              }
              
              }
              }

              
                         
              //  printf("\nThe list of msgs \n");

              userNeed[0]=='9';
              }

            if(userNeed[0]=='2')
              {

  // startttttttttttttttttt
           n = recv(fd, recvbuf, recvbuflen, 0);
        if (recv > 0) 
          strcpy(toWhichUser, recvbuf); 
            printf("toWhichUser: %s\n", toWhichUser);
           // userId=recvbuf;
         
          //   userPass=recvbuf;
          char textSize[100];
      n = recv(fd, recvbuf, recvbuflen, 0);
        if (recv > 0) 
        {
        strcpy(msgToBeStored, recvbuf); 
               printf("msgToBeStored: %s\n", msgToBeStored);
             sprintf(textSize, "%d",n);
             }
                     n = recv(fd, recvbuf, recvbuflen, 0);
                     
        if (recv > 0) 
        strcpy(confirmSend, recvbuf); 
               printf("confirmSend: %s\n", confirmSend);
              userNeed[0]='9';
                  if((strncmp(confirmSend, "y", 1)==0)||(strncmp(confirmSend, "Y", 1)==0)){
                  
                    char storedPath[300]="data/home/20121604/server/";
                    char timeStamp[100];
                    sprintf(timeStamp, "%d", (int)time(NULL));
                    strncat(storedPath,toWhichUser,strlen(userId)-1);
                     strcat(storedPath,"/");
                      printf("Timestamp: %s\n",timeStamp);
                     strcat(storedPath,timeStamp);
                     strcat(storedPath,"_");
                    strncat(storedPath,userId,strlen(userId)-1);
                    strcat(storedPath,"_");
                    strcat(storedPath,textSize);
                    printf("\n%s\n",storedPath);
                     FILE *storeMsg;
                    storeMsg=fopen(strcat(storedPath,".msg"), "w");
                     fprintf(storeMsg,"%s \n",msgToBeStored);//wrote good attempt
                     fflush(storeMsg);
                   printf("\n%s\n",storedPath);
                        }

              }
                         
               if(userNeed[0]=='4')
              {
            rcnt=-1;
            userNeed[0]='4';
            }
              }//while user need !=4
          /*  
        rcnt = recv(fd, recvbuf, recvbuflen, 0);
        if (rcnt > 0) 
            printf("Message received: %s\n", recvbuf);

        if (rcnt == 0)
            printf("Connection closing...\n");
            */
            
    } while (rcnt > 0);
}

void access_server_ini(char * ini_name)
{
    dictionary  *   ini ;


    ini = iniparser_load(ini_name);
    if (ini==NULL) {
        fprintf(stderr, "cannot parse file: %s\n", ini_name);
        
    }
    iniparser_dump(ini, stderr);

    port = iniparser_getint(ini, "server:ListenPort", -1);
    printf("%d this is \n", port);
    serverMsg=iniparser_getstring(ini, "server:ServerMsg", NULL);
    strncpy(serverM, serverMsg,  strlen(serverMsg));
    printf("server msg:  %s\n",serverM);
        
}

int authentication_processing(char * ini_nam){
      dictionary  *   ini ;
        char tempPass[100];
    /*char userAttribute[]="users:";
    ///char *userAttributePointer;
    //strcat(userAttribute, userId);
    int ch=0;
    if(ch==0)
    {
    int i,j;
     for(i = 0; userAttribute[i] != '\0'; ++i);

    for(j = 0; userId[j] != '\0'; ++j, ++i)
    {
        userAttribute[i] = userId[j];
    }

    userAttribute[i] = '\0';
    ch=1;
     }

  const char *test=userAttribute;
    ch=2;*/
  
    //strcpy(userAttributePointer, userAttribute);
    ini = iniparser_load(ini_nam);
    if (ini==NULL) {
        fprintf(stderr, "cannot parse file: %s\n", ini_nam);
        
    }
    iniparser_dump(ini, stderr);
   

      int id = atoi(userId);


   if(id==10000)
   {
   //printf("\n we are here");
    pass=iniparser_getstring(ini,"users:10000","no");
    printf("\npass= %s",pass);
    strcpy(tempPass, pass);
        printf("\nuserId*= %s",userId);
        printf("\ntempPass*= %s",tempPass);
         
   }
    if(id==10001)
   {
    pass=iniparser_getstring(ini,"users:10001","no");
    printf("\npass= %s",pass);
    strcpy(tempPass, pass);
        printf("\nuserId*= %s",userId);
        printf("\ntempPass*= %s",tempPass);
   }
         if(id==10002)
   {
    pass=iniparser_getstring(ini,"users:10002","no");
    printf("\npass= %s",pass);
    strcpy(tempPass, pass);
        printf("\nuserId*= %s",userId);
        printf("\ntempPass*= %s\n",tempPass);
   }
    //printf("%s temp pass?",tempPass);
          printf("\n **tempPass= %d \n**userPass= %d\n",strlen(tempPass),strlen(userPass));
    if((strlen(tempPass)+1)!=strlen(userPass)){
        //printf("\n **tempPass= %d \n**userPass= %d\n",tempPass,userPass);
                            return -1;
                            }
        int i;
    for(i=0;i<strlen(tempPass);i++){
     if((tempPass[i]!=userPass[i])){
         //printf("temp=%c userPass=%c %d\n",tempPass[i],userPass[i],i);
         return -1;
         }
         }
         
     return 0;
                             }
void make_user_folder(){
       int id;
     if(id=10000){
   char* userDir="mkdir -p data/home/20121604/server/10000";
   system(userDir);
   }
       if(id=10001){
   char* userDir="mkdir -p data/home/20121604/server/10001";
   system(userDir);
   }
     if(id=10002){
   char* userDir="mkdir -p data/home/20121604/server/10002";
   system(userDir);
   }
                      }
