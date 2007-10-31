#ifndef __CRYPTO_AES
#define __CRYPTO_AES

#define MAXKC	(256/32)
#define MAXKB	(256/8)
#define MAXNR	14

typedef unsigned char	u8;	
typedef unsigned short	u16;	
typedef unsigned int	u32;

#define RIJNDAEL_BLOCKSIZE 16


class cryptoAES
{
protected:
	/*  The structure for key information */
	typedef struct {
		int	decrypt;
		int	Nr;			/* key-length-dependent number of rounds */
		u32	ek[4*(MAXNR + 1)];	/* encrypt key schedule */
		u32	dk[4*(MAXNR + 1)];	/* decrypt key schedule */
	} rijndael_ctx;

public:
	int KeyLen, BlockSize;
	char *Name;

	cryptoAES() 
	{
		KeyLen = 0;
		Name = "";
		BlockSize = 0;
	}

	void Encrypt(unsigned char *dst, unsigned char *src, int len) 
	{
		rijndael_ctx *ctx = &myaes.enc;
		u_char *iv = myaes.iv;
		u_char in[RIJNDAEL_BLOCKSIZE];
		u_char *cprev, *cnow, *plain;
		int i, j, blocks = len / RIJNDAEL_BLOCKSIZE;

		if (len == 0)
			return;
		if (len % RIJNDAEL_BLOCKSIZE)
			return;
//			fatal("rijndael_cbc_encrypt: bad len %d", len);
		cnow  = dst;
		plain = (u_char *) src;
		cprev = iv;
		for (i = 0; i < blocks; i++, plain+=RIJNDAEL_BLOCKSIZE,
			cnow+=RIJNDAEL_BLOCKSIZE) {
			for (j = 0; j < RIJNDAEL_BLOCKSIZE; j++)
				in[j] = plain[j] ^ cprev[j];
			rijndael_encrypt(ctx, in, cnow);
			cprev = cnow;
		}
		memcpy(iv, cprev, RIJNDAEL_BLOCKSIZE);
	}
	void Decrypt(unsigned char *dst, unsigned char *src, int len) 
	{
		rijndael_ctx *ctx = &myaes.dec;
		u_char *iv = myaes.iv;
		u_char ivsaved[RIJNDAEL_BLOCKSIZE];
		u_char *cnow  = (u_char *) (src+len-RIJNDAEL_BLOCKSIZE);
		u_char *plain = dst+len-RIJNDAEL_BLOCKSIZE;
		u_char *ivp;
		int i, j, blocks = len / RIJNDAEL_BLOCKSIZE;

		if (len == 0)
			return;
		if (len % RIJNDAEL_BLOCKSIZE)
			return;
//			fatal("rijndael_cbc_decrypt: bad len %d", len);
		memcpy(ivsaved, cnow, RIJNDAEL_BLOCKSIZE);
		for (i = blocks; i > 0; i--, cnow-=RIJNDAEL_BLOCKSIZE,
			plain-=RIJNDAEL_BLOCKSIZE) {
			rijndael_decrypt(ctx, cnow, plain);
			ivp = (i == 1) ? iv : cnow-RIJNDAEL_BLOCKSIZE;
			for (j = 0; j < RIJNDAEL_BLOCKSIZE; j++)
				plain[j] ^= ivp[j];
		}
		memcpy(iv, ivsaved, RIJNDAEL_BLOCKSIZE);
	}
	void SessionKey(unsigned char *key)
	{
		rijndael_set_key(&myaes.enc, key, 8*KeyLen, 1);
		rijndael_set_key(&myaes.dec, key, 8*KeyLen, 0);
	}
	void SessionIV(unsigned char *key)
	{
//		if (key == NULL || ivlen != RIJNDAEL_BLOCKSIZE) 
//			return;
//			fatal("bad/no IV for %s.", cc->cipher->name);
		memcpy(myaes.iv, key, RIJNDAEL_BLOCKSIZE);
	}

	struct rijndael {
		u_char iv[16];
		rijndael_ctx enc;
		rijndael_ctx dec;
	} rijndael;

	struct rijndael myaes;


	void rijndael_decrypt(rijndael_ctx *ctx, u_char *src, u_char *dst);
	void rijndael_encrypt(rijndael_ctx *ctx, u_char *src, u_char *dst);

	private:
	int rijndaelKeySetupEnc(u32 rk[/*4*(Nr + 1)*/], const u8 cipherKey[], int keyBits);
	int rijndaelKeySetupDec(u32 rk[/*4*(Nr + 1)*/], const u8 cipherKey[], int keyBits,int have_encrypt);
	void rijndaelEncrypt(const u32 rk[/*4*(Nr + 1)*/], int Nr, const u8 pt[16], u8 ct[16]);
	void rijndaelDecrypt(const u32 rk[/*4*(Nr + 1)*/], int Nr, const u8 ct[16], u8 pt[16]);
	void rijndael_set_key(rijndael_ctx *ctx, u_char *key, int bits, int encrypt);
};

class cryptoAES128 : public cryptoAES
{
public:
	cryptoAES128() 
	{
		BlockSize = 16;
		KeyLen = 16;
		Name = "aes128-cbc";
	}
};

class cryptoAES256 : public cryptoAES
{
public:
	cryptoAES256() 
	{
		BlockSize = 16;
		KeyLen = 32;
		Name = "aes256-cbc";
	}
};



#endif