CC=gcc
# using libcurl
CFLAGS=-lcurl 

all: elimage

elimage: 
	$(CC) $(CFLAGS) elimage.c -o elimage

clean:
	rm -rf  elimage
