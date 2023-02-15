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
#include "config_file.h"
#include <vector>

size_t curlCallback(void *contents, size_t size, size_t nmemb, std::string *s)
{
    size_t newLength = size*nmemb;
    s->append((char*)contents, newLength);
    return newLength;
}

void initAuth() {

    // Variables where we want to store the data from the config file
    std::string aid = "";
    std::string secret = "";
    std::string apikey = "";
    std::string username = "";
    std::string password = "";
    std::string hwid = "";
    std::string license = "";
    std::string email = "";

    bool hasFile;

    // get if user was authenticated before
    std::ifstream file("acc.cfg");
    if(!file.is_open()){
        hasFile = false;
    } else {
        hasFile = true;
    }

    if (hasFile) {
        // Names of the variables in the config file.
        std::vector<std::string> ln = {"aid","secret","apikey", "username", "password", "hwid"};

        // Open the config file for reading
        std::ifstream f_in("acc.cfg");
        CFG::ReadFile(f_in, ln, aid, secret, apikey, username, password, hwid);
        f_in.close();

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
        curl_mime_data(part, hwid.c_str(), CURL_ZERO_TERMINATED);
        part = curl_mime_addpart(multipart);

        /* Set the form info */
        curl_easy_setopt(handle, CURLOPT_MIMEPOST, multipart);

        result = curl_easy_perform(handle); /* post away! */

        /* free the post data again */
        curl_mime_free(multipart);
    } else {

        // Names for the variables in the config file. They can be different from the actual variable names.
        std::vector<std::string> ln = {"aid","secret","apikey", "username", "password", "hwid", "license", "email"};

        // get the parameters from the user somehow here and then write them to the strings here (this should be maybe called at hook.cpp)
        // we need to get the parameters from a login form in ImGui, so gotta figure that out ig, lets assume the values are set
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
        curl_mime_data(part, "register", CURL_ZERO_TERMINATED);
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
        curl_mime_data(part, hwid.c_str(), CURL_ZERO_TERMINATED);
        part = curl_mime_addpart(multipart);
        curl_mime_name(part, "license");
        curl_mime_data(part, license.c_str(), CURL_ZERO_TERMINATED);
        part = curl_mime_addpart(multipart);
        curl_mime_name(part, "email");
        curl_mime_data(part, email.c_str(), CURL_ZERO_TERMINATED);
        part = curl_mime_addpart(multipart);

        /* Set the form info */
        curl_easy_setopt(handle, CURLOPT_MIMEPOST, multipart);
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, curlCallback);

        result = curl_easy_perform(handle); /* post away! */

        /* free the post data again */
        curl_mime_free(multipart);

        // Open the config file for writing
        std::ofstream f_out("acc.cfg");
        CFG::WriteFile(f_out, ln, aid,secret,apikey,username,password,hwid,license,email);
        f_out.close();
    }
}

#endif //ZYGISKPG_AUTH_H
