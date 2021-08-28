
CC=mingw32-gcc

lvdll.o: lvdll.c lvdll.h
	$(CC) -c -DBUILD_DLL lvdll.c

lvdll.dll: lvdll.o
	$(CC) -shared -o lvdll.dll lvdll.o -Wl,--out-implib,lvdll.a

clean:
	rm -f lvdll.o lvdll.a lvdll.dll
