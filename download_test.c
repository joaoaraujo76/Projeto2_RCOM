#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

#define SERVER_PORT 21
char* address;
char* g_name;
char* g_pass;
char* g_ftp;
char* g_path;

int getIp(char* name) {
    struct hostent *h;

/**
 * The struct hostent (host entry) with its terms documented

    struct hostent {
        char *h_name;    // Official name of the host.
        char **h_aliases;    // A NULL-terminated array of alternate names for the host.
        int h_addrtype;    // The type of address being returned; usually AF_INET.
        int h_length;    // The length of the address in bytes.
        char **h_addr_list;    // A zero-terminated array of network addresses for the host.
        // Host addresses are in Network Byte Order.
    };

    #define h_addr h_addr_list[0]	The first address in h_addr_list.
*/
    if ((h = gethostbyname(name)) == NULL) {
        herror("gethostbyname()");
        return 1;
    }

    printf("Host name  : %s\n", h->h_name);
    printf("IP Address : %s\n", inet_ntoa(*((struct in_addr *) h->h_addr)));

    address = inet_ntoa(*((struct in_addr *) h->h_addr));

    return 0;
}
			

int main(int argc, char **argv) {

    
    char* url = argv[1];
    printf("url: %s\n", url);
    
    //NAME START-----------------------------------------------------------
    
    char idk1[100];
    int j = 7;
    int index = 0;
	
    while (1) {
	if (url[j] == ':') {
		break;
	}
	idk1[index] = url[j];
	
	j++;
	index++;
    }
    idk1[index+1] = '\0';
	
    //NAME END------------------------------------------------------
    
    //PASS START----------------------------------------------------
    char idk2[100]; 
    j = 7;
    index = 0;
	
    while (1) {
    	if (url[j] == ':') {
		j++;
		break;
	}
	j++;
    }
	
    while (1) {
	if (url[j] == '@') {
		break;
	}
	idk2[index] = url[j];
	j++;
	index++;
    }
	
    
    //PASS END----------------------------------------------------
    
    //FTP START----------------------------------------------------
    char idk3[100]; 
    j = 7;
    index = 0;
	
    while (1) {
    	if (url[j] == ']') {
		j++;
		break;
	}
	j++;
    }
	
    while (1) {
	if (url[j] == '/') {
		break;
	}
	idk3[index] = url[j];
	j++;
	index++;
    }
    
    //FTP END----------------------------------------------------
    
    //PATH START----------------------------------------------------
    
    char idk4[100]; 
    j = 7;
    index = 0;
	
    while (1) {
    	if (url[j] == '/') {
		break;
	}
	j++;
    }
	
    while (1) {
	if (j >= 100) {
		break;
	}
	idk4[index] = url[j];
	j++;
	index++;
    }
    
    //PATH END----------------------------------------------------
    
    char message1[100] = "user ";
    char message2[100] = "pass ";
    char message3[100] = "pasv\n";
    char message4[100] = "retr ";
    char enter[] = "\n";
    
   
    strcat(message1, idk1);
    strcat(message1, enter);
    
    printf("name: %s\n", idk1);
    

    strcat(message2, idk2);
    strcat(message2, enter);
    
    printf("pass: %s\n", idk2);
    
    strcat(message4, idk4);
    strcat(message4, enter);
   
    printf("path: %s\n", idk4);
    
    
    if (getIp(idk3) == 1) {
        printf("Error in getIP\n");
        return 1;
    }
    
    printf("ftp: %s\n", idk3);
    
    
   

    
    size_t bytes;
    
    
    

    /*server address handling*/
    int sockfd;
    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(address);    /*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(SERVER_PORT);        /*server TCP port must be network byte ordered */

    /*open a TCP socket*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(-1);
    }
    
    char conf[4000];
    char line[100];
    
    /*connect to the server*/
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("connect()");
        exit(-1);
    }
    
    sleep(1);
    
    bytes = read(sockfd, conf, 1000);
    printf("%s\n", conf);
    
    
    /*User Message*/
    printf("SENDING: %s", message1);
    
    bytes = write(sockfd, message1, strlen(message1));
    if (bytes > 0)
        printf("Bytes escritos %ld\n", bytes);
    else {
        perror("write()");
        exit(-1);
    }
    
    sleep(1);
    
    bytes = read(sockfd, conf, 1000);
    
    strcpy(line, "");
    
    for (int i = 0; i < 1000; i++) {
    	line[i] = conf[i];
    	if (line[i] == '\n') {
    		break;
    	}
    }
    
    printf("%s\n", line);
    
    /*Pass Message*/
    printf("SENDING: %s", message2);
    
    bytes = write(sockfd, message2, strlen(message2));
    if (bytes > 0)
        printf("Bytes escritos %ld\n", bytes);
    else {
        perror("write()");
        exit(-1);
    }
    
    sleep(1);
    
    bytes = read(sockfd, conf, 1000);
    
    strcpy(line, "");
    
    for (int i = 0; i < 1000; i++) {
    	line[i] = conf[i];
    	if (line[i] == '\n') {
    		break;
    	}
    }
    
    
    printf("%s\n", line);

    /*Passive Mode Message*/
    printf("SENDING: %s", message3);
    
    bytes = write(sockfd, message3, strlen(message3));
    if (bytes > 0)
        printf("Bytes escritos %ld\n", bytes);
    else {
        perror("write()");
        exit(-1);
    }
    
    sleep(1);
   
    bytes = read(sockfd, conf, 1000);
    
    strcpy(line, "");
    
    for (int i = 0; i < 1000; i++) {
    	line[i] = conf[i];
    	if (line[i] == '\n') {
    		break;
    	}
    }

    printf("%s\n", line);
    
    char delim[] = ",)";
    char *ptr = strtok(line, delim);
    
    int num1;
    int num2;
    int i = 0;

    while(ptr != NULL)
    {

	if (i == 4) {
	    num1 = atoi(ptr);
	}
	else if (i == 5) {
	    num2 = atoi(ptr);
	}
	
	ptr = strtok(NULL, delim);
	i++;
    }
    
    printf("num1 = %d ||| num2 = %d\n",  num1, num2);
    
    int new_port = num1 * 256 + num2;
    
    int sock2fd;
    struct sockaddr_in server_addr2;
    bzero((char *) &server_addr2, sizeof(server_addr2));
    server_addr2.sin_family = AF_INET;
    server_addr2.sin_addr.s_addr = inet_addr(address);    /*32 bit Internet address network byte ordered*/
    server_addr2.sin_port = htons(new_port);        /*server TCP port must be network byte ordered */

    /*open a TCP socket*/
    if ((sock2fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(-1);
    }
    
    
    /*connect to the server*/
    if (connect(sock2fd, (struct sockaddr *) &server_addr2, sizeof(server_addr2)) < 0) {
        perror("connect()");
        exit(-1);
    }
    
    
    /*Passive Mode Message*/
    printf("SENDING: %s", message4);
    
    bytes = write(sockfd, message4, strlen(message4));
    if (bytes > 0)
        printf("Bytes escritos %ld\n", bytes);
    else {
        perror("write()");
        exit(-1);
    }
    
    sleep(1);
   
    bytes = read(sockfd, conf, 1000);

    printf("%s\n", conf);
    
    char num_bytes[100];
    
    int kik = 0;
    while (kik < 1000) {
    	if (conf[kik] == '(') {
    		kik++;
    		break;
    	}
    	kik++;
    }
    
    int index_final = 0;
    
    while (kik < 1000) {
    	if (conf[kik] == ' ') {
    		break;
    	}
    
    	num_bytes[index_final] = conf[kik];
    	kik++;
    	index_final++;
    }
    
    
    int final_number = atoi(num_bytes);
    
    printf("byte number = %d\n", final_number);
    	
    
    char res[final_number * 10];
    
    bytes = read(sock2fd, res, final_number * 10);
    
    
    
    


    printf("FILE READ:\n");
    printf("%s\n", res);
    
    
    
    
    
    
    



    if (close(sockfd)<0) {
        perror("close()");
        exit(-1);
    }
    if (close(sock2fd)<0) {
        perror("close()");
        exit(-1);
    }
    
    
    FILE *f = fopen("ftp_file", "wb");
    fwrite(res, 1, final_number, f);
    fclose(f);
    
    
    
    
    return 0;
}




