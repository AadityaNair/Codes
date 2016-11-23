#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <dirent.h>
#include <openssl/sha.h>
#include "encode.c"
#include "RSA.c"
int listenSocket = 0;
int connectionSocket = 0;
char buffer[1024];
struct sockaddr_in s_serv_addr;
int portno = 5005;
char IP[25];
char perm;
key_pair public, private;


void initServer();
char* replace(char *, char, char);

void get_hash(char * data, char * buf) {
	int i;
	unsigned char hash[SHA_DIGEST_LENGTH];
    memset(hash, 0x0, SHA_DIGEST_LENGTH);
    memset(buf, 0x0, SHA_DIGEST_LENGTH);

	SHA1((unsigned char *)data, strlen(data), hash);
	for (i=0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf((char*)&(buf[i*2]), "%02x", hash[i]);
    }
}

int main() {
	listenSocket = socket(AF_INET,SOCK_STREAM,0);
		strcpy(IP, "127.0.0.1");
	printf("Socket created\n");
	initServer();
	return 0;
}

char* replace(char* string, char replaceThis, char replaceWith)
{
	int l = strlen(string);
	int i;
	for (i = 0; i < l; ++i)
	{
		if(string[i] == replaceThis)
			string[i] = replaceWith;
	}
	return string;
}


void initServer() {
	char filename[50];
	int filesize;
	long long int plaint, ciphert;
	char encrypted[20];
	char encoded[20];
	char hashed[SHA_DIGEST_LENGTH];
	char fbuffer[1024];
	char * encoded_file;
	char e_recv[10];
	int idx, counter;
	char c;
	char n_recv[10];
	struct stat obj;
	perm = 'n';
	bzero((char *) &s_serv_addr,sizeof(s_serv_addr));
	

	s_serv_addr.sin_family = AF_INET;	//For a remote machine
	s_serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_serv_addr.sin_port = htons(portno);

	int yes=1;

if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
    perror("setsockopt");
}

	bind(listenSocket,(struct sockaddr * )&s_serv_addr,sizeof(s_serv_addr));

	if(listen(listenSocket,10) == -1){
		printf("[SERVER] FAILED TO ESTABLISH LISTENING \n\n");
	}
	printf("Connection created. Listening on port %d\n", portno);

while((connectionSocket=accept(listenSocket , (struct sockaddr*)NULL,NULL))<0);

	bzero(buffer,1024);
	if(recv(connectionSocket,buffer,1024,0)<0)
		printf("ERROR while reading from Client\n");

	strcpy(buffer, replace(buffer,'&',' '));
	printf("REQ: %s\n", buffer);

	switch(buffer[0]) {
		case 'k':

			bzero(e_recv, 10);
			bzero(n_recv, 10);

			idx=1;
			counter = 0;
			c = buffer[idx];	
			while (c!=':') {
				e_recv[counter] = c;
				idx++;
				counter++;
				c = buffer[idx];
			}
			idx++;
			c = buffer[idx];
			public.public_key.n = 0;	
			while (c!='\0') {
				public.public_key.n = public.public_key.n + (c - '0');
				public.public_key.n = public.public_key.n * 10;
				idx++;
				c = buffer[idx];
			}
			public.public_key.n = public.public_key.n / 10;
			// n_recv[counter] = '\0';
			public.public_key.e = atoi(e_recv);
			printf("Key received %lld %lld !\n", public.public_key.e, public.public_key.n);
			break;
		case 'd':
			bzero(hashed, SHA_DIGEST_LENGTH);

			sscanf(buffer+1, "%s", filename);
			
			FILE *fp = fopen(filename, "rb");

			bzero(buffer,1024);

			printf("File requested: %s\n", filename);

			encoded_file = readFile(fp);
			filesize = strlen(encoded_file);

			buffer[0]='x';
			sprintf(buffer+1, "%d", filesize);
			send(connectionSocket,buffer,1024,0);

			idx = 0;
			c = encoded_file[0];
			for (idx=0; idx<filesize; idx+=2) {
				c = encoded_file[idx];
				plaint = 10 * (c - '0');
				c = encoded_file[idx+1];
				plaint = plaint + (c - '0');
				ciphert = Encryption(plaint, public);
				// printf("%lld_", ciphert);
				sprintf(encrypted, "%lld", ciphert);
				sprintf(encoded, "%lld", plaint);
				get_hash(encoded, hashed);
				sprintf(buffer, "%s:%s", encrypted, hashed);
				send(connectionSocket,buffer,1024,0);
			}
			break;
}
	close(connectionSocket);
	initServer();
}
