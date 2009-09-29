#ifndef __BUFFER_H
#define __BUFFER_H


#include <openssl/bn.h>

namespace WIPNUX
{

typedef struct {
        unsigned char  *buf;           /* Buffer for data. */
        unsigned int    alloc;         /* Number of bytes allocated for data. */
        unsigned int    offset;        /* Offset of first byte containing data. */
        unsigned int    end;           /* Offset of last byte containing data. */
}       Buffer;

void     buffer_init(Buffer *);
void     buffer_clear(Buffer *);
void     buffer_free(Buffer *);

unsigned int    buffer_len(Buffer *);
void    *buffer_ptr(Buffer *);

void     buffer_append(Buffer *, const void *, unsigned int);
void    *buffer_append_space(Buffer *, unsigned int);

int      buffer_check_alloc(Buffer *, unsigned int);

void     buffer_get(Buffer *, void *, unsigned int);

void     buffer_consume(Buffer *, unsigned int);
void     buffer_consume_end(Buffer *, unsigned int); 

void     buffer_dump(Buffer *);   

unsigned int   buffer_get_int(Buffer *);
void    buffer_put_int(Buffer *, unsigned int);
unsigned int   buffer_get_int(Buffer *);

int     buffer_get_char(Buffer *);
void    buffer_put_char(Buffer *, int);

void   *buffer_get_string(Buffer *, unsigned int *);
void   *buffer_get_string_ptr(Buffer *, unsigned int *);
void    buffer_put_string(Buffer *, const void *, unsigned int);
void    buffer_put_cstring(Buffer *, const char *);

int buffer_put_bignum2(Buffer *buffer, const BIGNUM *value);
int buffer_get_bignum2(Buffer *buffer, BIGNUM *value);
void *buffer_get_string_ret(Buffer *buffer, unsigned int *length_ptr);
}

#endif
