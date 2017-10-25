/* selftest.c - basic StreamHash5 tests
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

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "streamhash5.h"

/* benchmark complexity (input length and number of iterations) */
#define LEN (16*1024)
#define RTR (1024*1024)

static void dump(const char *txt, const unsigned char *hash) {
    unsigned i;

    printf("%s:\n", txt);
    for(i=0; i<64; ++i) {
        if(i%8 == 0)
            printf("        ");
        printf("0x%02x", hash[i]);
        if(i==63) {
            printf("\n");
            break;
        }
        printf(",%s", i%8==7 ? "\n" : " ");
    }
}

static void test_vector() {
    const char test_msg[] = "The quick brown fox jumps over the lazy dog";
    const unsigned char test_md[64] = {
        0x8f, 0x54, 0x93, 0x4e, 0xb8, 0x43, 0x89, 0x62,
        0x00, 0x8a, 0x56, 0x0b, 0xc1, 0xa6, 0x2c, 0xde,
        0xc1, 0x5f, 0x56, 0x61, 0x48, 0x96, 0x9f, 0x1c,
        0x28, 0xef, 0x8c, 0xb7, 0x06, 0x94, 0x37, 0x70,
        0xa6, 0x46, 0x2b, 0x9c, 0x29, 0xc9, 0x7a, 0x6f,
        0x15, 0xa7, 0xf6, 0x2a, 0xb3, 0xa5, 0x68, 0xa9,
        0x8e, 0x8e, 0x21, 0xdb, 0x68, 0x90, 0xd6, 0x10,
        0xb8, 0x09, 0x27, 0xb3, 0x63, 0x16, 0x56, 0x01};
    unsigned char md[64];

    assert(STREAMHASH5(test_msg, sizeof test_msg - 1, md) == 1);
    if(memcmp(test_md, md, sizeof md)) {
        dump("Expected", test_md);
        dump("Received", md);
        exit(1);
    }
}

static void test_alignment(int rel) {
    FILE *f;
    unsigned char msg[128], md_ref[64], md[128];
    STREAMHASH5_CTX ctx;

    f = fopen("/dev/urandom", "r");
    assert(f != NULL);
    assert(fread(msg, sizeof msg, 1, f) == 1);
    fclose(f);

    /* make sure the input is *not* aligned */
    assert(STREAMHASH5(msg+rel, 33, md_ref) == 1);

    assert(STREAMHASH5_Init(&ctx) == 1);
    assert(STREAMHASH5_Update(&ctx, msg+rel, 8) == 1);
    assert(STREAMHASH5_Update(&ctx, msg+8+rel, 16) == 1);
    assert(STREAMHASH5_Update(&ctx, msg+24+rel, 1) == 1);
    assert(STREAMHASH5_Update(&ctx, msg+25+rel, 8) == 1);
    assert(STREAMHASH5_Final(&ctx, md+rel) == 1);
    assert(memcmp(md_ref, md+rel, 64) == 0);
}

static void benchmark() {
    long i;
    unsigned char msg[LEN], md[64];
    time_t start;

    printf("Message size: %d bytes\n", LEN);
    memset(msg, 0, sizeof msg);
    assert(STREAMHASH5(msg, LEN, md) == 1); /* populate the caches */

    start=clock();
    for(i=0; i<RTR; ++i)
        STREAMHASH5(msg, LEN, md);
    printf("Performance: %.2f GB/s\n",
        0.000000001*LEN*RTR/(clock()-start)*CLOCKS_PER_SEC);
}

int main() {
    int i;

    test_vector();
    for(i=0; i<32; ++i)
        test_alignment(i);
    printf("Tests succeeded\n");
    benchmark();
    return 0;
}

/* end of selftest.c */
