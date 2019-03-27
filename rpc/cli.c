/*************************************************************************
    > File Name: str_cli.c
  > Author:perrynzhou 
  > Mail:zhou.lin@vivo.com 
  > Created Time: Tue 26 Mar 2019 09:46:40 PM EDT
 ************************************************************************/
#include "str_convert.h"
#include <rpc/rpc.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[])
{
  CLIENT *cli;
  char **p;

  if (argc != 3)
  {
    printf("Usage: %s {ip} {string content}\n", argv[1]);
    exit(1);
  }

  cli = clnt_create(argv[1], STR_CONVERTION_PROG, STR_CONVERTION_VERS, "tcp");
  if (cli == NULL)
  {
    clnt_pcreateerror(argv[1]);
    exit(1);
  }

  printf("...Ready For Start RPC Client\n");
  char input_str[256] = {'\0'};
  char *ptr = (char *)&input_str;
  strncpy((char *)input_str, argv[2], strlen(argv[2]));
  p = convertion_1(&ptr, cli);

  printf("...After RPC Service\n");
  if (p == NULL)
  {
    clnt_perror(cli, argv[1]);
    exit(1);
  }
  printf("Result string=%s\n", *p);

  return 0;
}