#pragma once
#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H
#include <string>
class TcpState;
class TcpClose;
class TcpEstablished;
class TcpListener;
class TcpConnection {
public:
	enum class StateType {
		Close = 1,
		Establish = 2,
		Listen = 3
	};
	TcpConnection();
	virtual ~TcpConnection() {}
	virtual void Close();
	virtual void Open();
	virtual void Tranmit();
	virtual void ChangeState(StateType type);
private:
	TcpState* _state;

	TcpClose* _closeState;
	TcpEstablished* _EstablishState;
	TcpListener* _listenState;

};
#endif // !TCP_CONNECTION_H
