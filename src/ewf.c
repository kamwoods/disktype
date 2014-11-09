/*
 * ewf.c
 * Layered data source for EWF images via libewf.
 * For use with disktype, Copyright (c) 2003 Christoph Pfisterer
 *
 * Copyright (c) 2007 David Loveall
 *
 * Updated November 9, 2014 by Kam Woods
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

#ifndef LIBEWF_HANDLE
/* libewf version 2 no longer defines LIBEWF_HANDLE
 */
#define HAVE_LIBEWF_V2_API
#endif

/*
 * types
 */

typedef struct ewf_source {
  SOURCE c;
  libewf_handle_t *e;
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
  // TODO: Fix error handling
  //libewf_error_t *ewf_error;
  size64_t src_size = 0;
  uint32_t src_blocksize = 0;

  src = (EWF_SOURCE *)malloc(sizeof(EWF_SOURCE));

  if (src == NULL)
    bailout("Out of memory");
    memset(src, 0, sizeof(EWF_SOURCE));

    //src->e = libewf_open(filenames, file_amount, LIBEWF_OPEN_READ);
    //if (src->e == NULL)
    //  bailout("Can't open EWF file");

/* New EWF section */
    if ( libewf_handle_open (
           src->e,
           filenames, 
           file_amount, 
           LIBEWF_OPEN_READ,
           NULL ) != 1 )
    {
      bailout("Failed to open EWF handle");
    }

/**/

    src->c.size_known = 1;

#if defined( LIBEWF_STRING_DIGEST_HASH_LENGTH_MD5 )
    //src->c.size = libewf_get_media_size(src->e);
    if ( libewf_handle_get_media_size (
           src->e,
           &src_size,
           NULL ) !=1 )
    {
      src->c.size = 0;
    }
    else
    {
      src->c.size = src_size;
    }
    src->c.blocksize = libewf_get_bytes_per_sector(src->e);
#else
    if( libewf_handle_get_media_size( 
          src->e, 
          &src_size,
          NULL ) != 1 )
    {
      src->c.size = 0;
      //bailout("Unable to get media size of EWF file");
    }
    else
    {
      src->c.size = src_size;
    }
    if( libewf_handle_get_bytes_per_sector( 
          src->e, 
          &src_blocksize,
          NULL ) != 1 )
    {
      // TODO: put a default in somewhere
      //src->c.blocksize = DEFAULT_SECTOR_SIZE;
      bailout("Unable to get sector size of EWF file");
    }
    else
    {
      src->c.blocksize = src_blocksize;
    }
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
  int64_t read_size;

  /* read from lower layer */
  read_size = libewf_handle_read_random (
         es->e, 
         buf, 
         es->c.blocksize, 
         pos,
         NULL );
  if (read_size != es->c.blocksize)
  {
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

  libewf_handle_close(
    es->e,
    NULL );
}

#endif
