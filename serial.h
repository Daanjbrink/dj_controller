#ifndef SERIAL_H_
#define SERIAL_H_

#define BAUDRATE ((F_CPU / (115200UL * 16)) - 1)

void initSerial();
void SerialWrite(char *c, short len);
void SerialWriteByte(char c);

#endif