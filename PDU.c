#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "PDU.h"

#define LTZ -1

void checkDataLen(int lengthOfData, int FLAG){
    if (FLAG == LTZ){
        if (lengthOfData < 0){
            fprintf(stderr, "sendPDU error: negative payload length\n");
            exit(-1);
        }
    }
}

void checkSendInt(int sentBytes, uint8_t *pduBuffer, int FLAG){
    if (FLAG == LTZ){
        if (sentBytes < 0){
            perror("send");
            free(pduBuffer);
            exit(-1);
        }
    }
}

void checkRecvInt(int recvBytes, int FLAG){
    if (FLAG == LTZ){
        if (recvBytes < 0){
            perror("recv");
            exit(-1);
        }
    }
}

void checkPduLen(uint16_t pduLen){
    if (pduLen < 2){
        fprintf(stderr, "recvPDU error: invalid PDU length %u\n", pduLen);
        exit(-1);
    }
}

void checkAlloc(uint8_t *pduBuffer){
    if (pduBuffer == NULL){
        perror("malloc");
        exit(-1);
    }
}

int sendPDU(int clientSocket, uint8_t *dataBuffer, int lengthOfData){
    checkDataLen(lengthOfData, LTZ);

    uint16_t pduLen = (uint16_t)(lengthOfData + 2);
    uint16_t netLen = htons(pduLen);
    uint8_t *pduBuffer = (uint8_t *)malloc(pduLen);

    checkAlloc(pduBuffer);

    memcpy(pduBuffer, &netLen, sizeof(uint16_t));

    if (lengthOfData > 0)
        memcpy(pduBuffer + 2, dataBuffer, lengthOfData);

    int sentBytes = send(clientSocket, pduBuffer, pduLen, 0);

    checkSendInt(sentBytes, pduBuffer, LTZ);

    free(pduBuffer);
    return lengthOfData;
}

int recvPDU(int socketNumber, uint8_t *dataBuffer, int bufferSize){
    uint16_t netLen;
    int recvBytes = recv(socketNumber, &netLen, sizeof(uint16_t), MSG_WAITALL);

    checkRecvInt(recvBytes, LTZ);

    if (recvBytes == 0)
        return 0;

    uint16_t pduLen = ntohs(netLen);
    checkPduLen(pduLen);

    int payloadLength = pduLen - 2;

    if (payloadLength > bufferSize){
        fprintf(stderr, "recvPDU error: payload length > buffer size\n");
        exit(-1);
	}

    recvBytes = recv(socketNumber, dataBuffer, payloadLength, MSG_WAITALL);
    checkRecvInt(recvBytes, LTZ);

    if (recvBytes == 0)
        return 0;

    return recvBytes;
}
