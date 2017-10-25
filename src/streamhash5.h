/* streamhash5.h version 1.0
 *
 * ISC License
 *
 * Copyright (c) 2017, Michał Trojnara
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stddef.h>
#include <wmmintrin.h>

typedef struct {
    /* TODO: put other implementations here */
#ifdef __SSE2__
    __m128i state[4];   /* the state vector */
    __m128i bit_len;    /* number of bits processed so far */
    __m128i buf_data;   /* up to 128 unprocessed bits */
    size_t buf_len;     /* number of data bytes in the buffer */
#endif
} STREAMHASH5_CTX;

int STREAMHASH5_Init(STREAMHASH5_CTX *state);
int STREAMHASH5_Update(STREAMHASH5_CTX *state, const void *data, size_t len);
int STREAMHASH5_Final(STREAMHASH5_CTX *state, void *md);
int STREAMHASH5(const void *data, size_t len, void *md);

/* end of streamhash5.h */
