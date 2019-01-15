/*************************************************************************
  > File Name: string_algorithms.cc
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: 二  1/15 11:33:38 2019
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
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
  //INT_FAST8_MAX  127
  uint8_t bits_count = (INT_FAST8_MAX + 1 - 32) >> 3;
  uint8_t *bits = new uint8_t[INT_FAST8_MAX];
  memset(bits, 0, sizeof(uint8_t) * bits_count);
  for (int i = 0; i < ssize; i++)
  {
    uint8_t v = src[i];
    fprintf(stdout, "char=%c[%d],index=%d,mode=%d\n", src[i], src[i], v >> 3, (v & 7));
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
