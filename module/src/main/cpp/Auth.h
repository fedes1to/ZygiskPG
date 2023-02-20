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

std::string secret = "1iiFtJqzRAsSQ5EAoXZ2SHsvEg9VsKJFZo7";
std::string aid = "252530";
std::string apikey = "3972111712928518569275628818854436378567856538451588";

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

bool tryAutoLogin() {
    std::string array[2];
    short loop = 0;
    std::string line;
    std::ifstream myfile ("/sdcard/license.key");
    if (myfile.is_open())
    {
        while (! myfile.eof() )
        {
            getline (myfile,line);
            array[loop] = line;
            loop++;
        }
        myfile.close();
    }

    std::string username = array[0];
    std::string password = array[1];

    LOGW("got username at %s", username.c_str());
    LOGW("got password at %s", password.c_str());

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
    curl_mime_data(part, username.c_str(), CURL_ZERO_TERMINATED);
    part = curl_mime_addpart(multipart);
    curl_mime_name(part, "password");
    curl_mime_data(part, password.c_str(), CURL_ZERO_TERMINATED);
    part = curl_mime_addpart(multipart);
    curl_mime_name(part, "hwid");
    curl_mime_data(part, "no_hwid_set", CURL_ZERO_TERMINATED);

    /* Set the form info */
    curl_easy_setopt(handle, CURLOPT_MIMEPOST, multipart);

    result = curl_easy_perform(handle); /* post away! */

    /* free the post data again */
    curl_mime_free(multipart);

    sleep(1);

    // need to do code to return whether login was successful or not
    return hasAuthenticated;
}

#endif //ZYGISKPG_AUTH_H
