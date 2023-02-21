#include "TcpState.h"
#include "TcpClose.h"
#include "TcpEstablished.h"
#include "TcpListener.h"
#include "TcpConnection.h"

TcpConnection::TcpConnection() {
	_closeState = new TcpClose();
	_EstablishState = new TcpEstablished();
	_listenState = new TcpListener();
	_state = _closeState;
}

void TcpConnection::Close() {
	_state->Close(this);
}

void TcpConnection::Open() {
	_state->Open(this);
}

void TcpConnection::Tranmit() {
	_state->Tranmit(this);
}

void TcpConnection::ChangeState(StateType type) {
	switch (type) {
	case StateType::Close : {
		_state = _closeState;
		break;
	}
	case StateType::Establish: {
		_state = _EstablishState;
		break;
	}
	case StateType::Listen: {
		_state = _listenState;
		break;
	}
	default:
		break;
	}
}