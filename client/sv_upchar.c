#include <string.h>
#include <ctype.h>
#include <rpc/rpc.h>
#include "strcap.h"

char ** strdate_1_svc(char **arg, struct svc_req *req) {


  static char *result; 
  int numrcv;
  int i;

  numrcv = strlen(arg[0]);
  
  for (i=0; i< numrcv; i++){ // bufの中の小文字を大文字に変換
    if(isalpha(arg[0][i])) arg[0][i] = toupper(arg[0][i]);
  }
    return(&arg[0]);
}
