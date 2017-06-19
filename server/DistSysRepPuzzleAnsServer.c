#include <string.h>
#include <ctype.h>
#include <rpc/rpc.h>
#include "strcap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_LEN 512

typedef struct {
  char schema[10];
  char domain[255];
  char path[255];
  int port;
} URL;

URL *parseURL(char *url)
{
  URL *obj;
  char *p;
  char *p_domain;
  char *p_path;
  char *p_end;

  obj = (URL *)malloc(sizeof(URL));
  memset(obj, '\0', sizeof(URL));
  p_end = url + strlen(url);
  p = strstr(url, "://");
  if (p) {
    strncpy(obj->schema, url, p-url);
    p_domain = url+strlen(obj->schema)+3;
    if (p_path = strchr(p_domain, '/')) {
      strncpy(obj->domain, p_domain, p_path-p_domain);
      strncpy(obj->path, p_path, p_end-p_path);
    } else {
      strncpy(obj->domain, p_domain, p_end-p_domain);
      strcpy(obj->path, "/");
    }
    if (p = strchr(obj->domain, ':')) {
      obj->port = atoi(p+1);
      *p = '\0';
    }
    if (0<strlen(obj->domain)) {
      return obj;
    }
  }
  return NULL;
}

char* get_puzz_route(char *str){
  static char ret[BUF_LEN];
  static char ret2[BUF_LEN];
  char dummy[BUF_LEN];
  char getstr[BUF_LEN];
  memset(ret, '\0', BUF_LEN);
  memset(ret2, '\0', BUF_LEN);
  memset(dummy, '\0', BUF_LEN);
  memset(getstr, '\0', BUF_LEN);
  int read_size;
  int s;
  struct hostent *servhost;
  struct in_addr addr;
  struct sockaddr_in server;
  char send_buf[BUF_LEN];
  int i;
  URL *url;

  url = parseURL(str);
  if (url == NULL) {
    printf("Parse error: %s\n", str);
    return NULL;
  }
  servhost = gethostbyname(url->domain);
  if (servhost == NULL) {
    printf("IP変換失敗\n");
    return NULL;
  }

  printf("servhost->h_name: %s\n", servhost->h_name);
  for (i=0; servhost->h_aliases[i]; i++) {
    printf("servhost->h_aliases[%u]: %s\n", i, servhost->h_aliases[i]);
  }

  printf("servhost->h_addrtype: %u\n", servhost->h_addrtype);
  printf("servhost->h_length: %u\n", servhost->h_length);
  for (i=0; servhost->h_addr_list[i]; i++) {
    memcpy(&addr, servhost->h_addr_list[i], 4);
    printf("servhost->h_addr_list[%u]: %s\n", i, inet_ntoa(addr));
  }
  printf("\n");

  memset(&server, '\0', sizeof(server));
  server.sin_family = AF_INET;
  memcpy(&server.sin_addr, servhost->h_addr, servhost->h_length);
  server.sin_port = htons(80);

  // ソケット作成
  if ( (s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("ソケット作成失敗\n");
    return NULL;
  }

  // 接続
  if (connect(s, (struct sockaddr *)&server, sizeof(server)) == -1) {
    printf("接続失敗\n");
    return NULL;
  }

  // write
  sprintf(send_buf, "GET %s HTTP/1.1\r\n", url->path);
  write(s, send_buf, strlen(send_buf));

  sprintf(send_buf, "host: %s\r\n", url->domain);
  write(s, send_buf, strlen(send_buf));

  sprintf(send_buf, "\r\n");
  write(s, send_buf, strlen(send_buf));

  // read
  read_size = read(s, getstr, BUF_LEN);

  if (0<read_size) {
    sscanf(getstr,
      "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
      dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,
      dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,ret,dummy,
      dummy,dummy,dummy);
  }
  close(s);
  free(url);

  int ret_index = 0;
  int ret2_index = 0;
  for(ret_index = 0,ret2_index = 0;ret_index < strlen(ret);){
    if(ret2_index%3==0){
      ret2[ret2_index] = '.';
      ret2_index++;
    }else{
      ret2[ret2_index] = ret[ret_index];
      ret2_index++;
      ret_index++;
    }
  }


  return ret2;
}


char ** strdate_1_svc(char **arg, struct svc_req *req) {
  static char *result;
  char ret[512];
  for(int i=strlen(arg[0])-1;i>=0;i--){
    if(arg[0][i] == '-')arg[0][i] = '\0';
    else break;
  }
  printf("受信文字列：%s\n",arg[0]);
  strcpy(arg[0],get_puzz_route(arg[0]));
  printf("送信文字列：%s\n",arg[0]);
  return(&arg[0]);
}
