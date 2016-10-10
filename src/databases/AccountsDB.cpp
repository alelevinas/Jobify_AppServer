//
// Created by ale on 9/10/16.
//

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include "AccountsDB.h"

AccountsDB::AccountsDB(const std::string &db_name) : DB(db_name) {}

AccountsDB::~AccountsDB() {

}

bool AccountsDB::add_account(std::string username, std::string password) {
    using namespace CryptoPP;
    CryptoPP::Weak::MD5 hash;
    std::string md5digest;

    StringSource s(password, true, new HashFilter(hash, new HexEncoder(new StringSink(md5digest))));

    return this->add(username,md5digest);
}
