extern "C" {
#include <openssl/ssl.h>
#include <openssl/evp.h>
#include <openssl/err.h>
}

#include "Coder.hpp"

#define KEYWORD "DetzoldIstDoof"

namespace Licenser {

using namespace std;

static unsigned char *unbase64(const char *input, int length, size_t *olen) {

	BIO *b64, *bmem;

	char *inbuf = (char *)calloc(length, 1);
	strncpy(inbuf, input, length);

	unsigned char *buffer = (unsigned char *)calloc(length, 1);

	b64 = BIO_new(BIO_f_base64());

	bmem = BIO_new_mem_buf(inbuf, length);
//	BIO_set_mem_eof_return(bmem, 0);
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); 
	bmem = BIO_push(b64, bmem);

	*olen = BIO_read(bmem, buffer, length);

	BIO_free_all(bmem);

	free(inbuf);

	return buffer;
}

static char *base64(const unsigned char *input, int length) {

	BIO *bmem, *b64;
	BUF_MEM *bptr;

	b64 = BIO_new(BIO_f_base64());
	bmem = BIO_new(BIO_s_mem());
	b64 = BIO_push(b64, bmem);
	BIO_write(b64, input, length);
	BIO_flush(b64);
	BIO_get_mem_ptr(b64, &bptr);

	char *buff = (char *)malloc(bptr->length);
	memcpy(buff, bptr->data, bptr->length-1);
	buff[bptr->length-1] = 0;

	BIO_free_all(b64);

	return buff;
}

void Coder::decodeBF(const string &input, string &output) throw (exception) {

	unsigned char *rawbuf;
	size_t         rawbuflen = 0;
	EVP_CIPHER_CTX ctx;
	unsigned char  key[EVP_MAX_KEY_LENGTH];
	unsigned char  iv[EVP_MAX_IV_LENGTH];
	unsigned char *out;
	int            outlen;
	int            tmplen = 0;
	unsigned char  salt[PKCS5_SALT_LEN];

	// Blowfish decoding
	EVP_add_cipher (EVP_bf_cbc());
	EVP_add_cipher_alias (SN_bf_cbc, "bf");

	// decode Base64
	rawbuf = unbase64(input.c_str(), input.length(), &rawbuflen);

	memcpy(salt, rawbuf + 8, sizeof(salt));

	// Salted__ and salt[8] not to be included
	rawbuflen -= 16;

	// turn password into MD5 message digest
	EVP_BytesToKey(EVP_get_cipherbyname("bf"), EVP_md5(),
			salt, (const unsigned char *)KEYWORD,
			strlen(KEYWORD), 1, key, iv);

	EVP_CIPHER_CTX_init(&ctx);

	EVP_DecryptInit_ex(&ctx, EVP_bf_cbc(), 0, key, iv);

	outlen = rawbuflen * 2;
	out = (unsigned char *)calloc(outlen, 1);

	if (!EVP_DecryptUpdate(&ctx, out, &outlen, rawbuf + 16, rawbuflen)) {
		free(out);
		free(rawbuf);
		EVP_CIPHER_CTX_cleanup(&ctx);
		throw runtime_error("EVP_DecryptUpdate gave us an error");
	}

	if(!EVP_DecryptFinal_ex(&ctx, out + outlen, &tmplen)) {
		free(out);
		free(rawbuf);
		EVP_CIPHER_CTX_cleanup(&ctx);
		throw runtime_error("EVP_DecryptFinal_ex threw up");
	}

	EVP_CIPHER_CTX_cleanup(&ctx);

	// that only works if really a \0 teminated string comes out
	output.assign((char *)out);

	free(out);
	free(rawbuf);

}


void Coder::encodeBF(const string &input, string &output) throw (exception) {

	unsigned char *rawbuf;
	int            rawbuflen = 0;
	EVP_CIPHER_CTX ctx;
	unsigned char  key[EVP_MAX_KEY_LENGTH];
	unsigned char  iv[EVP_MAX_IV_LENGTH];
	int            tmplen = 0;
	unsigned char  salt[PKCS5_SALT_LEN];
	int32_t        rd = rand();
	int            inputlength = input.length();

	// Blowfish decoding
	EVP_add_cipher (EVP_bf_cbc());
	EVP_add_cipher_alias (SN_bf_cbc, "bf");

	memcpy(salt, (const void *)&rd, sizeof(salt));

	// turn password into MD5 message digest
	EVP_BytesToKey(EVP_get_cipherbyname("bf"), EVP_md5(),
			salt, (const unsigned char *)KEYWORD,
			strlen(KEYWORD), 1, key, iv);
	
	rawbuflen = inputlength * 3;
	rawbuf = (unsigned char *)calloc(rawbuflen, 1);

	EVP_CIPHER_CTX_init(&ctx);

	EVP_EncryptInit_ex(&ctx, EVP_bf_cbc(), 0, key, iv);

	if (!EVP_EncryptUpdate(&ctx, rawbuf, &rawbuflen, (unsigned char *)input.c_str(), inputlength)) {
		free(rawbuf);
		EVP_CIPHER_CTX_cleanup(&ctx);
		throw runtime_error("EVP_EncryptUpdate threw up");
	}
	/* Buffer passed to EVP_EncryptFinal() must be after data just
	 *           encrypted to avoid overwriting it.  */
	if (!EVP_EncryptFinal_ex(&ctx, rawbuf + rawbuflen, &tmplen)) {
		free(rawbuf);
		EVP_CIPHER_CTX_cleanup(&ctx);
		throw runtime_error("EVP_EncryptFinal_ex gave an error");
	}

	rawbuflen += tmplen;
	EVP_CIPHER_CTX_cleanup(&ctx);

	unsigned char *tmpbuf = (unsigned char *)calloc(tmplen * 2, 1);

	memcpy(tmpbuf, "Salted__", 8);
	memcpy(tmpbuf + 8, salt, sizeof(salt));
	memcpy(tmpbuf + 16, rawbuf, rawbuflen);

	free(rawbuf);

	// encode the answer base64
	char *ret = base64(tmpbuf, rawbuflen + 16);

	output.assign(ret);

	free(ret);
	free(tmpbuf);

}

}



