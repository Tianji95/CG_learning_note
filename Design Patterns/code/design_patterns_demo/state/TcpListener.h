#pragma once
#ifndef TCP_LISTENER_H
#define TCP_LISTENER_H
#include "TcpState.h"

class TcpListener : public TcpState {
public:
	virtual void Close(TcpConnection* conn) {
		conn->ChangeState(TcpConnection::StateType::Close);
	}
private:

};
#endif // !TCP_LISTENER_H
