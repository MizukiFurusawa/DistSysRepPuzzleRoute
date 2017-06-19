#include <stdio.h>
#include <rpc/rpc.h>
#include "strcap.h"
int main(int argc, char **argv)
{
  char *server; /* 遠隔サーバ */
  CLIENT *client; /* クライアント情報 */
  char **result; /* 遠隔呼び出しの結果 */
  /* サーバ名をコマンドラインから取る */
 
  if (argc != 3) {
    fprintf(stderr, "usage: rdate hostname string\n");
    exit(1);
  }
  server = argv[1];
  *result = argv[2];
  printf("arg = %s\n", *result);
  /* UDP 型のクライアントを作る */
  client = clnt_create(server, DATE_PROG, DATE_VERS, "udp");
  if (client == NULL) {
    clnt_pcreateerror(server); /* 接続失敗 */
    exit(2);
      }
  /* strdate を呼ぶ */
  result = strdate_1(result, client);
  if (result == NULL) {
    clnt_perror(client, server); /* 呼び出し失敗 */
    exit(2);
  }
  printf("Run on host %s = %s\n", server, *result);
  clnt_destroy(client); /* クライアント情報を破棄 */
  exit(0);
}
