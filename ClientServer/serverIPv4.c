#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <string.h>
#include "clientF.h"
#include "recv.h"

int main(int argc, char **argv){

	struct sockaddr_in server;
	int sock_fct;
	int bind_fct;
	int list;
	int acc;
	int byte_count;
	char dataRecv[1024],msg[1024];
	int data_sent;



	struct sockaddr cl;
        int len;
        int len2;
        char com[] = "Comanda neimplementata";
        int len3 = sizeof(com);
        FILE *file;//fisier in care scriem datele

		
	
	//se extrage adresa ipv4 din rezultatul comenzii ifconfig si se scrie in fisierul ip1.txt
	system(" ifconfig | grep 'inet ' | cut -f1 > ip1.txt");
	
	char ip[30];
	FILE *fp;
	char *ip2 = strtok(ip," ");
        int k=1;
	fp = fopen("ip1.txt", "r");//fisierul unde scriem rezultatul comenzii ifconfig
	fgets(ip,30,fp);

	while(k>0){
		ip2=strtok(NULL, " ");//exttragem tokenul ce contine doar adresa ip
		k--;
		ip2 = strtok(NULL, ip);
	}
	printf(ip2);
	//membrii structurii server	
	server.sin_family = AF_INET;//ipv4 
	server.sin_addr.s_addr = inet_addr(ip2);//atribuim adresa masinii pe care ruleaza serverul
	server.sin_port = htons(atoi("22215"));//portul cu care se conecteaza clientul
	
	//creeaza un socket ipv4, TCP
	sock_fct = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if(sock_fct == -1){
	
		perror("sock_fct");
		return EXIT_FAILURE;
	
	}//end if

	printf("Socketul merge \n");
	
	//leaga socketul la structura server (adresa+port)
	bind_fct = bind(sock_fct,(struct sockaddr *)&server, sizeof server);

	if(bind_fct==-1){
		perror("bind_fct");
		return EXIT_FAILURE;
	}
	printf("Bind merge \n");
	
	//socketul asculta cereri de conectare
	list = listen(sock_fct, 10);//maxim 10 clienti

	if(list == -1){
		perror("list");
		return EXIT_FAILURE;
	}
	
	printf("Listen...Waiting for connections\n");

	len = sizeof(dataRecv);
	len2 = sizeof(msg); 
	
	
	while(1){
		int varSize=sizeof cl; 
		char comanda[3]="05#";
	//se accepta conexiunile
	//in structura client se completeaza membrii care corespund socketului 
		acc = accept(sock_fct, (struct sockaddr *) &cl, &varSize );
		if(acc <0){
			perror("accept()");
			return EXIT_FAILURE;
		}//end if
		if((fork())==0){// daca se conecteaza un nou utilizator se mai aloca un proces
		       //pentru a servi clientul	

				while(1){
	//se receptioneaza datele de pe socket
	//se returneaza numarul de octeti primiti
					byte_count = recv(acc, dataRecv, len, 0);
	

					if(byte_count<0){
						perror("recv()");
						return EXIT_FAILURE;
					}//end if
					if(sizeof(dataRecv)==0){
						break;
					}
	//se compara comanda primita cu comanda #05
					if(strncmp(dataRecv,comanda,3)!=0){
	
		//se trimite mesajul comanda neimplementata 
						data_sent = send(acc, com,len3,0);
					}//end if
					else{	//functia connection returneaza file descriptorul socketului, un nr intreg
						int sock_fct = conection();//daca comanda e potrivita, apelam crearea socketului
						file=fopen("pagina.txt","w+");
						if(file==NULL){
							printf("mmm");
						}
						while(1){
							char *var = receive(sock_fct);//primim datele din clientul IPv6
	
							if(var == NULL){//verificam daca mai sunt date de primit
								break;
									}//end if
							fprintf(file,"%s",var);//scriem datele in fisier
							char arr[1024];
							strcpy(arr,var);//copiem din pointer in array de char
							int x=sizeof(arr);
							data_sent = send(acc, arr, x, 0);//trimitem datele catre client IPv4
							memset(var, 0, sizeof var);//golim bufferul
							memset(arr , 0, sizeof arr);
							if(data_sent== -1){
			 					perror("send()");
			  					return EXIT_FAILURE;}//end if
		
							}//end while
						fclose(file);

					}//end else
					memset(dataRecv, 0, sizeof dataRecv);
						

				}//while(1);//while
	close(acc);//inchidem socketul nou, astfel eliberam memoria
	}//if
}//while true
	return 0;

}
