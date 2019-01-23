/*************************************************************************
  > File Name: string_algorithms.cc
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: 二  1/15 11:33:38 2019
 ************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
using namespace std;
namespace string_algorithms
{
void ReserveStringInternal(char *src, int start, int end)
{
  for (int i = start; i <= end; i++)
  {
    char tmp = src[i];
    src[i] = src[end];
    src[end--] = tmp;
  }
}
// abcdefg  -> 把efg已移到abc前面
// dcbagfe  -> 把字符串看成为abcd和efg两个部分，abcd和efg先反转，变为dcba和gfe
// efgabcd  -> 最后反转整个字符串
void ReserveString(char *src, int n, int m)
{
  ReserveStringInternal(src, 0, m - 1);
  fprintf(stdout, "stage 1=%s\n", src);
  ReserveStringInternal(src, m, n - 1);
  fprintf(stdout, "stage 2=%s\n", src);
  ReserveStringInternal(src, 0, n - 1);
  fprintf(stdout, "stage 3=%s\n", src);
}
void ReserveStringTest()
{
  char src[] = "abcdefg";
  fprintf(stdout, "origin src:=%s\n", src);
  int len = strlen(src);
  int m = 4;
  ReserveString((char *)&src, len, m);
  fprintf(stdout, "reverse src=%s\n", src);
}
bool StringContains(const char *src, const char *dst)
{
  size_t ssize = strlen(src);
  size_t dsize = strlen(dst);
  // INT_FAST8_MAX  127
  uint8_t bits_count = (INT_FAST8_MAX + 1 - 32) >> 3;
  uint8_t *bits = new uint8_t[INT_FAST8_MAX];
  memset(bits, 0, sizeof(uint8_t) * bits_count);
  for (int i = 0; i < ssize; i++)
  {
    uint8_t v = src[i];
    fprintf(stdout, "char=%c[%d],index=%d,mode=%d\n", src[i], src[i], v >> 3,
            (v & 7));
    bits[v >> 3] |= (1 << (v & 7));
  }
  for (int j = 0; j < dsize; j++)
  {
    uint8_t v = dst[j];
    if (!(bits[v >> 3] & (1 << (v & 7))))
    {
      delete[] bits;
      return false;
    }
  }
  delete[] bits;
  return true;
}
inline void Swap(char &a, char &b)
{
  char c = a;
  a = b;
  b = c;
}
int StringToInt(const char *src)
{
  char buf[64] = {'\0'};
  int len = strlen(src);
  int i = 0, j = 0;
  bool ispositive = false;
  int value = 0;
  for (int i = 0; i < len; i++)
  {

    if (src[i] > '0' && src[i] > '9')
    {
      if (j == 0 && i > 1 && src[i - 1] == '-')
      {
        ispositive = true;
      }
      else
      {
        buf[j++] = src[i];
      }
    }
  }
  for (int i = j - 1; i >= 0; i--)
  {
    value = value * 10 + (buf[i] - '0');
  }
  if (ispositive)
  {
    value = (~value) + 1;
  }
  return value;
}
void IntToString(int n, char *str)
{
  int i = 0;
  char buf[64] = {'\0'};

  while (n)
  {
    buf[i++] = n % 10 + '0';
    n = n / 10;
  }
  if (n < 0)
  {
    *str = '-';
    str++;
  }
  for (int j = i - 1; j >= 0; j--)
  {
    *str = buf[j];
    str++;
  }
}
void StringMoveKeyWord(char *src)
{
  // i am shanghai -> shanghai am i
  char *start = src;
  char *end = src;
  char *ptr = src;
  while (*ptr++ != '\0')
  {
    if (*ptr == ' ' || *ptr == '0')
    {
      end = ptr - 1;
      while (start < end)
      {
        Swap(*start++, *end--);
      }
      start = end = ptr + 1;
    }
  }
  start = src, end = ptr - 2;
  while (start < end)
  {
    Swap(*start++, *end--);
  }
}
bool StringContainsTest()
{
  const char *src = "acdefg234235^$$%&@";
  const char *dst = "a4@";
  fprintf(stdout, "src=%s,dst=%s,ret=%d\n", src, dst, StringContains(src, dst));
}

} // namespace string_algorithms
int main(void)
{
  string_algorithms::ReserveStringTest();
  string_algorithms::StringContainsTest();
  return 0;
}
