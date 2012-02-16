CC=gcc
# using libcurl uncomment # in below to use img.vim-cn.com for default
CFLAGS=-lcurl -DELIMAGE_URL=\"img.vim\-cn.com\"

all: elimage

elimage: 
	$(CC) $(CFLAGS) elimage.c -o elimage

clean:
	rm -rf  elimage
