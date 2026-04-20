#ifndef PDU_H
#define PDU_H

#include <stdint.h>

void checkDataLen(int lengthOfData, int FLAG);
void checkSendInt(int sentBytes, uint8_t *pduBuffer, int FLAG);
void checkRecvInt(int recvBytes, int FLAG);
void checkPduLen(uint16_t pduLen);
void checkAlloc(uint8_t *pduBuffer);

int sendPDU(int clientSocket, uint8_t *dataBuffer, int lengthOfData);
int recvPDU(int socketNumber, uint8_t *dataBuffer, int bufferSize);

#endif


