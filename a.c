
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "curl.h"
#include "easy.h"


#define URL_1   122.112.138.114:8001/RTCM33
//GET /RTCM32 HTTP/1.1\r\nUser-Agent: NTRIP CHC/1.0.0\r\nAccept: */*\r\nConnection: Keep-Alive\r\nAuthorization: Basic YmFzZTA1MTE6NDMzNDYwMTc=\r\n

static size_t cnp_tns_parse_data(char *ptr, size_t size, size_t nmemb, void *stream)
{
    int ret = -1;
    int index = 0;
    size_t num = size*nmemb;

    char* buf = malloc(num+1);
    memset(buf, 0x00, num+1);
    memcpy(buf, ptr, num);
	
	if (strcmp(buf, "ICY 200 OK") == 0)
	{
		printf("%s\r\n", buf);
	}
}


static int cnp_tns_curl(CURL *curl)
{
    int ret=0;
	
    CURLcode res;
    struct curl_slist *headers = NULL;


    
    curl_easy_setopt(curl, CURLOPT_URL, URL_1); 
    headers = curl_slist_append(headers, "HTTP/1.1");
	headers = curl_slist_append(headers, "User-Agent: NTRIP CHC/1.0.0");
	headers = curl_slist_append(headers, "Connection: Keep-Alive");
	headers = curl_slist_append(headers, "Authorization: Basic YmFzZTA1MTE6NDMzNDYwMTc=");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER,headers);  
	
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 5000);        
     
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cnp_tns_parse_data);
    //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, tns_reqdata); 
    //curl_easy_setopt(curl, CURLOPT_POST, 1); 
                       
     res = curl_easy_perform(curl);
     if(res != CURLE_OK){
            //CNP_ERR("curl exec failed: %s\n", curl_easy_strerror(res));
            ret = -1; goto error;
     } 
        
    curl_slist_free_all(headers);
	curl_easy_cleanup(curl);
	
    return ret;
}


void main()
{
	CURL *curl = curl_easy_init();
	
	cnp_tns_curl(curl);
	
	while(1){
		sleep(1);
	}
}
