set up:

sudo apt-get install libcjson1 libcjson-dev ibmosquitto-dev libmosquittopp-dev libssl-dev xsltproc 

sudo mv libmosquitto.so.1 /usr/local/lib/
sudo mv libmosquittopp.so.1 /usr/local/lib/

@create: Makefile.mk

@include: add LDLIBS += -lmosquittopp    \
