
//first modify


/* This is an example application source code using the multi interface 
  * to do a multipart formpost without "blocking". */  
 #include <stdio.h>  
 #include <string.h>  
 #include <sys/time.h>  
 #include <curl/curl.h>  
 #include "curl.h"
 #define FILE_PATH "/home/farsight/ceshi/2016.7.22/2016.7.22/pbx/1.c"
 #define NAME "file"
 
 #define URL "http://192.168.1.94/cgi-bin/fxq.cgi"
#define FILENAME   "curlposttest.log"

size_t write_data(void* buffer,size_t size,size_t nmemb,void *stream)
{
	FILE *fptr = (FILE*)stream;
	fwrite(buffer,size,nmemb,fptr);
	return size*nmemb;
}

char  *curl_test()  
 {  
   CURL *curl;  
   
   CURLM *multi_handle;  
   int still_running;  
     FILE *fptr;  
	 
	   
    if ((fptr = fopen(FILENAME, "w")) == NULL) {  
        fprintf(stderr, "fopen file error: %s\n", FILENAME);  
        return NULL;  
    }  
   
   struct curl_httppost *formpost=NULL;  
   struct curl_httppost *lastptr=NULL;  
   struct curl_slist *headerlist=NULL;  
   static const char buf[] = "Expect:";  
   
   /* Fill in the filename field */  
   curl_formadd(&formpost,  
                &lastptr,  
                CURLFORM_COPYNAME, NAME,  
                CURLFORM_FILE, FILE_PATH,  
                CURLFORM_END);  
   curl = curl_easy_init();  
   multi_handle = curl_multi_init();  
   
   /*initalize custom header list (stating that Expect: 100-continue is not 
      wanted */  
  headerlist = curl_slist_append(headerlist, buf);  
   if(curl && multi_handle) {  
   
     /* what URL that receives this POST */  
     curl_easy_setopt(curl, CURLOPT_URL, URL);  
     curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);  
   
     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);  
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);  
    curl_easy_setopt(curl, CURLOPT_POST, 1); 
   
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);  
     curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);  
   
     curl_multi_add_handle(multi_handle, curl);  
   
     curl_multi_perform(multi_handle, &still_running);  
   
    do {  
      struct timeval timeout;  
      int rc; /* select() return code */  
   
      fd_set fdread;  
      fd_set fdwrite;  
       fd_set fdexcep;  
      int maxfd = -1;  
   
      long curl_timeo = -1;  
   
       FD_ZERO(&fdread);  
       FD_ZERO(&fdwrite);  
       FD_ZERO(&fdexcep);  
   
       /* set a suitable timeout to play around with */  
       timeout.tv_sec = 3;  
       timeout.tv_usec = 0;  
   
      curl_multi_timeout(multi_handle, &curl_timeo);  
      if(curl_timeo >= 0) {  
       timeout.tv_sec = curl_timeo / 1000;  
        if(timeout.tv_sec > 1)  
           timeout.tv_sec = 1;  
        else  
          timeout.tv_usec = (curl_timeo % 1000) * 1000;  
       }  
   
       /* get file descriptors from the transfers */  
       curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);  
   
       /* In a real-world program you OF COURSE check the return code of the 
          function calls.  On success, the value of maxfd is guaranteed to be 
          greater or equal than -1.  We call select(maxfd + 1, ...), specially in 
          case of (maxfd == -1), we call select(0, ...), which is basically equal 
          to sleep. */  
   
      rc = select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout);  
   
      switch(rc) {  
       case -1:  
         /* select error */  
         break;  
       case 0:  
       default:  
         /* timeout or readable/writable sockets */  
         //printf("perform!\n");  
         curl_multi_perform(multi_handle, &still_running);  
        // printf("running: %d!\n", still_running);  
        break;  
       }  
   } while(still_running); 
   
     curl_multi_cleanup(multi_handle);  
   
     /* always cleanup */  
     curl_easy_cleanup(curl);  
   
     /* then cleanup the formpost chain */  
     curl_formfree(formpost);  
   
     /* free slist */  
   curl_slist_free_all (headerlist);  
   }  
   return NULL;
 }  


