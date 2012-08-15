/*
 * This file is part of the libemb project.
 *
 * Copyright (C) 2011 Stefan Wendler <sw@kaltpost.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdarg.h>

#include "conio.h"

void cio_print(char *line)
{
     int   i = 0;

     while(line[i] != 0) {
          cio_printc(line[i++]);
     }
}

void cio_printi(int n)
{
     int buffer[16];
     int i;
     int j;

     if(n == 0) {
          cio_printc('0');
          return;
     }

     for (i = 15; i > 0 && n > 0; i--) {
          buffer[i] = (n%10)+'0';
          n /= 10;
     }

     for(j = i+1; j <= 15; j++) {
          cio_printc(buffer[j]);
     }
}

void cio_printb(int n, int size)
{

     int i;

     int mask = 1 << (size - 1);

     for(i = 0; i < size; i++) {
          if((n & (mask >> i)) != 0) {
               cio_printc('1');
          } else {
               cio_printc('0');
          }
     }

}

static const unsigned long _dv[] = {
//   4294967296       // 32 bit unsigned max
     1000000000,     // +0
     100000000,     // +1
     10000000,     // +2
     1000000,     // +3
     100000,     // +4
//   65535      // 16 bit unsigned max
     10000,    // +5
     1000,    // +6
     100,    // +7
     10,    // +8
     1,    // +9
};

static void _puth(unsigned int n)
{
     static const char hex[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
     cio_printc(hex[n & 15]);
}

static void _xtoa(unsigned long x, const unsigned long *dp)
{
     char c;
     unsigned long d;

     if(x) {
          while(x < *dp) ++dp;
          do {
               d = *dp++;
               c = '0';
               while(x >= d) ++c, x -= d;
               cio_printc(c);
          } while(!(d & 1));
     } else
          cio_printc('0');
}

void cio_printf(char *format, ...)
{
     char c;
     int i;
     long n;

     va_list a;
     va_start(a, format);

     while((c = *format++)) {
          if(c == '%') {
               switch(c = *format++) {
               case 's':                       // String
                    cio_print(va_arg(a, char*));
                    break;
               case 'c':                       // Char
                    cio_printc((char)va_arg(a, int));
                    break;
               case 'i':                       // 16 bit Integer
               case 'u':                       // 16 bit Unsigned
                    i = va_arg(a, int);
                    if(c == 'i' && i < 0) i = -i, cio_printc('-');
                    _xtoa((unsigned)i, _dv + 5);
                    break;
               case 'l':                       // 32 bit Long
               case 'n':                       // 32 bit uNsigned loNg
                    n = va_arg(a, long);
                    if(c == 'l' &&  n < 0) n = -n, cio_printc('-');
                    _xtoa((unsigned long)n, _dv);
                    break;
               case 'x':                       // 16 bit heXadecimal
                    i = va_arg(a, int);
                    _puth(i >> 12);
                    _puth(i >> 8);
                    _puth(i >> 4);
                    _puth(i);
                    break;
               case 0:
                    return;
               default:
                    goto bad_fmt;
               }
          } else
bad_fmt:
               cio_printc(c);
     }
     va_end(a);
}

