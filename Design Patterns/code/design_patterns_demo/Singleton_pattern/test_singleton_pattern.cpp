#include"Singleton.h"

int main() {
	Singleton::GetInstance().Draw();
	SingletonTemplate<A>::GetInstance().Draw();
  	return 0;
}