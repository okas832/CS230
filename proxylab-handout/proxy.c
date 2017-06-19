
#include "csapp.h"
/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *ver_hdr = "HTTP/1.0";
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *host_hdr = "Host: ";
static const char *conn_hdr = "Connection: close\r\n";
static const char *proxy_conn_hdr = "Proxy-Connection: close\r\n";

void * process(void * arg);
int urlp(char *url, char *host,char *path, int *port);

int main(int argc, char **argv){
	int listenfd;
	socklen_t clientlen;
	struct sockaddr_storage clientaddr;
	char client_hostname[MAXBUF], client_port[MAXBUF];
	pthread_t tid;	

	signal(SIGPIPE, SIG_IGN); //if connection broke, ignore

	if(argc == 1) listenfd = Open_listenfd("18796"); //my default proxy port
	else listenfd = Open_listenfd(argv[1]);
	
	while(1){
		clientlen = sizeof(struct sockaddr_storage);
		int *connfd = malloc(sizeof(int)); //store each connection fd
										   //because there will be a race condition in thread
		*connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		Getnameinfo((SA*) &clientaddr, clientlen,
                    client_hostname  , MAXLINE, 
                    client_port      , MAXLINE,
                    0);
		printf("Connected to %s:%s\n",client_hostname,client_port);
		pthread_create(&tid, NULL, process, (void *)connfd);
	}
	Close(listenfd);
	return 0;
}

void * process(void * arg){
	int connfd = *(int *)arg;
	char buf[MAX_OBJECT_SIZE] = {0,};
	char method[MAXBUF] = {0,}, url[MAXBUF] = {0,}, ver[MAXBUF] = {0,};
	char host[MAXBUF] = {0,}, path[MAXBUF] = {0,};
	char req[MAXBUF];
	int port;
	char portc[10];
	int clientfd;
	int n;
	rio_t rio_c,rio_s;
	
	//get request	
	Rio_readinitb(&rio_c, connfd);
	Rio_readlineb(&rio_c, buf, MAX_OBJECT_SIZE - 1);
	
	printf("Request : %s", buf);
	
	//seperate the first line of the request	
	if(sscanf(buf, "%s %s %s",method,url,ver) != 3){
		printf("Wrong Request\n");
		free(arg);
		Close(connfd);
		return NULL;
	}

	//parse url to host, path, port
	if(urlp(url,host,path,&port) == -1){
		printf("Wrong url(maybe HTTPS)\n");
		free(arg);
		Close(connfd);
		return NULL;
	}

	//connect to the target server for request
	sprintf(portc,"%d",port);
	clientfd = Open_clientfd(host,portc);
	if(clientfd < 0){
		printf("Cannot connect to server");
		free(arg);
		Close(connfd);
		return NULL;
	}
	//construct request
	sprintf(req,"%s %s %s\r\n%s%s\r\n%s%s%s\r\n",method
                                                ,path
                                                ,ver_hdr
                                                ,host_hdr
                                                ,host
                                                ,user_agent_hdr
                                                ,conn_hdr
                                                ,proxy_conn_hdr);
	printf("%s",req);
	//send it to server
	write(clientfd,req,strlen(req));
	
 	//get responce
	Rio_readinitb(&rio_s,clientfd);
	while((n = Rio_readnb(&rio_s, buf,MAX_OBJECT_SIZE - 1))){ // read rawdata until end
		//send to the proxy client
		write(connfd,buf,n);
	}
	
	free(arg);
	Close(clientfd);
	Close(connfd);
	return NULL;
}

int urlp(char *url, char *host, char *path, int *port){
	char * host_s;
	char * host_f;
	char * path_s;
	int len;
	if(strncmp(url,"http://",7) != 0) return -1;

	//get host name
	host_s = url + 7;
	host_f = strpbrk(host_s," :/");
	len = host_f - host_s;
	strncpy(host,host_s,len);
	host[len] = '\0';

	//get port nunber
	if(*host_f == ':') *port = atoi(host_f + 1);
	else *port = 80;

	//get path
	path_s = strpbrk(host_f,"/");
	if(path_s == NULL) strcpy(path,"/");	
	else strcpy(path,path_s);
	return 0;	
}
