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

std::string secret = "1iiFtJqzRAsSQ5EAoXZ2SHsvEg9VsKJFZo7";
std::string aid = "252530";
std::string apikey = "3972111712928518569275628818854436378567856538451588";

bool hasAuthenticated = false;

size_t curlCallback(void *contents, size_t size, size_t nmemb, std::string *s)
{
    size_t newLength = size*nmemb;
    s->append((char*)contents, newLength);
    if (s->find("success") != std::string::npos) {
        hasAuthenticated = true;
    }
    return newLength;
}

std::string getHwid() {
    CURL *curl;
    CURLcode result;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    char* ip;

    curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");

    /* Perform the request, res will get the return code */
    auto res = curl_easy_perform(curl);
    /* Check for errors */
    if((res == CURLE_OK) &&
       !curl_easy_getinfo(curl, CURLINFO_LOCAL_IP, &ip) && ip) {
        return ip;
    }

    /* always cleanup */
    curl_easy_cleanup(curl);
}

bool tryAutoLogin() {
    std::string username = "";
    std::string password = "";

    // Names of the variables in the config file.
    std::vector<std::string> ln = {"username", "password"};

    // Open the config file for reading
    std::ifstream f_in("acc.cfg");
    CFG::ReadFile(f_in, ln,username, password);
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
    curl_mime_data(part, getHwid().c_str(), CURL_ZERO_TERMINATED);
    part = curl_mime_addpart(multipart);

    /* Set the form info */
    curl_easy_setopt(handle, CURLOPT_MIMEPOST, multipart);

    result = curl_easy_perform(handle); /* post away! */

    /* free the post data again */
    curl_mime_free(multipart);

    // need to do code to return whether login was successful or not
    return hasAuthenticated;
}

bool tryLogin(std::string username, std::string password) {

    // Names for the variables in the config file. They can be different from the actual variable names.
    std::vector<std::string> ln = {"username", "password"};

    // Open the config file for writing
    std::ofstream f_out("acc.cfg");
    CFG::WriteFile(f_out, ln,username,password);
    f_out.close();
    return tryAutoLogin();
}

bool tryRegister(std::string username, std::string password, std::string license, std::string email) {

    // Names for the variables in the config file. They can be different from the actual variable names.
    std::vector<std::string> ln = {"username", "password", "license", "email"};

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
    curl_mime_data(part, getHwid().c_str(), CURL_ZERO_TERMINATED);
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
    CFG::WriteFile(f_out, ln, username,password,license,email);
    f_out.close();

    // need to do code to return whether register was successful or not
    return hasAuthenticated;
}

#endif //ZYGISKPG_AUTH_H
