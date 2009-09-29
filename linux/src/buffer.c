#include "buffer.h"
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>

#define BUFFER_MAX_CHUNK        0x100000
#define BUFFER_MAX_LEN          0xa00000
#define BUFFER_ALLOCSZ          0x008000

namespace WIPNUX
{

#define error printf

int buffer_get_ret(Buffer *buffer, void *buf, unsigned int len)
{
        if (len > buffer->end - buffer->offset) {
                error("buffer_get_ret: trying to get more bytes %d than in buffer %d",
                    len, buffer->end - buffer->offset);
                return (-1);
        }
        memcpy(buf, buffer->buf + buffer->offset, len);
        buffer->offset += len;
        return (0);
}

unsigned int get_u32(const void *vp)
{
        const u_char *p = (const u_char *)vp;
        unsigned int v;

        v  = (unsigned int)p[0] << 24;
        v |= (unsigned int)p[1] << 16;
        v |= (unsigned int)p[2] << 8;
        v |= (unsigned int)p[3];

        return (v);
}

void put_u32(void *vp, unsigned int v)
{
        unsigned char *p = (unsigned char *)vp;

        p[0] = (unsigned char)(v >> 24) & 0xff;
        p[1] = (unsigned char)(v >> 16) & 0xff;
        p[2] = (unsigned char)(v >> 8) & 0xff;
        p[3] = (unsigned char)v & 0xff;
}

void buffer_clear(Buffer *buffer)
{
        buffer->offset = 0;
        buffer->end = 0;   
}


void buffer_consume(Buffer *buffer, unsigned int bytes)
{
        if (bytes > buffer->end - buffer->offset) 
	  return;

        buffer->offset += bytes;   
}


static int buffer_compact(Buffer *buffer)
{
        /*
         * If the buffer is quite empty, but all data is at the end, move the
         * data to the beginning.
         */
        if (buffer->offset > MIN(buffer->alloc, BUFFER_MAX_CHUNK)) {
                memmove(buffer->buf, buffer->buf + buffer->offset,
                        buffer->end - buffer->offset);
                buffer->end -= buffer->offset;
                buffer->offset = 0;
                return (1);
        }
        return (0);
}   

void *buffer_append_space(Buffer *buffer, unsigned int len)
{
        unsigned int newlen;
        void *p;

        if (len > BUFFER_MAX_CHUNK)
          return NULL;

        /* If the buffer is empty, start using it from the beginning. */
        if (buffer->offset == buffer->end) {
                buffer->offset = 0;
                buffer->end = 0;   
        }
restart: 
        /* If there is enough space to store all data, store it now. */
        if (buffer->end + len < buffer->alloc) {
                p = buffer->buf + buffer->end;  
                buffer->end += len;
                return p;
        }

        /* Compact data back to the start of the buffer if necessary */
        if (buffer_compact(buffer))
                goto restart;

        /* Increase the size of the buffer and retry. */
        newlen = roundup(buffer->alloc + len, BUFFER_ALLOCSZ);
        if (newlen > BUFFER_MAX_LEN)
          return NULL;
        buffer->buf = (unsigned char *)realloc(buffer->buf, newlen);
        buffer->alloc = newlen;
        goto restart;
        /* NOTREACHED */
}

void buffer_append(Buffer *buffer, const void *data, unsigned int len)
{
        void *p;
        p = buffer_append_space(buffer, len);
        memcpy(p, data, len);
}

int buffer_get_char_ret(char *ret, Buffer *buffer)
{
        if (buffer_get_ret(buffer, ret, 1) == -1) {
                error("buffer_get_char_ret: buffer_get_ret failed");
                return (-1);
        }
        return (0);
}

void buffer_init(Buffer *buffer)
{
        const unsigned int len = 4096;

        buffer->alloc = 0;
        buffer->buf = (unsigned char *)malloc(len);
        buffer->alloc = len;
        buffer->offset = 0;
        buffer->end = 0;
}

void buffer_free(Buffer *buffer)
{
        if (buffer->alloc > 0) {
                memset(buffer->buf, 0, buffer->alloc);
                buffer->alloc = 0;
                free(buffer->buf);
        }
}

unsigned int buffer_len(Buffer *buffer)
{
        return buffer->end - buffer->offset;
}

void * buffer_ptr(Buffer *buffer)
{
        return buffer->buf + buffer->offset;
}

int buffer_get_char(Buffer *buffer)
{
        char ch;

        if (buffer_get_char_ret(&ch, buffer) == -1)
          return 0;
        return (unsigned char) ch;
} 

void buffer_put_char(Buffer *buffer, int value)
{
        char ch = value;

        buffer_append(buffer, &ch, 1);
}  

unsigned int buffer_get_int(Buffer *buffer)
{
        u_char buf[4];

        if (buffer_get_ret(buffer, (char *) buf, 4) == -1)
                return (-1);
        return get_u32(buf);
}

void buffer_put_int(Buffer *buffer, unsigned int value)
{
        char buf[4];

        put_u32(buf, value);
        buffer_append(buffer, buf, 4);
}

void buffer_put_string(Buffer *buffer, const void *buf, unsigned int len)
{
        buffer_put_int(buffer, len);
        buffer_append(buffer, buf, len);
}
void buffer_put_cstring(Buffer *buffer, const char *s)
{
        if (s == NULL)
          return;
        buffer_put_string(buffer, s, strlen(s));
}

int buffer_put_bignum2(Buffer *buffer, const BIGNUM *value)
{
        unsigned int bytes;
        unsigned char *buf;
        int oi;
        unsigned int hasnohigh = 0;

        if (BN_is_zero(value)) {
                buffer_put_int(buffer, 0);
                return 0;
        }
        if (value->neg) {
                error("buffer_put_bignum2_ret: negative numbers not supported");
                return (-1);
        }
        bytes = BN_num_bytes(value) + 1; /* extra padding byte */
        if (bytes < 2) {
                error("buffer_put_bignum2_ret: BN too small");
                return (-1);
        }
        buf = (unsigned char *)malloc(bytes);
        buf[0] = 0x00;
        /* Get the value of in binary */
        oi = BN_bn2bin(value, buf+1);   
        if (oi < 0 || (unsigned int)oi != bytes - 1) {
                error("buffer_put_bignum2_ret: BN_bn2bin() failed: "
                    "oi %d != bin_size %d", oi, bytes);
                free(buf);
                return (-1);
        }
        hasnohigh = (buf[1] & 0x80) ? 0 : 1;
        buffer_put_string(buffer, buf+hasnohigh, bytes-hasnohigh);
        memset(buf, 0, bytes);
        free(buf);
        return (0);
}

void *buffer_get_string_ret(Buffer *buffer, unsigned int *length_ptr)
{
        unsigned char *value;
        unsigned int len;

        /* Get the length. */
        len = buffer_get_int(buffer);
        if (len > 256 * 1024) {
                error("buffer_get_string_ret: bad string length %u", len);
                return (NULL);
        }
        /* Allocate space for the string.  Add one byte for a null character. */
        value = (unsigned char *)malloc(len + 1);
        /* Get the string. */
        if (buffer_get_ret(buffer, value, len) == -1) {
                error("buffer_get_string_ret: buffer_get failed");
                free(value);
                return (NULL);
        }
        /* Append a null character to make processing easier. */
        value[len] = 0;
        /* Optionally return the length of the string. */
        if (length_ptr)
                *length_ptr = len;
        return (value);
}  

int buffer_get_bignum2(Buffer *buffer, BIGNUM *value)
{
        unsigned int len;
        unsigned char *bin;

        if ((bin = (unsigned char *)buffer_get_string_ret(buffer, &len)) == NULL)
	{
                return (-1);
        }
        if (len > 0 && (bin[0] & 0x80)) 
        {
                free(bin);
                return (-1);
        }
        if (len > 8 * 1024) 
        {
                free(bin);
                return (-1);
        }
        if (BN_bin2bn(bin, len, value) == NULL) 
        {
                free(bin);
                return (-1);
        }
        free(bin);
        return (0);
}
}
