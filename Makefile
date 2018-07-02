
CFLAGS = -std=c99 -Wall
TEST_CFLAGS = -Ideps
CUSTOM_MALLOC_FLAGS = -Db64_malloc=custom_malloc -Db64_realloc=custom_realloc

ifeq ($(USE_CUSTOM_MALLOC), true)
CFLAGS += $(CUSTOM_MALLOC_FLAGS)
endif

default: b64_ebus
#	./$< -e

b64_ebus: CFLAGS+=$(TEST_CFLAGS)
b64_ebus: encode_ebus.o encode.o decode.o
	cc -o b64_ebus  encode_ebus.o encode.o decode.o

clean:
	rm -f *.o b64_ebus 

.PHONY: default clean
