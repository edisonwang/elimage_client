/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2011, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
/* 
 * Modified by Edison Wang for elimage project 
 * Used to upload images to elimage website to share image
 * 
 * Email:jawwang1129@gmail.com
 * Last modified 02-15-2012
 */

#include <stdio.h>
#include <string.h>

#include <curl/curl.h>
#define ELIMAGE_URL "http://elimage.edisonnotes.com"

char *elimage_url = ELIMAGE_URL;

void usage(char *argv)
{
    printf("Usage :\n");
    printf("%s <imagename> [-u] <remote url> \n", argv);
    printf("For default using elimage.edisonnotes.com \n");
    printf("Example: %s imagefile \n", argv);
    printf("         %s imagefile -u img.vim-cn.com\n", argv);

}
int find_url(int argc , char *argv[])
{
    int i;
    for (i=1 ;i<argc; i++){
        if (strcmp ( argv[i], "-u" ) == 0)
            return i+1;
    }
    return -1;
}
int check_file(char* file)
{
     return access( file , 0 );
}
int main(int argc, char *argv[])
{
    int url_pointer;
    int file_pointer = 1;

    CURL *curl;
    CURLcode res;

    struct curl_httppost *formpost=NULL;
    struct curl_httppost *lastptr=NULL;
    struct curl_slist *headerlist=NULL;
    static const char buf[] = "Expect:";

    switch(argc){
        case 1:
            usage(argv[0]);
            return 1;
            break;
        case 2:
            if( (check_file( argv[file_pointer] )) == -1 )
            {
                printf ("File %s not found \n", argv[file_pointer]);
                usage(argv[0]);
                return 1;
            }
            break;
        case 4:
            url_pointer = find_url(argc,argv);
            if (url_pointer == -1 || argv[url_pointer]  == NULL)
            {
                usage(argv[0]);
                return -1;
            }
            elimage_url = argv[url_pointer];
            if (url_pointer == 2)
            {
                file_pointer = 3;
            }
            if( (check_file( argv[file_pointer] )) == -1 )
            {  
                printf ("File %s not found \n", argv[file_pointer]);
                usage(argv[0]);
                return 1;
            }
            break;
        default:
            usage(argv[0]);
            return 1;
    }

    curl_global_init(CURL_GLOBAL_ALL);

    /* Fill in the file upload field */
    curl_formadd(&formpost,
            &lastptr,
            CURLFORM_COPYNAME, "sendfile",
            CURLFORM_FILE, argv[file_pointer],
            CURLFORM_END);

    curl = curl_easy_init();
    /* initalize custom header list (stating that Expect: 100-continue is not
       wanted */
    headerlist = curl_slist_append(headerlist, buf);
    if(curl) {
        /* what URL that receives this POST */
        curl_easy_setopt(curl, CURLOPT_URL, elimage_url);
        if ( (argc == 2) && (!strcmp(argv[1], "noexpectheader")) )
            /* only disable 100-continue header if explicitly requested */
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
        res = curl_easy_perform(curl);

        /* always cleanup */
        curl_easy_cleanup(curl);

        /* then cleanup the formpost chain */
        curl_formfree(formpost);
        /* free slist */
        curl_slist_free_all (headerlist);
    }
    return 0;
}
