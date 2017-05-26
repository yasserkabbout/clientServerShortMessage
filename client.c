#include<stdio.h>
#include<sys/types.h>//socket
#include<sys/socket.h>//socket
#include<string.h>//memset
#include<stdlib.h>//sizeof
#include<netinet/in.h>//INADDR_ANY
#include <time.h>
#include "iniparser/src/iniparser.h" // For ini files
#define MAXSZ 200
/* */ 
int port;
char *serverI;
char serverIp[100];
char *UserIdchange;
char *passIdchange;
char tUserIdchange[100];
char tpassIdchange[100];
char updateiniString[100];
 char userId[100];
 char pass[100];
     //let's see if the
            char date[10];
             char user[5];
             char size[5];
             char result[200];
             int len,pos,c=0,j;
             int i;
             

void access_client_ini(char * ini_name);
void update_client_ini(char * ini_name,int selection);
void auth_client_ini(char * ini_name);
int main()
{
    access_client_ini("client.ini"); 
 int sockfd;//to create socket

 struct sockaddr_in serverAddress;//client will connect on this

 int n,authFlag=1,menueFlag=1,menueOption;
 char msg1[MAXSZ];
 char msg2[MAXSZ];
 char AuthResult[100];

    char sendToWho[MAXSZ];
   char msgSendToWho[MAXSZ];
    char confirmSend[2];
               char drOption[1];
           int drID;
               char checkForDelet[2];
               char readedMsg[100];
    //

 //create socket
 sockfd=socket(AF_INET,SOCK_STREAM,0);
 //initialize the socket addresses
 memset(&serverAddress,0,sizeof(serverAddress));
 serverAddress.sin_family=AF_INET;
 serverAddress.sin_addr.s_addr=inet_addr(serverIp);
 serverAddress.sin_port=htons(port);

 //client  connect to server on port
 connect(sockfd,(struct sockaddr *)&serverAddress,sizeof(serverAddress));
 //send to sever and receive from server
  
 while(1)
 {
         
        
           //Greetings message
         n=recv(sockfd,msg2,MAXSZ,0);
          if (recv > 0) 
          printf("s: %s\n ",msg2);        
           //end of greetings 
    
    //Authniction data
    while(authFlag==1)
    {
    printf("Enter your User ID : ");
    fgets(userId,MAXSZ,stdin);
    if(userId[0]=='#')
    break;
    n=strlen(userId)+1;
    send(sockfd,userId,n,0);
    
    printf("Enter your Password : ");
        fgets(pass,MAXSZ,stdin);
    if(pass[0]=='#')
    break;
    n=strlen(pass)+1;
    send(sockfd,pass,n,0);
    
     
   //end of authnication 
  //result of the Provided Authnication
           n=recv(sockfd,msg2,MAXSZ,0);
           if (recv > 0) 
           strcpy(AuthResult,msg2);
           printf("s : %s\n",AuthResult);
           if(strlen(AuthResult)<40)
           authFlag=-1;

           }
           int menueFlag=1;
            auth_client_ini("client.ini");
            while(menueFlag!=-1)
            {
           printf("Hello %s\nPlease choose your option:\n1)Read/Delete Messages\n2)Write Message to User\n3)Change config parameters\n4)Quit\nOption->",userId);
           scanf("%d",&menueOption);
           if(menueOption==1)
           {
          strcpy(msg2,"");
           n=1;
          send(sockfd,"1",n,0); 
          //recv file names
         int counter=0;
         int begin1=0;
         int begin2;
         int end;
         char fromWho[begin1];
         char dateUnixaa[10];
         int beginFlag=0;
         char msgs[100][100];
          while(strcmp(msg2,"finish")!=0){
          
          n=recv(sockfd,msg2,MAXSZ,0);
        
          if (recv > 0) 
          {
          if((strcmp(msg2,"finish")!=0)&&(counter!=0)&&(counter!=1))
           {
            end=strlen(msg2);
            
             for(i=begin1;i<strlen(msg2);i++)
             {
                     if(msg2[i]=='_'&&(beginFlag==0))
                     {
                     begin1=i;
                     beginFlag=1;
                     }
                     if(msg2[i]=='_')
                     begin2=i;
             }
             strcpy(msgs[counter-2],msg2);
            // printf("\n%d)from: %s",counter-1,msgs[counter-2]); 
             strncat(result,date,10);  
             for(c=0;c<5;c++)
             {
              user[c]=msgs[counter-2][c+11];
             }   
            // printf("user %s",user);
            strncat(result,"From:",5); 
             strncat(result,user,5); 
              strncat(result,",",1); 
             for(c=0;c<10;c++)
             {
              date[c]=msgs[counter-2][c];
             }   
            // printf("date %s",date);
              char date2[10];
              strncpy(date2,date,10);
             //time convertor
             //time_t     now;
              struct tm  ts;
              char       dateConverted[80];
          // Get current time
       time_t now = (time_t) strtol(date2, NULL, 10); 
           // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
            ts = *localtime(&now);
                 strftime(dateConverted, sizeof(dateConverted), "%Y-%m-%d %H:%M:%S", &ts);
                 strncat(result,"Date:",5);
                 strcat(result,dateConverted);
                  strncat(result,",",1); 
                // printf("\n%s\n",dateConverted);

             //end of time
     
              for(c=0;msgs[counter-2][c+17]!='.';c++)
             {
              size[c]=msgs[counter-2][c+17];
             }   
            // printf("size %s",size); 
            strncat(result,"Size:",5);
             strncat(result,size,2); 
             strncat(result,"Byte.",5);

                    printf("\n%d) %s",counter-1,result);           
                 
             }//
            counter++;
           }
           }//while
           strcpy(msg2,"");
           strcpy(readedMsg,"");
           printf("\n");
           printf("Type R for Reading, D for deleting ->");
           scanf("%s",&drOption);
           n=2;
           send(sockfd,drOption,n,0); 
           printf("\nType ID of the message");
           scanf("%d",&drID);
           if(drOption[0]=='R'||drOption[0]=='r')
           {
           for(j=0;j<=counter;j++)
            {
                 if(drID==j)
                 {
                 // printf("%s we are here \n",msgs[j-1]);
                  n=strlen(msgs[j-1])+1;
                 send(sockfd,msgs[j-1],n,0);
            strcpy(msg2,"");
            strcpy(readedMsg,"");
            n=recv(sockfd,msg2,MAXSZ,0);
            if (recv > 0) 
            {
            strcpy(readedMsg,msg2);
            printf("Your Message :%s   \n",readedMsg);    
            }
                 }
            }

              
           }
           if(drOption[0]=='D'||drOption[0]=='d')
           {

           for(j=0;j<=counter;j++)
            {
              if(drID==j)
              {
                 printf("Are you sure that you want to delet message<%d> (Y OR N)\n",j);
                 scanf("%s",&checkForDelet);
                 n=2;
          send(sockfd,checkForDelet,n,0);
                 if(checkForDelet[0]=='y'||checkForDelet[0]=='Y')
                 {
                  printf("The file has been removed successfully\n");
                  n=strlen(msgs[j-1])+1;
                 send(sockfd,msgs[j-1],n,0);
                 }
                if(checkForDelet[0]=='N'||checkForDelet[0]=='n')
                 {
                  printf("we have cancelled delete operation\n");
                 }

              
            }
           }
           }//finish compare
               menueOption=9;
           }
           if(menueOption==2)
              {
        n=1;
    send(sockfd,"2",n,0); 

        printf("Give Target UserId: ");
    //fgets(msgSendToWho,MAXSZ,stdin);
    scanf("%s",&sendToWho);

    n=strlen(sendToWho)+1;
    send(sockfd,sendToWho,n,0);    

   printf("Please Type Text: ");
    //fgets(msgSendToWho,MAXSZ,stdin);
    scanf("%s",&msgSendToWho);
    if(msgSendToWho[0]=='#')
    break;
    n=strlen(msgSendToWho)+1;
    send(sockfd,msgSendToWho,n,0);

   printf("Would you like to Send message to %s? (Y/N)",sendToWho);
    //fgets(msgSendToWho,MAXSZ,stdin);
    scanf(" %s",&confirmSend);
    if(confirmSend[0]=='#')
    break;
    n=strlen(confirmSend)+1;
    send(sockfd,confirmSend,n,0);

       }//option 2
       if(menueOption==3)
       {
       access_client_ini("client.ini");
          int selection;
          char checkSelection[2];
         printf("Would you change any following value?\n1)TargetServer : %s\n2)TargetPort : %d\n3)UserID : %s\n4)Passwd : %s\nOption->",serverIp,port,tUserIdchange,tpassIdchange);
       scanf("%d",&selection);
       if(selection==1)
       {
       printf("\nPlease give new TargetServer->");
       scanf("%s",&updateiniString);
       printf("\nDo you want to use this new value? (Y/N))");
       scanf("%s",&checkSelection);
       if(checkSelection[0]=='y'||checkSelection[0]=='Y')
        update_client_ini("client.ini",selection);
        }
               if(selection==2)
       {
       printf("\nPlease give new TargetPort ->");
       scanf("%s",&updateiniString);
       printf("\nDo you want to use this new value? (Y/N))");
       scanf("%s",&checkSelection);
       if(checkSelection[0]=='y'||checkSelection[0]=='Y')
        update_client_ini("client.ini",selection);
        }
        if(selection==3)
       {
       printf("\nPlease give new UserID  ->");
       scanf("%s",&updateiniString);
       printf("\nDo you want to use this new value? (Y/N))");
       scanf("%s",&checkSelection);
       if(checkSelection[0]=='y'||checkSelection[0]=='Y')
       {
        update_client_ini("client.ini",selection);
          n=strlen(updateiniString)+1;
         send(sockfd,updateiniString,n,0);
        }
        }
                               if(selection==4)
       {
       printf("\nPlease give new Passwd  ->");
       scanf("%s",&updateiniString);
       printf("\nDo you want to use this new value? (Y/N))");
       scanf("%s",&checkSelection);
       if(checkSelection[0]=='y'||checkSelection[0]=='Y')
        update_client_ini("client.ini",selection);
        }
       }
       if(menueOption==4)
       {
                                 n=1;
    send(sockfd,"4",n,0); 
              menueFlag=-1;
         printf("\nSee you in Capstone Project \n"); //finally :) alhamdollah
                       

       }

            }

  
  
 }


 return 0;
}
void access_client_ini(char * ini_name)
{
     strcpy(serverIp,"");
     strcpy(tUserIdchange,"");
     strcpy(tpassIdchange,"");
     
    dictionary  *   ini ;
     
    ini = iniparser_load(ini_name);
    if (ini==NULL) {
        fprintf(stderr, "cannot parse file: %s\n", ini_name);
        
    }
    iniparser_dump(ini, stderr);

    port = iniparser_getint(ini, "client:TargetPort", -1);

    serverI=iniparser_getstring(ini, "client:TargetServer", NULL);
    
    strncpy(serverIp, serverI,  strlen(serverI));
 
     UserIdchange=iniparser_getstring(ini, "client:UserID", NULL);
    
    strncpy(tUserIdchange, UserIdchange,  strlen(UserIdchange));
    
        passIdchange=iniparser_getstring(ini, "client:Passwd", NULL);
    
    strncpy(tpassIdchange, passIdchange,  strlen(passIdchange));
}
void update_client_ini(char * ini_name,int selection)
{

     
         dictionary  *   ini ;

    ini = iniparser_load(ini_name);
    if (ini==NULL) {
        fprintf(stderr, "cannot parse file: %s\n", ini_name);
        
    }
               FILE *finp = fopen("client.ini", "w");
      fclose(finp);
    if(selection==1)
    {
iniparser_set(ini,"client:TargetServer",updateiniString);
finp = fopen("client.ini", "w");
iniparser_dump_ini(ini, finp);
fclose(finp);
    }
    if(selection==2)
    {
iniparser_set(ini,"client:TargetPort",updateiniString);
finp = fopen("client.ini", "w");
iniparser_dump_ini(ini, finp);
fclose(finp);
    }
        if(selection==3)
    {
iniparser_set(ini,"client:UserID",updateiniString);
finp = fopen("client.ini", "w");
iniparser_dump_ini(ini, finp);
fclose(finp);
    }
        if(selection==4)
    {
iniparser_set(ini,"client:Passwd",updateiniString);
finp = fopen("client.ini", "w");
iniparser_dump_ini(ini, finp);
fclose(finp);
    }
    	
 
}
void auth_client_ini(char * ini_name)
{

     
         dictionary  *   ini ;

    ini = iniparser_load(ini_name);
    if (ini==NULL) {
        fprintf(stderr, "cannot parse file: %s\n", ini_name);
        
    }
               FILE *finp = fopen("client.ini", "w");
      fclose(finp);

iniparser_set(ini,"client:UserID",userId);
finp = fopen("client.ini", "w");
iniparser_dump_ini(ini, finp);
fclose(finp);

iniparser_set(ini,"client:Passwd",pass);
finp = fopen("client.ini", "w");
iniparser_dump_ini(ini, finp);
fclose(finp);
}
