/*
 * MIT License
 *
 * Copyright (c) 2025 Tom McGuire adapted from openai-c by LunaStev (under MIT License)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// This code wraps some libcurl functions for testing under the J Language
// It also provides a C callback funftion that curl_easy_perform will need to 
// capture the text returned from an http request. 

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

struct memory {
    char* response;
    size_t size;
};

struct memory chunk;

//typedef size_t (*callback_ptr)(void*, size_t, size_t, void*);

extern size_t write_callback(void*, size_t, size_t, void*);

//extern int jcurlperform(CURL* curl);
//extern void set_curl_string_opt(CURL *curl, int opt_flag, char* param_str);
//extern void set_curl_long_opt(CURL *curl, int opt_flag, long lparam);
//extern void set_curl_ptr_opt(CURL *curl, int opt_flag, void* pparam);

CURL *get_curl_ptr(CURL *jcurlptr) {
  CURL *curl1 = curl_easy_init();
  jcurlptr = curl1;
  return curl1;
}

void set_curl_string_opt(void *curl, int opt_flag, char* param_str) {
  CURL *curl1 = (CURL *) curl;
  curl_easy_setopt(curl1, opt_flag, param_str);
}

void set_curl_long_opt(CURL *curl, int opt_flag, long lparam) {
  curl_easy_setopt(curl, opt_flag, lparam);
}

void set_curl_ptr_opt(CURL *curl, int opt_flag, void* pparam) {
  curl_easy_setopt(curl, opt_flag, pparam);
}

int jcurlperform(CURL* curl) {
  CURLcode res;
  FILE *file_ptr; // Declare a FILE pointer

  file_ptr = fopen("jcurlget.log", "w");

    // Check if the file was opened successfully
    if (file_ptr == NULL) {
        perror("Error opening file"); // Print a system-specific error message
        return 100; // Indicate an error
    }

  chunk = (struct memory){malloc(1), 0};

  if(curl) {
    /* Perform the request, res gets the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    fprintf(file_ptr,"curl_easy_perform(curl) completed\n");
    if(res != CURLE_OK) {
      fprintf(file_ptr, "curl_easy_perform() failed: %d, %s\n",res,
              curl_easy_strerror(res));
      curl_easy_cleanup(curl);
      fclose(file_ptr);
      return res;
    }
    /* always cleanup */
    curl_easy_cleanup(curl);
    fprintf(file_ptr,"almost done log response\n");
    fprintf(file_ptr,"%s\n",chunk.response);
    fclose(file_ptr);

    return res;
  }
  fprintf(file_ptr,"Problem setting up curl handle\n");
  fclose(file_ptr);
  return 200;
}
