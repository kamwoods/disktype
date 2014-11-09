/*
 * ewf.c
 * Layered data source for EWF images via libewf.
 * For use with disktype, Copyright (c) 2003 Christoph Pfisterer
 *
 * Copyright (c) 2007 David Loveall
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "global.h"

#ifdef USE_LIBEWF
#include <libewf.h>

/*
 * types
 */

typedef struct ewf_source {
 SOURCE c;
 LIBEWF_HANDLE *e;
} EWF_SOURCE;

/*
 * helper functions
 */

static SOURCE *init_ewf_source(char * const filenames[], uint16_t file_amount);
static int read_block_ewf(SOURCE *s, u8 pos, void *buf);
static void close_ewf(SOURCE *s);

/*
 * ewf analyzer
 */

void analyze_ewf(char * const filenames[], uint16_t file_amount)
{
 SOURCE *s;

 /* create and analyze wrapped source */
 s = init_ewf_source(filenames, file_amount);
 analyze_source(s, 1);
 close_source(s);
}

/*
 * initialize the ewf source
 */

static SOURCE *init_ewf_source(char * const filenames[], uint16_t file_amount)
{
 EWF_SOURCE *src;

 src = (EWF_SOURCE *)malloc(sizeof(EWF_SOURCE));
 if (src == NULL)
 bailout("Out of memory");
 memset(src, 0, sizeof(EWF_SOURCE));

 src->e = libewf_open(filenames, file_amount, LIBEWF_OPEN_READ);
 if (src->e == NULL)
 bailout("Can't open EWF file");

 src->c.size_known = 1;

#if defined( LIBEWF_STRING_DIGEST_HASH_LENGTH_MD5 )
 src->c.size = libewf_get_media_size(src->e);
 src->c.blocksize = libewf_get_bytes_per_sector(src->e);
#else
 if( libewf_get_media_size( src->e, &( src->c.size ) ) != 1 )
 bailout("Unable to get media size of EWF file");
 if( libewf_get_bytes_per_sector( src->e, (uint32_t *) &( src->c.blocksize ) ) != 1 )
 bailout("Unable to get sector size of EWF file");
#endif
 src->c.read_block = read_block_ewf;
 src->c.close = close_ewf;

 return (SOURCE *)src;
}

/*
 * raw read
 */

static int read_block_ewf(SOURCE *s, u8 pos, void *buf)
{
 EWF_SOURCE *es = (EWF_SOURCE *)s;

 /* read from lower layer */
 if(libewf_read_random(es->e, buf, es->c.blocksize, pos) == -1) {
 return 0;
 }

 return 1;
}

/*
 * cleanup
 */

static void close_ewf(SOURCE *s)
{
 EWF_SOURCE *es = (EWF_SOURCE *)s;

 libewf_close(es->e);
}

#endif
