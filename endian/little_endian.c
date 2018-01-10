/*************************************************************************
	> File Name: little_endian.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月08日 星期一 21时27分35秒
 ************************************************************************/

#include <stdio.h>
#include <endian.h>
#include <stdint.h>
#if __BYTE_ORDER == __BIG_ENDIAN
inline uint16_t swap2bytes(uint16_t val)
{
    return val;
}
inline uint32_t swap4bytes(uint32_t val)
{
    return val;
}
inline uint64_t swap8bytes(uint64_t val)
{
    return val;
}
#else
inline uint16_t swap2bytes(uint16_t val)
{
    return ((((val) >> 8) & 0x00FF) | (((val) << 8) & 0xFF00));
}

inline uint32_t swap4bytes(uint32_t val)
{
    return ((((val) >> 24) & 0x000000FF) | (((val) >> 8) & 0x0000FF00) | (((val) << 8) & 0x00FF0000) | (((val) << 24) & 0xFF000000));
}
// Swap 8 byte, 64 bit values:

inline uint64_t swap8bytes(uint64_t val)
{
    return ((((val) >> 56) & 0x00000000000000FF) | (((val) >> 40) & 0x000000000000FF00) | (((val) >> 24) & 0x0000000000FF0000) | (((val) >> 8) & 0x00000000FF000000) | (((val) << 8) & 0x000000FF00000000) | (((val) << 24) & 0x0000FF0000000000) | (((val) << 40) & 0x00FF000000000000) | (((val) << 56) & 0xFF00000000000000));
}
#endif

int main(void)
{
    uint32_t val = 0xAABBCCDD;
    fprintf(stdout, " val=0x%X\n", swap4bytes(val));
    return 0;
}
