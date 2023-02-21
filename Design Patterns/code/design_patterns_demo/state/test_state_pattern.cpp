#include "TcpConnection.h"
int main() {
	TcpConnection tcp;
	tcp.Open();
	tcp.Tranmit();
	tcp.Close();
}