#pragma once
#ifndef TCP_CLOSE_H
#define TCP_CLOSE_H
#include "TcpState.h"

class TcpClose : public TcpState {
public:
	virtual void Open(TcpConnection* conn)
	{
		conn->ChangeState(TcpConnection::StateType::Establish);
	}
private:

};

#endif