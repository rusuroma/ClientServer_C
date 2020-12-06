#ifndef clientF
#define clientF
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#define SERVER_PORT 80


int conection()
 {
         int sock_fct;
         struct sockaddr_in6 cl;
         int ret;
         char *msg = "GET / HTTP/1.0\r\n\r\n";
         int len, data_sent;
         char dataRecv[1024];
         int byte_count;
         sock_fct = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);//cream socketul

         if(sock_fct == -1){

                 perror("sock_fct");
                 return EXIT_FAILURE;

         }//end if
        cl.sin6_family = AF_INET6;//tipul adresei socketului
        inet_pton(AF_INET6, "2606:4700::6811:793a", &cl.sin6_addr);//atribuim addresa catre serverul cerut
        cl.sin6_port = htons(SERVER_PORT);//atribuim numarul portului

        ret = connect(sock_fct, (struct sockaddr*)&cl, sizeof(cl));
        if(ret == -1){
                perror("conect()");
                close(sock_fct);
                return EXIT_FAILURE;
	 }//end if

         len = strlen(msg);

         data_sent = send(sock_fct, msg, len, 0);
	 if(data_sent == -1){
                 perror("send()");
                 close(sock_fct);
                 return EXIT_FAILURE;
         }//end if
         //printf("S-a trimis cererea GET...\n");

       
return sock_fct;//returnam socketul creat

 }//end main
#endif
