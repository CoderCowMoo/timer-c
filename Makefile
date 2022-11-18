.PHONY: all clean

CC = x86_64-w64-mingw32-gcc
WARNS = -Wall -Wextra
OPTIMS = -Os -flto

all: timer.exe

timer.exe: timer.c
	$(CC) $^ -mconsole $(WARNS) $(OPTIMS) -o $@

clean:
	rm timer.exe

release: timer.c
	$(CC) $^ $(OPTIMS) -s -o timer.exe
