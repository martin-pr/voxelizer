#include "common.h"

#include <openssl/md5.h>

#include "config.h"

namespace {
	char toHex(unsigned char c) {
		if(c < 10)
			return '0' + c;
		return 'A' + c - 10;
	}
}

const boost::filesystem::path cache_path(const boost::filesystem::path& source) {
	const std::string str = source.string();

	unsigned char* hash = MD5((const unsigned char*)str.c_str(), str.length(), NULL);

	std::string filename;
	for(unsigned a=0;a<MD5_DIGEST_LENGTH;++a) {
		filename += (toHex(hash[a] >> 4));
		filename += (toHex(hash[a] & 0x0F));
	}

	return boost::filesystem::path(
		config::get<std::string>("main.cache") + "/" + filename);
}
