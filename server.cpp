#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <ctype.h>
#include <fstream>
#include <stddef.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>

using namespace std;
typedef struct structure{
	char name[2000];
	char type[2000];
	char time[2000];
	int size;
	char md5[2000];
}point;

void scans(int sockfd);
point file[1000];point serv_file[1000];
int commctr=0;
char root[900];char comm[300][300];
char serv_root[900];char serv_comm[300][300];
int serv_commctr=0;
char *concat(char c1[],char c2[]){
	char *ans = (char *)malloc(sizeof(char)*100);
	for(int i=0;i<strlen(c1);i++)ans[i]=c1[i];
	ans[strlen(c1)]=' ';
	for(int i=strlen(c1)+1;i<strlen(c1)+strlen(c2)+1;i++)ans[i]=c2[i-strlen(c1)-1];
	ans[strlen(c1)+strlen(c2)+1]='\0';
	return ans;
}

int receive(int sockfd,struct sockaddr_in serv_addr,socklen_t client_len)
{
	int recvdatalen,i=0,recv_byte=0;
	char received_data[2000],send_data[2000];
	recv(sockfd,&recvdatalen,sizeof(recvdatalen),0);
	int no_files=recvdatalen;
	for(i=0;i<no_files;i++)
	{
		//Filename
		recv_byte=recv(sockfd,received_data,2000,0);
		received_data[recv_byte]='\0';
		strcpy(file[i].name,received_data);
		//Last Modified
		recv_byte=recv(sockfd,received_data,2000,0);
		received_data[recv_byte]='\0';
		strcpy(file[i].time,received_data);
		//Size of File
		recv(sockfd,&recvdatalen,sizeof(recvdatalen),0);
		file[i].size=recvdatalen;
		//Type of File
		recv_byte=recv(sockfd,received_data,2000,0);
		received_data[recv_byte]='\0';
		strcpy(file[i].type,received_data);

	}
	return no_files;
}
void filehash(int sockfd,struct sockaddr_in serv_addr,socklen_t client_len)
{
	int recvdatalen,i=0,recv_byte=0;
	char received_data[2000],send_data[2000];
	recv(sockfd,&recvdatalen,sizeof(recvdatalen),0);
	int no_files=recvdatalen;
	for(i=0;i<no_files;i++)
	{
		//Filename
		recv_byte=recv(sockfd,received_data,2000,0);
		received_data[recv_byte]='\0';
		strcpy(file[i].name,received_data);
		//Last Modified
		recv_byte=recv(sockfd,received_data,2000,0);
		received_data[recv_byte]='\0';
		strcpy(file[i].time,received_data);
		//Size of File
		recv(sockfd,&recvdatalen,sizeof(recvdatalen),0);
		file[i].size=recvdatalen;
		//Type of File
		recv_byte=recv(sockfd,received_data,2000,0);
		received_data[recv_byte]='\0';
		strcpy(file[i].type,received_data);
		//MD5 Hash
		recv(sockfd,file[i].md5,33,0);
	}
	if(!strcmp(comm[1],"Verify"))
	{
		for(i=0;i<no_files;i++)
		{
			if(!strcmp(comm[2],file[i].name))
			{
				printf("\n***\n");
				printf("FileName: %s\nFileSize: %d\nFiletype: %s\nTime: %s\nMD5: %s\n",file[i].name,file[i].size,file[i].type,file[i].time,file[i].md5);
				break;
			}
		}
		if(i==no_files)
			printf("No Such File on Remote Host\n");
		else printf("End\n");
	}
	else if(!strcmp(comm[1],"checkAll"))
	{
		for(i=0;i<no_files;i++)
		{					
			printf("\n***\n");
			printf("FileName: %s\nFileSize: %d\nFiletype: %s\nTime: %s\nMD5: %s\n",file[i].name,file[i].size,file[i].type,file[i].time,file[i].md5);
		}
		printf("End\n");
	}
	else
		printf("Missing Argument\n");
}
char stamp[299];
void shortlist(int no_files)
{
	int i;
	char stamp1[100],stamp2[100];char arr[200];char mon[50];
	if(strlen(comm[2])>0&&strlen(comm[3])>0&&strlen(comm[4])>0&&strlen(comm[5])>0)
	{
		strcpy(stamp1,concat(comm[2],comm[3]));
		strcpy(stamp2,concat(comm[4],comm[5]));
		for(i=0;i<no_files;i++)
		{
			for(i=0;i<3;i++)
				mon[i]=stamp1[i+4];
			mon[3]='\0';
			arr[4]='-';
			if(strcmp(mon,"Jan")==0)
			{
				arr[5]='0';
				arr[6]='1';
			}
			else if(strcmp(mon,"Feb")==0)
			{
				arr[5]='0';
				arr[6]='2';
			}
			else if(strcmp(mon,"Mar")==0)
			{
				arr[5]='0';
				arr[6]='3';
			}
			else if(strcmp(mon,"Apr")==0)
			{
				arr[5]='0';
				arr[6]='4';
			}
			else if(strcmp(mon,"May")==0)
			{
				arr[5]='0';
				arr[6]='5';
			}
			else if(strcmp(mon,"Jun")==0)
			{
				arr[5]='0';
				arr[6]='6';
			}
			else if(strcmp(mon,"Jul")==0)
			{
				arr[5]='0';
				arr[6]='7';
			}
			else if(strcmp(mon,"Aug")==0)
			{
				arr[5]='0';
				arr[6]='8';
			}
			else if(strcmp(mon,"Sep")==0)
			{
				arr[5]='0';
				arr[6]='9';
			}
			else if(strcmp(mon,"Oct")==0)
			{
				arr[5]='1';
				arr[6]='0';
			}
			else if(strcmp(mon,"Nov")==0)
			{
				arr[5]='1';
				arr[6]='1';
			}
			else if(strcmp(mon,"Dec")==0)
			{
				arr[5]='1';
				arr[6]='2';
			}
			if(stamp1[8]==' ')
			{
				arr[8]='0';
				arr[9]=stamp1[9];
			}
			else
			{
				arr[8]=stamp1[8];
				arr[9]=stamp1[9];
			}
			arr[7]='-';
			arr[10]=' ';

			for(i=0;i<4;i++)
				arr[i]=stamp1[i+20];

			for(i=0;i<8;i++)
				arr[i+11]=stamp1[i+11];
			arr[19]='\0';
			for(i=0;i<20;i++)
				stamp[i]=arr[i];
			if(strcmp(stamp,stamp1)>=0&&strcmp(stamp,stamp2)<=0)
			{
				printf("\n***\n");
				printf("FileName: %s\nFileSize: %d\nFiletype: %s\nTime: %s\n",file[i].name,file[i].size,file[i].type,file[i].time);
			}
		}
		printf("End\n");
	}
	else
		printf("Error\n");
}
void longlist(int no_files)
{
	int i=0;
	for(i=0;i<no_files;i++)
	{
		printf("\n***\n");
		printf("FileName: %s\nFileSize: %d\nFiletype: %s\nTime: %s\n",file[i].name,file[i].size,file[i].type,file[i].time);
	}
	printf("End\n");
}
void regex(int sockfd,struct sockaddr_in serv_addr,socklen_t client_len,int no_files)
{
	int recvdatalen,i=0,recv_byte=0;
	char received_data[2000],send_data[2000];
	if(strlen(comm[2])>0)
	{
		recvdatalen=recv(sockfd,received_data,2000,0);
		received_data[recvdatalen]='\0';
		while(strcmp(received_data,"End of File"))
		{
			for(i=0;i<no_files;i++)
			{
				if(!strcmp(received_data,file[i].name))
				{
					printf("\n***\n");
					printf("FileName: %s\nFileSize: %d\nFiletype: %s\nTime: %s\n",file[i].name,file[i].size,file[i].type,file[i].time);
				}
			}
			recvdatalen=recv(sockfd,received_data,2000,0);
			received_data[recvdatalen]='\0';
		}
		printf("End\n");
	}
	else
		printf("Argument for regEx is Missing \n");
}

void sends(int connfd,struct sockaddr_in serv_addr,int listenfd,int no_files)
{
	int i;
	send(listenfd,&no_files,sizeof(int),0);
	for(i=0;i<no_files;i++)
	{
		for(i=0;i<no_files;i++)
		{
			//Filename
			send(listenfd,serv_file[i].name,2000,0);
			//Last Modified
			send(listenfd,serv_file[i].time,2000,0);
			//Size of File
			send(listenfd,&serv_file[i].size,sizeof(int),0);
			//File Type
			send(listenfd,serv_file[i].type,2000,0);
		}
	}

}

void serv_regex(int connfd,struct sockaddr_in serv_addr,int listenfd,int no_files)
{
	char systemcomm[200];char send_data[2000];
	printf("\nAnkit\n");
	strcpy (systemcomm,concat("ls ",serv_comm[2]));
	system(concat(systemcomm,">out"));
	FILE * out= fopen("out","r");
	while(fscanf(out,"%s",&send_data)!=EOF)
	{
		send(listenfd,send_data,2000,0);
	}
	send(listenfd,"End of File\0",2000,0);

}
void serv(int portlin,int portsen,string proto)
{	
	char sendBuff[1025];char received_data[2000],send_data[2000];
	char recv_data[2000];
	int recvdatalen;int ini[700];
	int listenfd = 0,connfd = 0;
	struct sockaddr_in serv_addr,client_addr;
	int sin_size = sizeof(struct sockaddr_in);
	socklen_t * lol = (socklen_t *) &sin_size;
	if (proto=="tcp")
	{
		listenfd = socket(AF_INET, SOCK_STREAM, 0);
		if(listenfd<0)
		{
			perror("Socket Creation Failed");
			exit(0);
		}
	}
	else
	{
		listenfd = socket(AF_INET, SOCK_DGRAM, 0);
		if(listenfd<0)
		{
			perror("Socket Creation Failed");
			exit(0);
		}
	}
	printf("socket retrieve success\n");
	//memset(&serv_addr, '0', sizeof(serv_addr));
	//memset(sendBuff, '0', sizeof(sendBuff));
	int merge=sizeof(struct sockaddr_in);
	serv_addr.sin_family = AF_INET;    
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	serv_addr.sin_port = htons(portlin);int i;int no_files=0;
	bzero(&(serv_addr.sin_zero),8);

	if(bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
	{
		perror("Unable to Bind");
		exit(0);
	}
	//client_len = sizeof(struct sockaddr_in);
	if(listen(listenfd, 10) == -1)
	{
		printf("Failed to listen\n");
		exit(0);
	}
	printf("Server Listening on Port: %d\n",portlin);
	while(1)
	{
		printf("\n************************\n");

		connfd = accept(listenfd, (struct sockaddr *)&client_addr,lol); // accept awaiting request
		printf("Connected To %s,%d", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		//strcpy(sendBuff, "Goodnight Ankit");
		//write(connfd, sendBuff, strlen(sendBuff));
		//printf("\n77777777777\n");

		while(1)
		{
			recvdatalen=recv(connfd,received_data,2000,0);
			received_data[recvdatalen]='\0';
			printf("\nlen %d\n",recvdatalen);
			char fullstring[90]; 
			serv_commctr=0;
			for(i=0;i<strlen(received_data);i++)
			{
				serv_root[serv_commctr]=received_data[i];
				serv_commctr++;
			}
			serv_root[serv_commctr]='\0';
			serv_commctr++;
			//printf("r->%s",received_data);
			int lens=0;serv_commctr=0;
			for(i=0;i<strlen(serv_root);i++)
			{
				if(serv_root[i]==' ')
				{
					serv_comm[serv_commctr][lens]='\0';
					serv_commctr++;
					lens=0;
				}
				else
				{
					serv_comm[serv_commctr][lens]=serv_root[i];
					lens++;
				}
			}
			serv_comm[serv_commctr][lens]='\0';
			serv_commctr++;
			lens++;
			if(recvdatalen==0)
			{
				printf("Connection Ended\n");
				close(connfd);
				break;
			}
			if(!strcmp(received_data,"exit"))
			{
				printf("Connection Closed\n");
				close(connfd);
				break;
			}
			else
			{
				printf("Command Given: %s\n",received_data);
				sleep(1);
				//	printf("%sbb\n%sbb\n",serv_comm[0],serv_comm[1]);
				//	printf("\n%d\n",strcmp(serv_comm[0],"IndexGet"));
				if(strcmp(serv_comm[0],"IndexGet")==0)
				{
					//		printf("\nInside\n");
					DIR *direct;
					struct dirent *point;char syscomm[1000];
					for(i=0;i<strlen(received_data);i++)
					{
						if(!strcmp(received_data,"ShortList"))
							recvdatalen++;
					}
					direct=opendir("./");
					no_files=0;
					int fsize;
					string line;
					if (direct!=NULL)
					{
						while(point=readdir(direct))
						{
							struct stat st;
							strcpy(serv_file[no_files].name,point->d_name);
							stat(point->d_name,&st);
							fsize = st.st_size;
							strcpy(syscomm,concat("file ",serv_file[no_files].name));
							system(concat(syscomm,"> Type"));
							ifstream in;
							in.open("Type");
							getline(in,line);
							in.close();
							strcpy(serv_file[no_files].type,line.c_str());
							serv_file[no_files].size=fsize;
							strcpy(serv_file[no_files].time,ctime(&st.st_mtime));
							//checksum(serv_file[no_files].name,serv_file[no_files].md5);
							no_files++;
						}
						closedir(direct);
					}
					else
						printf("Error in Reading Files");
					//	printf("\nserv_file[0]=%d\n",no_files);
					sends(listenfd,serv_addr,connfd,no_files);
					if(serv_commctr>1)
					{
						printf("\nServ_regex\n");
						serv_regex(listenfd,serv_addr,connfd,no_files);
					}
				}
				if(strcmp(serv_comm[0],"Download")==0)
				{
					int inthead=0;
					ifstream ifile(serv_comm[1]);
					if(ifile){
						send(connfd,"file exists",2000,0);
						char ch;
						FILE *fp;
						fp = fopen(serv_comm[1],"r");
						int count;
						while(fscanf(fp,"%c",&ch)!=EOF){
							count=0;
							send_data[count++]=ch;
							while(count<2000 && fscanf(fp,"%c",&ch)!=EOF)
							{
								send_data[count++]=ch;
							}
							send(connfd,&count,sizeof(int),0);
							send(connfd,send_data,2000,0);
						}
						send(connfd,&inthead,sizeof(int),0);
						send(connfd,"End of File",2000,0);
					}
					else
					{
						send(connfd,"file doesn't exist",2000,0);
					}
				}
				if(strcmp(serv_comm[0],"Upload")==0)
				{
					{
						recv(connfd,&ini[0],sizeof(int),0);
						recvdatalen=recv(connfd,recv_data,2000,0);
						recv_data[recvdatalen] = '\0';
						FILE *fp;
						fp = fopen(serv_comm[1],"w");
						while(strcmp(recv_data,"End of File")!=0)
						{
							for(int i=0;i<ini[0];i++)
								fprintf(fp,"%c",recv_data[i]);
							recv(connfd,&ini[1],sizeof(ini[1]),0);
							recvdatalen=recv(connfd,recv_data,2000,0);
							recv_data[recvdatalen] = '\0';
						}
						fclose(fp);
						printf("File upload completed                            [OK]\n");
					}
				}
				if(strcmp(serv_comm[0],"FileHash")==0)
				{

				}
			}no_files=0;

		}
		fflush(stdout);
	}
	close(connfd);    
	sleep(1);
	return;
}
int cli(int portlin,int portsen,string proto)
{

	int sockfd = 0,n = 0;
	char recvBuff[2000];
	int listenfd = 0,connfd = 0;
	struct hostent *host;
	struct sockaddr_in serv_addr,client_addr;
	if(proto=="tcp")
	{
		if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
		{
			printf("\n Error : Could not create socket \n");
			return 1;
		}
	}
	else
	{
		if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))< 0)
		{
			printf("\n Error : Could not create socket \n");
			return 1;
		}
	}
	host = gethostbyname("127.0.0.1");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portsen);
	serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(serv_addr.sin_zero),8);
	//serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	socklen_t client_len=sizeof(client_addr);
	client_len = sizeof(struct sockaddr_in);
	while(connect(sockfd,(struct sockaddr *)&serv_addr, sizeof(struct sockaddr))==-1);
	//{
	printf("\n Connect Estblished\n");
	//		return 2;
	//}
	printf("Client Connected to port: %d\n",portsen);
	scans(sockfd);
	//printf("scanned\n");
	int no_files=0;
	while(strcmp(comm[0],"exit")!=0)
		//	n = read(sockfd, recvBuff, sizeof(recvBuff)-1) > 0;
	{
		if(strcmp(comm[0],"IndexGet")==0)
		{
			if(proto=="tcp")
			{
				send(sockfd,root,strlen(root),0);
			}
			no_files=receive(sockfd,serv_addr,client_len);
			printf("\nNo  of files%d\n",no_files);
			if(!strcmp(comm[1],"ShortList"))
			{
				shortlist(no_files);
			}
			else if(strcmp(comm[1],"LongList")==0)
			{
				printf("\nlonglist\n");
				longlist(no_files);
				//		recvBuff[n] = 0;
				//		if(fputs(recvBuff, stdout) == EOF)
				//		{
				//			printf("\n Error : Fputs error");
				//		}
				//		printf("\n");
			}
			else if(strcmp(comm[1],"RegEx")==0)
				regex(sockfd,serv_addr,client_len,no_files);
		}
		if(!strcmp(comm[0],"FileHash"))
		{

			send(sockfd,root,strlen(root),0);
			filehash(sockfd,serv_addr,client_len);
		}
		
		else if(strcmp(comm[0],"Upload")==0)
		{
			int inndata=0;char array[5000];int recvdatalen;
			if(commctr<2)
				printf("Insufficient Arguments");
			else 
			{
				
				{
					ifstream ifile(comm[1]);
					int lala=25000;
					if(ifile)
					{
						send(sockfd,root,2000,0);
						{
							char ch;
							FILE *fp;
							char send_data[2000];
							fp = fopen(comm[1],"r");
							int count;
							while(fscanf(fp,"%c",&ch)!=EOF)
							{
								count=0;
								send_data[count++]=ch;
								while(count<2000 && fscanf(fp,"%c",&ch)!=EOF){
									send_data[count++]=ch;
								}
								send(sockfd,&count,sizeof(int),0);
								send(sockfd,send_data,2000,0);
							}
							send(sockfd,&lala,sizeof(int),0);
							send(sockfd,"End of File",2000,0);
						}
					}
					else
						printf("No such file or directory");
				}
			}
		}
		if(strcmp(comm[0],"Download")==0)
		{	char recv_data[2000];int recvdatalen;
			if(commctr < 2){
				printf("arguments missing\n");
				scans(sockfd);
				continue;
			}

			else{	

				send(sockfd,root,strlen(root),0);

				recvdatalen=recv(sockfd,recv_data,2000,0);

				recv_data[recvdatalen]='\0';

				if(strcmp(recv_data,"file doesn't exist")!=0){

					recv(sockfd,&recvdatalen,sizeof(recvdatalen),0);
					int intdata=recv(sockfd,recv_data,2000,0);

					FILE *fp;
					fp = fopen(comm[1],"w");
					while(strcmp(recv_data,"End of File")!=0){
						printf(".");
						for(int i=0;i<recvdatalen;i++)
							fprintf(fp,"%c",recv_data[i]);
						recv(sockfd,&recvdatalen,sizeof(int),0);
						intdata = recv(sockfd,recv_data,2000,0);
						recv_data[intdata]='\0';
					}
					fclose(fp);
					printf("File Download completed                            [OK]\n");
				}
				else
					printf("No such file or directory found on the remote host              [Fail]\n");
			}
		}
		scans(sockfd);
		//	if( n < 0)
		///	{
		//		printf("\n Read Error \n");
		//	}


	}
	return 0;
}
void serv_scans(char received_data[2000])
{
	int i=0;
}
void scans(int sockfd)
{
	/*	int i=0;char c;int ctr=0;
		printf("Prompt: ");
		scanf("%c",&c);
		scanf("%c",&c);
		while(c!='\n')
		{
		root[ctr]=c;
		ctr++;
		scanf("%c",&c);
		}
		root[ctr++]='\0';
		commctr=0;int lens=0;
		for(i=0;i<strlen(root);i++)
		{
		if(root[i]==' ')
		{
		comm[commctr][lens++]='\0';
		commctr++;
		lens=0;
		}
		else
		{
		comm[commctr][lens]=root[i];
		lens++;
		}
		}
		comm[commctr][lens++]='\0';
		commctr++;*/

	int i=0;
	char fullstring[90]; 
	printf("Prompt: ");
	gets(root);
	//while(strcmp(root,"")==0)
	//{
	//gets(root);

	//}
	commctr=0;int lens=0;
	for(i=0;i<strlen(root);i++)
	{
		if(root[i]==' ')
		{
			comm[commctr][lens]='\0';
			commctr++;
			lens=0;
		}
		else
		{
			comm[commctr][lens]=root[i];
			lens++;
		}
	}
	comm[commctr][lens++]='\0';
	commctr++;
	//printf("comm %s\n",comm[0]);
	//printf("root %s\n",comm[0]);

}
int main(void)
{
	string proto;
	int portlin=0,portsen;
	char sendBuff[1025];  
	int numrv;
	pid_t pid;
	cout << "Please enter protocol(tcp/udp): ";
	cin >> proto; 
	cout << "Please enter port no. to be used for listening(>1024): ";
	cin >> portlin;
	cout << "Please enter port no. to be used for sending(>1024): ";
	cin >> portsen;
	pid=fork();
	if (pid==-1)
	{
		cout << "Create Process Error";
		exit(0);
	}
	if(pid==0)
		serv(portlin,portsen,proto);
	else
	{
		cli(portlin,portsen,proto);
		//	sleep(2);
	}
	kill(pid,SIGQUIT);
	return 0;
}


