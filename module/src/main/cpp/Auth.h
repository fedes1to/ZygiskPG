//
// Created by lbert on 2/13/2023.
//

#ifndef ZYGISKPG_AUTH_H
#define ZYGISKPG_AUTH_H

#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <list>

std::string secret = OBFUSCATE("1iiFtJqzRAsSQ5EAoXZ2SHsvEg9VsKJFZo7");
std::string aid = OBFUSCATE("252530");
std::string apikey = OBFUSCATE("3972111712928518569275628818854436378567856538451588");

bool hasAuthenticated;

size_t curlCallback(void *contents, size_t size, size_t nmemb, std::string *s)
{
    size_t newLength = size*nmemb;
    s->append((char*)contents, newLength);
    if (s->find("success") != std::string::npos) {
        hasAuthenticated = true;
    }
    return newLength;
}

#endif //ZYGISKPG_AUTH_H
