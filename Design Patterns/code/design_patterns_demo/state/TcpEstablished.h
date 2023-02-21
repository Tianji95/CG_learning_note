#pragma once
#ifndef TCP_ESTABLISHED_H
#define TCP_ESTABLISHED_H
#include "TcpState.h"
class TcpEstablished : public TcpState {
public:
	virtual void Tranmit(TcpConnection* conn) {
		conn->ChangeState(TcpConnection::StateType::Listen);
	}
private:
};
#endif // !TCP_ESTABLISHED_H
