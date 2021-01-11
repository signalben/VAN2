
#include "node.h"

//file contains functions "owned" by the node, rather than any device

//only used to echo messages so far
void CMD_VAN_NODE (Message inData) {
	if (inData.cmd == ECHO) {
		echoMessage(inData);
	}
}

//swaps source, dest, marks as an echo repsonse, returns to sender
void echoMessage(Message inData) { 
	Message buff = inData;
	buff.src = inData.dest;
	buff.dest = inData.src;
	buff.cmd = RESPONSE;
	handleMessage(buff);
}



   
