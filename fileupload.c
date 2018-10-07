
#include <stdio.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
  CURL *curl;
  CURLcode res;
  struct stat file_info;
  curl_off_t speed_upload, total_time;
  FILE *fd;

  fd = fopen("/home/shilesh/test/firmware.txt", "rb"); /* open file to upload */
  if(!fd){
	printf("fail to open file \n");
    	return 1; /* can't continue */
	}
  /* to get the file size */
  if(fstat(fileno(fd), &file_info) != 0){
	printf("File size \n");
    return 1; /* can't continue */
	}


/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if(curl) {

    /* upload to this place */
    //curl_easy_setopt(curl, CURLOPT_URL, "file:///home/dast/src/curl/debug/new");
    /*
     * You better replace the URL with one that works!
     */
    /* tell it to "upload" to the URL */
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    /* set where to read from (on Windows you need to use READFUNCTION too) */
    curl_easy_setopt(curl, CURLOPT_READDATA, fd);



    /* and give the size of the upload (optional) */
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                     (curl_off_t)file_info.st_size);

    /* enable verbose for easier tracing */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }
    else {
	printf("FTP upload file successfully\n");
	/* now extract transfer info */
      //curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD_T, &speed_upload);
      //curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME_T, &total_time);

      //fprintf(stderr, "Speed: %" CURL_FORMAT_CURL_OFF_T " bytes/sec during %" CURL_FORMAT_CURL_OFF_T ".%06ld seconds\n", speed_upload, (total_time / 1000000), (long)(total_time %1000000));
    }
    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  fclose(fd);
  return 0;
}
