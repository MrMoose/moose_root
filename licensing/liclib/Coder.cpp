#include "Coder.hpp"

#include <tools/Random.hpp>

// boooost
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/format.hpp>
#include <boost/asio.hpp>

#include <iostream>
#include <stdexcept>
#include <vector>

extern "C" {
#include <openssl/evp.h>
}

namespace moose {
namespace liclib {

namespace asio = boost::asio;
using asio::ip::tcp;


// throw std::exception
// throw asio error
bool verifyLicense(const std::string &n_ip, const std::string &n_license, int &n_max_queries, std::string &n_yahoo_id, std::string &n_customer_name, std::string &n_expiry_date) {

	asio::ip::tcp::endpoint ep(asio::ip::address::from_string(n_ip), 42023);

	std::vector<std::string> tokens;
	std::string request("getlicense ");
	std::string challenge;
	std::string response;
	std::string encoded_response;
	std::string encoded_answer;
	std::string answer;

	// create socket connection
	asio::io_service io_service;
	tcp::socket sock(io_service);

	sock.connect(ep);
	// MOEP handle errors


	request.append(n_license);
	request.append("\n");

	// write "getlicense <id>"
	asio::write(sock, asio::buffer(request, request.length()));

	// get challenge
	asio::streambuf sbuf;
	asio::read_until(sock, sbuf, '\n');
	std::istream is(&sbuf);
	std::getline(is, challenge);

	// look for errors
	if (!challenge.compare(0,3,"ERR")) {
		// analyse answer
		boost::algorithm::split(tokens, challenge, boost::algorithm::is_space());

		if (tokens.size() > 1) {
			if (!tokens[1].compare(0, 2, "-1")) {
				// license has expired
				sock.close();
				return false;
			}
			else if (!tokens[1].compare(0, 2, "-2")) {
				// license unknown
				sock.close();
				return false;
			} else {
				throw std::runtime_error("got malformed error challenge");
			}
		} else
			throw std::runtime_error("got malformed error challenge");
	}

	// construct response
	boost::format fmt("%s:%s:%lli");
	fmt % challenge % n_license % tools::urand();
	response = fmt.str();
	encodeBF(response, encoded_response);
	encoded_response.append("\n");

	// write response
	asio::write(sock, asio::buffer(encoded_response, encoded_response.length()));

	// get answer
	asio::read_until(sock, sbuf, '\n');
	std::getline(is, encoded_answer);

	decodeBF(encoded_answer, answer);

	// analyse answer
	boost::algorithm::split(tokens, answer, boost::algorithm::is_any_of(":"));

	if (tokens.size() > 5) {
		n_yahoo_id = tokens[2];
		n_max_queries = atoi(tokens[3].c_str());
		n_customer_name = tokens[4];
		n_expiry_date =  tokens[5];
		if (!tokens[1].compare(0, 2, "-)")) {
			return true;
		} else {
			return false;
		}

	} else if (tokens.size() > 1) {
		if (!tokens[1].compare(0, 2, "-)")) {
			return true;
		} else {
			return false;
		}
	} else
		throw std::runtime_error("got malformed answer from license server");

	sock.close();

	return false;
}

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

	char *ptr = 0;
	long  b64len;

	BIO *bmem = BIO_new(BIO_s_mem());
	BIO *b64  = BIO_new(BIO_f_base64());
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

	bmem = BIO_push(b64, bmem);
	BIO_write(bmem, input, length);
	BIO_flush(bmem);
	b64len = BIO_get_mem_data(bmem, &ptr);

	char *buff = (char *)malloc(b64len + 1);
	memcpy(buff, ptr, b64len);
	buff[b64len] = 0;

	BIO_free_all(bmem);

	return buff;
}

// throw std::exception
void decodeBF(const std::string &n_input, std::string &n_output) {

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
	rawbuf = unbase64(n_input.c_str(), n_input.length(), &rawbuflen);

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
		throw std::runtime_error("EVP_DecryptUpdate gave us an error");
	}

	if(!EVP_DecryptFinal_ex(&ctx, out + outlen, &tmplen)) {
		free(out);
		free(rawbuf);
		EVP_CIPHER_CTX_cleanup(&ctx);
		throw std::runtime_error("EVP_DecryptFinal_ex threw up");
	}

	EVP_CIPHER_CTX_cleanup(&ctx);

	// that only works if really a \0 teminated string comes out
	n_output.assign((char *)out);

	free(out);
	free(rawbuf);

}


void encodeBF(const std::string &n_input, std::string &n_output) {

	unsigned char *rawbuf;
	int            rawbuflen = 0;
	EVP_CIPHER_CTX ctx;
	unsigned char  key[EVP_MAX_KEY_LENGTH];
	unsigned char  iv[EVP_MAX_IV_LENGTH];
	int            tmplen = 0;
	unsigned char  salt[PKCS5_SALT_LEN];
	int32_t        rd = rand();   // MOEP replace by moose rand
	int            inputlength = n_input.length();

	// Blowfish decoding
	EVP_add_cipher (EVP_bf_cbc());
	EVP_add_cipher_alias (SN_bf_cbc, "bf");

	memcpy(salt, (const void *)&rd, sizeof(salt));

	// turn password into MD5 message digest
	EVP_BytesToKey(EVP_get_cipherbyname("bf"), EVP_md5(),
			salt, (const unsigned char *)KEYWORD,
			strlen(KEYWORD), 1, key, iv);

	rawbuf = (unsigned char *)calloc(inputlength * 3, 1);

	EVP_CIPHER_CTX_init(&ctx);

	EVP_EncryptInit_ex(&ctx, EVP_bf_cbc(), 0, key, iv);

	if (!EVP_EncryptUpdate(&ctx, rawbuf, &rawbuflen, (unsigned char *)n_input.c_str(), inputlength)) {
		free(rawbuf);
		EVP_CIPHER_CTX_cleanup(&ctx);
		throw std::runtime_error("EVP_EncryptUpdate threw up");
	}
	/* Buffer passed to EVP_EncryptFinal() must be after data just
	 *           encrypted to avoid overwriting it.  */
	if (!EVP_EncryptFinal_ex(&ctx, rawbuf + rawbuflen, &tmplen)) {
		free(rawbuf);
		EVP_CIPHER_CTX_cleanup(&ctx);
		throw std::runtime_error("EVP_EncryptFinal_ex gave an error");
	}

	rawbuflen += tmplen;
	EVP_CIPHER_CTX_cleanup(&ctx);

	unsigned char *tmpbuf = (unsigned char *)calloc(rawbuflen * 2 + 16, 1);

	memcpy(tmpbuf, "Salted__", 8);
	memcpy(tmpbuf + 8, salt, sizeof(salt));
	memcpy(tmpbuf + 16, rawbuf, rawbuflen);

	free(rawbuf);

	// encode the answer base64
	char *ret = base64(tmpbuf, rawbuflen + 16);

	n_output.assign(ret);

	free(ret);
	free(tmpbuf);

}

} // ns liclib
} // ns moose

