
#ifndef reciev 
#define recive
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include "clientF.h"
char* receive(int sock_fct){

	char dataRecv[1024];
	int byte_count;
	int len = sizeof(dataRecv);

	do{//primim datele intr-o bucla

          	byte_count = recv(sock_fct, dataRecv, len, 0);
	  //verificam rezultatul variabilei byte_count 
         	 if(byte_count < 0){
                	 perror("recv()");
                	 close(sock_fct);
                	 return NULL;

         	}//end if


        	 if(byte_count>0){
         	 	char *dataRecvP =NULL;
	  		dataRecvP= (char *)malloc(strlen(dataRecv)+1);//cream pointer pentru a trimite datele la server
	  		strcpy(dataRecvP, dataRecv);//punem datele primite in pointer

        	 	memset(dataRecv, 0, sizeof dataRecv);//golim bufferul
     	 		return dataRecvP;//returnam datele primite(pagina web) catre
		//serverul nostru
          	}//end if
         	 if (byte_count==0){
             	         printf("Teoretic s-a receptionat mesajul\n");
			 return NULL;
         	 }//end if

        	  memset(dataRecv, 0, sizeof dataRecv);
	}while(byte_count!=0);//end do-while


return NULL;
}
#endif
