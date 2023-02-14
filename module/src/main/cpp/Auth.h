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

void initAuth() {
    bool hasFile;
    static char* aid, secret, apikey;

    // get if user was authenticated before
    std::ifstream file("myfile.txt");
    if(!file.is_open()){
        hasFile = false;
    } else {
        hasFile = true;
    }

    if (hasFile) {
        // Variables where we want to store the data from the config file
        std::string aid = "";
        std::string secret = "";
        std::string apikey = "";

        // Names of the variables in the config file.
        std::vector<std::string> ln = {"aid","secret","apikey"};

        // Open the config file for reading
        std::ifstream f_in("acc.cfg");
        CFG::ReadFile(f_in, ln, aid, secret, apikey);
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
        curl_mime_data(part, "info", CURL_ZERO_TERMINATED);
        part = curl_mime_addpart(multipart);
        curl_mime_name(part, "aid");
        curl_mime_data(part, aid.c_str(), CURL_ZERO_TERMINATED);
        part = curl_mime_addpart(multipart);
        curl_mime_name(part, "secret");
        curl_mime_data(part, secret.c_str(), CURL_ZERO_TERMINATED);
        part = curl_mime_addpart(multipart);
        curl_mime_name(part, "apikey");
        curl_mime_data(part, apikey.c_str(), CURL_ZERO_TERMINATED);
        part = curl_mime_addpart(multipart);

        /* Set the form info */
        curl_easy_setopt(handle, CURLOPT_MIMEPOST, multipart);

        result = curl_easy_perform(handle); /* post away! */

        /* free the post data again */
        curl_mime_free(multipart);

    }
}

#endif //ZYGISKPG_AUTH_H
