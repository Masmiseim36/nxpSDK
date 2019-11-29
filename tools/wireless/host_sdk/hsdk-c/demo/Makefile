CC=gcc
CFLAGS=-std=c99 -fshort-enums -Wall -Wfatal-errors -pthread
LDFLAGS=-lframer -lfsci -lphysical -luart -lsys -pthread

HSDK_PATH=../../hsdk
SYS_INC  =-I$(HSDK_PATH)/include/sys
PHY_INC  =-I$(HSDK_PATH)/include/physical
UART_INC =-I$(HSDK_PATH)/include/physical/UART
PROTO_INC=-I$(HSDK_PATH)/include/protocol
FSCI_INC =-I$(HSDK_PATH)/include/protocol/FSCI

INC=-I../inc/ $(SYS_INC) $(PHY_INC) $(PROTO_INC) $(UART_INC) $(FSCI_INC)
CFLAGS+=$(INC)

all: clean HeartRateSensor ThreadLeader ZigBeeCoordinator

HeartRateSensor.o: HeartRateSensor.c
	$(CC) -c -o $@ $< $(CFLAGS)

cmd_ble_1.2.8.o: ../src/cmd_ble_1.2.8.c
	$(CC) -c -o $@ $< $(CFLAGS)

evt_ble_1.2.8.o: ../src/evt_ble_1.2.8.c
	$(CC) -c -o $@ $< $(CFLAGS)

evt_printer_ble_1.2.8.o: ../src/evt_printer_ble_1.2.8.c
	$(CC) -c -o $@ $< $(CFLAGS)

HeartRateSensor: HeartRateSensor.o cmd_ble_1.2.8.o evt_ble_1.2.8.o evt_printer_ble_1.2.8.o
	gcc -o $@ $^ $(CFLAGS) $(LDFLAGS)

ThreadLeader.o: ThreadLeader.c
	$(CC) -c -o $@ $< $(CFLAGS)

cmd_threadip_1.2.8.o: ../src/cmd_threadip_1.2.8.c
	$(CC) -c -o $@ $< $(CFLAGS)

evt_threadip_1.2.8.o: ../src/evt_threadip_1.2.8.c
	$(CC) -c -o $@ $< $(CFLAGS)

evt_printer_threadip_1.2.8.o: ../src/evt_printer_threadip_1.2.8.c
	$(CC) -c -o $@ $< $(CFLAGS)

ThreadLeader: ThreadLeader.o cmd_threadip_1.2.8.o evt_threadip_1.2.8.o evt_printer_threadip_1.2.8.o
	gcc -o $@ $^ $(CFLAGS) $(LDFLAGS)

ZigBeeCoordinator.o: ZigBeeCoordinator.c
	$(CC) -c -o $@ $< $(CFLAGS)

cmd_zigbee3_0_6.0.9.o: ../src/cmd_zigbee3_0_6.0.9.c
	$(CC) -c -o $@ $< $(CFLAGS)

evt_zigbee3_0_6.0.9.o: ../src/evt_zigbee3_0_6.0.9.c
	$(CC) -c -o $@ $< $(CFLAGS)

evt_printer_zigbee3_0_6.0.9.o: ../src/evt_printer_zigbee3_0_6.0.9.c
	$(CC) -c -o $@ $< $(CFLAGS)

ZigBeeCoordinator: ZigBeeCoordinator.o cmd_zigbee3_0_6.0.9.o evt_zigbee3_0_6.0.9.o evt_printer_zigbee3_0_6.0.9.o
	gcc -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o HeartRateSensor ThreadLeader ZigBeeCoordinator
