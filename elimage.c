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
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#include <curl/curl.h>

#ifndef ELIMAGE_URL
    #define ELIMAGE_URL "http://elimage.edisonnotes.com"
#endif

char *elimage_url = ELIMAGE_URL;

void usage(char *argv)
{
    printf("Usage :\n");
    printf("%s [-u <remote url> ] <image file list>\n", argv);
    printf("For default using elimage.edisonnotes.com \n");
    printf("Example: %s imagefile1 imagefile2 \n", argv);
    printf("         %s -u img.vim-cn.com imagefile1 inamefile2\n", argv);

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
int self_check_opt(int argc, char *argv[]){
    int c;
    if (argc == 1)
    {
        usage(argv[0]);
        return 1;
    }
    opterr = 0;

    while ((c = getopt (argc, argv, "hu:")) != -1)
        switch (c)
        {   
            case 'h':
                usage(argv[0]);
                return 1;
            case 'u':
                elimage_url = optarg;
                break;
            case '?':
                if (optopt == 'u')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
                
                return 1;
            default:
                abort ();
        }

    printf ("Using site : %s\n", elimage_url);

    return 0;
}
int main(int argc, char *argv[])
{

    int index = 0;
    int file_counter= 0;

    CURL *curl;
    CURLcode res;

    struct curl_httppost *formpost=NULL;
    struct curl_httppost *lastptr=NULL;
    struct curl_slist *headerlist=NULL;
    static const char buf[] = "Expect:";

    if( self_check_opt(argc, argv) != 0 )
    {
        return 1;
    }

    curl_global_init(CURL_GLOBAL_ALL);

    for (index = optind; index < argc; index++){
        if(check_file(argv[index]))
        {
            fprintf (stderr, "File not found:  %s\n", argv[index]);
            usage(argv[0]);
            return 1;
        } else {
            /* Fill in the file upload field */
            curl_formadd(&formpost,
                    &lastptr,
                    CURLFORM_COPYNAME, "sendfile",
                    CURLFORM_FILE, argv[index],
                    CURLFORM_END);
        }
        file_counter++;
    }
    if (file_counter == 0)
    {
        usage(argv[0]);
        return 1;
    }

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
