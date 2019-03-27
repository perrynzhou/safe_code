/*************************************************************************
    > File Name: str_svc.c
  > Author:perrynzhou 
  > Mail:zhou.lin@vivo.com 
  > Created Time: Tue 26 Mar 2019 09:46:17 PM EDT
 ************************************************************************/
#include "str_convert.h"
#include <rpc/rpc.h>
#include <stdio.h>
#include <string.h>
#define MAX_BUF_LEN (512)

char **convertion_1_svc(char **ptr, struct svc_req *req)
{
  if (NULL == ptr || *ptr == NULL)
  {
    return NULL;
  }
  static char buf[MAX_BUF_LEN] = {'\0'};
  static char *p;
  char *data = *ptr;
  size_t len = strlen(data);
  for (int i = 0; i < len; i++)
  {
    buf[i] = (data[i] | 32);
  }
  buf[len] = '\0';
  p = (char *)&buf;
  return (char **)&p;
}