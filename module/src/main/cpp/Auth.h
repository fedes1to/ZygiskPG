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

std::string getLine(const std::string& str, int lineNo)
{
    std::string line;
    std::istringstream stream(str);
    while (lineNo-- >= 0)
        std::getline(stream, line);
    return line;
}

bool tryAutoLogin() {
    LOGE("STARTING AUTOLOGIN");
    std::string faggot = Application$persistentDataPath()->getString();
    LOGE("Appending license.key");
    faggot += "/license.key";

    const char* username;
    const char* password;

    LOGE("Trying to enter file: %s", faggot.c_str());
    std::string line;
    std::ifstream file (faggot);
    if (file.is_open())
    {
        LOGW("FILE IS OPEN!");
        for (int lineno = 1; getline (file,line) && lineno < 3; lineno++) {
            if (lineno == 1)
            {
                LOGW("Getting username at: %s", line.c_str());
                username = line.c_str();
            }
            if (lineno == 2) {
                LOGW("Getting password at: %s", line.c_str());
                password = line.c_str();
            }
        }
        file.close();
    }
    const char* hwid = getDeviceUniqueIdentifier()->getChars();
    LOGE("Got HWID at: %s ,starting cURL", hwid);

    CURL *handle;
    CURLcode result;
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // declare handle
    handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, "https://api.auth.gg/v1/");

    // prepare post request
    curl_mime *multipart = curl_mime_init(handle);
    curl_mimepart *part = curl_mime_addpart(multipart);
    curl_mime_name(part, "type");
    curl_mime_data(part, "login", CURL_ZERO_TERMINATED);
    part = curl_mime_addpart(multipart);
    curl_mime_name(part, "aid");
    curl_mime_data(part, aid.c_str(), CURL_ZERO_TERMINATED);
    part = curl_mime_addpart(multipart);
    curl_mime_name(part, "apikey");
    curl_mime_data(part, apikey.c_str(), CURL_ZERO_TERMINATED);
    part = curl_mime_addpart(multipart);
    curl_mime_name(part, "secret");
    curl_mime_data(part, secret.c_str(), CURL_ZERO_TERMINATED);
    part = curl_mime_addpart(multipart);
    curl_mime_name(part, "username");
    curl_mime_data(part, username, CURL_ZERO_TERMINATED);
    part = curl_mime_addpart(multipart);
    curl_mime_name(part, "password");
    curl_mime_data(part, password, CURL_ZERO_TERMINATED);
    part = curl_mime_addpart(multipart);
    curl_mime_name(part, "hwid");
    curl_mime_data(part, hwid, CURL_ZERO_TERMINATED);

    /* Set the form info */
    curl_easy_setopt(handle, CURLOPT_MIMEPOST, multipart);

    result = curl_easy_perform(handle); /* post away! */

    /* free the post data again */
    curl_mime_free(multipart);

    sleep(1);

    // need to do code to return whether login was successful or not
    return hasAuthenticated;
}

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
