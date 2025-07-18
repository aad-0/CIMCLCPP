#-*- MakeFile -*-
all = main

CC  = gcc

CFLAGS = --std=c99 -g -Wextra -Wall -Werror -Wpedantic

main: main.o modbus_general.o modbus_master.o modbus_slave.o
	@echo "Go get them tiger!"
	$(CC) main.o modbus_general.o modbus_master.o modbus_slave.o -o main

%.o: %.c
	@echo "getting *.o"
	$(CC) -c $(CFLAGS) $< -o $@

main.o: main.c
	@echo "getting main.o"
	$(CC) $(CFLAGS) -c main.c

test:
	.\main
	@echo "ERRCode: $$?" %errorlevel%

gdb:
	@echo "You Lost SOUL !"
	gdb .\main.exe

clean:
	del .\*.o .\main.exe


