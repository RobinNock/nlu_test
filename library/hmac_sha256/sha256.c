/*********************************************************************
* Filename:   sha256.c
* Original Author:     Brad Conte (brad AT bradconte.com)
* Labeled and modified by: AnSheng(https://github.com/monkeyDemon)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Performs known-answer tests on the corresponding SHA1
	          implementation. These tests do not encompass the full
	          range of available test vectors, however, if the tests
	          pass it is very, very likely that the code is correct
	          and was compiled properly. This code also serves as
	          example usage of the functions.
*********************************************************************/
 

/*************************** HEADER FILES ***************************/
#include <stdlib.h>
#include <string.h>
#include "sha256.h"

/****************************** MACROS ******************************/
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

/**************************** VARIABLES *****************************/
static const WORD k[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

/*********************** FUNCTION DEFINITIONS ***********************/
void sha256_transform(SHA256_CTX *ctx, const BYTE databuf[])
{
	WORD a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

	// initialization 
	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (databuf[j] << 24) | (databuf[j + 1] << 16) | (databuf[j + 2] << 8) | (databuf[j + 3]);
	for ( ; i < 64; ++i)
		m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

	a = ctx->state[0];
	b = ctx->state[1];
	c = ctx->state[2];
	d = ctx->state[3];
	e = ctx->state[4];
	f = ctx->state[5];
	g = ctx->state[6];
	h = ctx->state[7];

	for (i = 0; i < 64; ++i) {
		t1 = h + EP1(e) + CH(e,f,g) + k[i] + m[i];
		t2 = EP0(a) + MAJ(a,b,c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;
	ctx->state[5] += f;
	ctx->state[6] += g;
	ctx->state[7] += h;
}

void SHA256_Init(SHA256_CTX *ctx)
{
	ctx->datalen = 0;
	ctx->bitlen = 0;
	ctx->state[0] = 0x6a09e667;
	ctx->state[1] = 0xbb67ae85;
	ctx->state[2] = 0x3c6ef372;
	ctx->state[3] = 0xa54ff53a;
	ctx->state[4] = 0x510e527f;
	ctx->state[5] = 0x9b05688c;
	ctx->state[6] = 0x1f83d9ab;
	ctx->state[7] = 0x5be0cd19;
}

void SHA256_Update(SHA256_CTX *ctx, const BYTE databuf[], WORD len)
{
	WORD i;

	for (i = 0; i < len; ++i) {
		ctx->ctxdata[ctx->datalen] = databuf[i];
		ctx->datalen++;
		if (ctx->datalen == 64) {
			// 64 byte = 512 bit  means the buffer ctx->data has fully stored one chunk of message
			// so do the sha256 hash map for the current chunk
			sha256_transform(ctx, ctx->ctxdata);
			ctx->bitlen += 512;
			ctx->datalen = 0;
		}
	}
}

void SHA256_Final(SHA256_CTX *ctx, BYTE hash[])
{
	WORD i;

	i = ctx->datalen;

	// Pad whatever data is left in the buffer.
	if (ctx->datalen < 56) {
		ctx->ctxdata[i++] = 0x80;  // pad 10000000 = 0x80
		while (i < 56)
			ctx->ctxdata[i++] = 0x00;
	}
	else {
		ctx->ctxdata[i++] = 0x80;
		while (i < 64)
			ctx->ctxdata[i++] = 0x00;
		sha256_transform(ctx, ctx->ctxdata);
		memset(ctx->ctxdata, 0, 56);
	}

	// Append to the padding the total message's length in bits and transform.
	ctx->bitlen += ctx->datalen * 8;
	ctx->ctxdata[63] = ctx->bitlen;
	ctx->ctxdata[62] = ctx->bitlen >> 8;
	ctx->ctxdata[61] = ctx->bitlen >> 16;
	ctx->ctxdata[60] = ctx->bitlen >> 24;
	ctx->ctxdata[59] = ctx->bitlen >> 32;
	ctx->ctxdata[58] = ctx->bitlen >> 40;
	ctx->ctxdata[57] = ctx->bitlen >> 48;
	ctx->ctxdata[56] = ctx->bitlen >> 56;
	sha256_transform(ctx, ctx->ctxdata);

	// copying the final state to the output hash(use big endian).
	for (i = 0; i < 4; ++i) {
		hash[i]      = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4]  = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8]  = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
	}

}

int sha256_test()
{
	// test data
	BYTE text1[] = {"??????????^_^"};
	BYTE text2[] = {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"};
	BYTE text3[] = {"aaaaaaaaaa"};
	// the true SHA256 result of test data
	// get from the online varify website  
	BYTE hash1[SHA256_BLOCK_SIZE] = {0x21,0xf7,0xec,0xe9,0xa3,0x8b,0xfd,0xa3,0xa2,0x5f,0xd2,0x33,0x83,0x42,0x65,0xf3,
					 0x6d,0x12,0xb3,0xc0,0x3a,0xc5,0xb5,0x8e,0xfb,0xe8,0x95,0xfd,0xad,0x25,0xd0,0xa4};
	BYTE hash2[SHA256_BLOCK_SIZE] = {0x24,0x8d,0x6a,0x61,0xd2,0x06,0x38,0xb8,0xe5,0xc0,0x26,0x93,0x0c,0x3e,0x60,0x39,
	                                 0xa3,0x3c,0xe4,0x59,0x64,0xff,0x21,0x67,0xf6,0xec,0xed,0xd4,0x19,0xdb,0x06,0xc1};
	BYTE hash3[SHA256_BLOCK_SIZE] = {0xcd,0xc7,0x6e,0x5c,0x99,0x14,0xfb,0x92,0x81,0xa1,0xc7,0xe2,0x84,0xd7,0x3e,0x67,
	                                 0xf1,0x80,0x9a,0x48,0xa4,0x97,0x20,0x0e,0x04,0x6d,0x39,0xcc,0xc7,0x11,0x2c,0xd0};
	BYTE buf[SHA256_BLOCK_SIZE];
	SHA256_CTX ctx;
	int idx;
	int pass = 1;

	SHA256_Init(&ctx);
	SHA256_Update(&ctx, text1, strlen((const char *)text1));//??????????????????????????
	SHA256_Final(&ctx, buf);
	pass = pass && !memcmp(hash1, buf, SHA256_BLOCK_SIZE);

	SHA256_Init(&ctx);
	SHA256_Update(&ctx, text2, strlen((const char *)text2));
	SHA256_Final(&ctx, buf);
	pass = pass && !memcmp(hash2, buf, SHA256_BLOCK_SIZE);

	SHA256_Init(&ctx);
	for (idx = 0; idx < 100000; ++idx)
	   SHA256_Update(&ctx, text3, strlen((const char *)text3));
	SHA256_Final(&ctx, buf);
	pass = pass && !memcmp(hash3, buf, SHA256_BLOCK_SIZE);

	return(pass);
}


