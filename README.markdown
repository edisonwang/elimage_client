##Elimage Client

Written by Edison Wang

####Inatall:

Clone the git repo first    

    $ git clone git://github.com/edisonwang/elimage_client.git
    $ cd elimage_client

if you want to define the defaule site to use by yourself     
you can just uncomment # in Makefile like below 
    
    CFLAGS=-lcurl -DELIMAGE_URL=\"img.vim\-cn.com\"

then    

    $ make

####Usage 

    ./elimage [-u <remote url> ] <image file list>
    For default using elimage.edisonnotes.com 
    Example: ./elimage imagefile1 imagefile2 
             ./elimage -u img.vim-cn.com imagefile1 inamefile2

####Requirement

1. libcurl:[http://curl.haxx.se/libcurl/](http://curl.haxx.se/libcurl/)
