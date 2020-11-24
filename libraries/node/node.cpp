
#include "node.h"

void CMD_VAN_NODE (Message inData) {
	if (inData.cmd == ECHO) {
		echoMessage(inData);
	}
}

void echoMessage(Message inData) { 
	Message buff = inData;
	buff.src = inData.dest;
	buff.dest = inData.src;
	buff.cmd = RESPONSE;
	handleMessage(buff);
}



   
