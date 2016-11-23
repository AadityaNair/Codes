#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>
#include <openssl/sha.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
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

int initDownload(char *);
char* sendMsg(char *, int);


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

void cpy(char * a, char * b) {
	int i, j;
	i=j=0;
	while (b[i]!='\0') {
		a[i] = b[i];
		i++;
	}
	a[i] = '\0';
}


int main() {
	listenSocket = socket(AF_INET,SOCK_STREAM,0);

	KeyGen(&public, &private);
	strcpy(IP, "127.0.0.1");

	char * buf;
	buf = strdup("test.txt");
	printf("%s\n", buf);
	char filename[50];
	char chat_msg[50];

	char * returncode;

	bzero(filename, 50);
	filename[0] = 'k';
	filename[1] = '\0';

	sprintf(filename+1, "%lld:", public.public_key.e);
	sprintf(filename+strlen(filename), "%lld\0", public.public_key.n);
	printf("Key: %s", filename);
	sendMsg(filename, 0);

	bzero(filename, 50);
	cpy(filename+1, buf);
	filename[0] = 'd';

	initDownload(filename);
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

char * sendMsg(char * msg, int wait) {
	msg = replace(msg,' ', '&');
	char returncode[1024];
	int ClientSocket = 0;
	struct sockaddr_in c_serv_addr;

	ClientSocket = socket(AF_INET,SOCK_STREAM,0);
	c_serv_addr.sin_family = AF_INET;
	c_serv_addr.sin_port = htons(portno);
	c_serv_addr.sin_addr.s_addr = inet_addr(IP);
	printf("Trying...");
while(connect(ClientSocket,(struct sockaddr *)&c_serv_addr,sizeof(c_serv_addr))<0);
	bzero(buffer,1024);
	sscanf(msg, "%s", buffer);
	if(send(ClientSocket,buffer,strlen(buffer),0)<0)
		printf("ERROR while writing to the socket\n");
	bzero(buffer,1024);
if (wait == 1) {
	if(recv(ClientSocket,buffer,1024,0)<0)
		printf("ERROR while reading from the socket\n");
 	sscanf(buffer, "%s", returncode);
	close(ClientSocket);
	return returncode;
}
printf("Sent!");
}

int initDownload(char * filename) {
	char returncode[1024];
	int ClientSocket = 0;
	int ret;
	char * decoded;
	int counter, idx;
	char c;
	char hashed[SHA_DIGEST_LENGTH];
	int i;
	long long int encrypted, decrypted;
	long long int fileSize;
	char encoded[5];
	struct sockaddr_in c_serv_addr;

	ClientSocket = socket(AF_INET,SOCK_STREAM,0);
	if(ClientSocket<0)
	{
		printf("ERROR WHILE CREATING A SOCKET\n");
		return 0;
	}

	c_serv_addr.sin_family = AF_INET;
	c_serv_addr.sin_port = htons(portno);
	c_serv_addr.sin_addr.s_addr = inet_addr(IP);
	printf("Trying...\n");

while(connect(ClientSocket,(struct sockaddr *)&c_serv_addr,sizeof(c_serv_addr))<0);


	bzero(buffer,1024);
	sscanf(filename, "%s", buffer);
	if(send(ClientSocket,buffer,strlen(buffer),0)<0)
		printf("ERROR while writing to the socket\n");
	bzero(buffer,1024);
	strcat(filename, "_received");
	FILE *fp = fopen(filename+1, "wb");
	if(recv(ClientSocket,buffer,1024,0)<0)
		printf("ERROR while reading from the socket\n");
 	sscanf(buffer, "%s", returncode);
	if(returncode[0]=='x') {
		i=1;
		fileSize = 0;
		while(returncode[i]!='\0') {
        fileSize *= 10;
        fileSize += returncode[i++]-'0';
    	}
		bzero(buffer,1024);
		printf("Receiving file of size:%d\n", fileSize);
		ret = 1;
		for (ret = 0; ret<fileSize/2; ret++)
		{
			bzero(buffer,1024);
			recv(ClientSocket,buffer,1024,0);

			bzero(hashed, 20);

			idx=0;
			counter = 0;
			c = buffer[idx];
			encrypted = 0;
			while (c!=':') {
				encrypted = encrypted + (c - '0');
				encrypted = encrypted * 10;
				idx++;
				counter++;
				c = buffer[idx];
			}
			encrypted = encrypted/10;
			decrypted = Decryption(encrypted, private);

			if (decrypted < 10) {
				sprintf(encoded, "0%lld", decrypted);
			}
			else {
				sprintf(encoded, "%lld", decrypted);
			}
			decoded = decode_string(encoded);
			fwrite(decoded, sizeof(char),strlen(decoded), fp);
			printf("Received [ %d of %d ] ...\r", ret+1, fileSize/2);
		}
		printf("\n");
		bzero(buffer,1024);
		fclose(fp);
	}
	printf("File download complete.\n");
	printf("Closing Connection\n");
	close(ClientSocket);
	return 1;
}
