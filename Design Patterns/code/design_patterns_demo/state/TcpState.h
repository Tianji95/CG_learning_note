#pragma once
#ifndef TCP_STATE_H
#define TCP_STATE_H
#include "TcpConnection.h"
class TcpState {
public:
	virtual void Close(TcpConnection* conn) {}
	virtual void Tranmit(TcpConnection* conn) {}
	virtual void Open(TcpConnection* conn) {}
private:
};
#endif // !TCP_STATE_H
