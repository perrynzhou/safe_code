/*************************************************************************
  > File Name: ttcp.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: 三 10/31 12:58:14 2018
 ************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
void get_sock_info(int fd, char *str)
{
  struct sockaddr ip_addr_struct;
  socklen_t len = sizeof(ip_addr_struct);
  getpeername(fd, (struct sockaddr *)&ip_addr_struct, &len);
  struct sockaddr_in *s = (struct sockaddr_in *)&ip_addr_struct;
  int port = ntohs(s->sin_port);
  char ip_addr[64] = {'\0'};
  inet_ntop(AF_INET, &s->sin_addr, (char *)&ip_addr, 64);
  sprintf(str, "%s:%d", ip_addr, port);
}
int wirte_n(int fd, const void *buf, size_t len)
{
  int writen = 0;
  while (writen < len)
  {
    size_t w = write(fd, (const char *)buf + writen, len - writen);
    if (w > 0)
    {
      writen += w;
    }
    else if (w == 0)
    {
      break;
    }
    else if (errno != EINTR)
    {
      printf("write error: %s(errno: %d)\n", strerror(errno), errno);
      break;
    }
  }
  return writen;
}
int read_n(int fd, void *buf, size_t len)
{
  int readn = 0;
  while (readn < len)
  {
    size_t r = read(fd, (char *)buf + r, len - r);
    if (r > 0)
    {
      readn += n;
    }
    else if (r == 0)
    {
      break;
    }
    else if (r != EINTR)
    {
      printf("read error: %s(errno: %d)\n", strerror(errno), errno);
      break;
    }
  }
  return readn;
}