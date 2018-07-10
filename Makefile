
CFLAGS = -std=c99 -Wall
TEST_CFLAGS = -Ideps
CUSTOM_MALLOC_FLAGS = -Db64_malloc=custom_malloc -Db64_realloc=custom_realloc

ifeq ($(USE_CUSTOM_MALLOC), true)
CFLAGS += $(CUSTOM_MALLOC_FLAGS)
endif

SRC= encode_ebus.c encode.c decode.c
OBJ= $(SRC:.c=.o)
OBJ_WINDOWS= $(SRC:.c=.obj)

default: all

all : linux windows

windows: b64_ebus.exe

linux: b64_ebus

b64_ebus: CFLAGS+=$(TEST_CFLAGS)
b64_ebus: $(OBJ)
	cc -o b64_ebus $(OBJ)

b64_ebus.exe : $(OBJ_WINDOWS)
	x86_64-w64-mingw32-gcc -o b64_ebus.exe $(OBJ_WINDOWS)
	
%.obj: %.c
	x86_64-w64-mingw32-gcc -o $@ -c $<

clean: clean_linux clean_windows

clean_linux:
	rm -f *.o b64_ebus

clean_windows:
	rm -f *.obj b64_ebus.exe
	
.PHONY: default clean windows all
