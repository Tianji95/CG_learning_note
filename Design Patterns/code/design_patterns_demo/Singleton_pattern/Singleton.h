#pragma once
#ifndef SINGLETON_H
#define SINGLETON_H
#include <iostream>

class Singleton {
public:
	static Singleton& GetInstance() { // һ����static�ģ����ҷ��ص������ã���������C++11�Ժ����̰߳�ȫ�ģ�C++11��ǰ�ǲ���ȫ�ġ�
		static Singleton single;
		return single;
	}
	void Draw() {
		std::cout << "Singleton Draw" << std::endl;
	}
private:
	Singleton(){ }
	virtual ~Singleton(){}
	Singleton(const Singleton& single) = delete;//ע��ɾ���������캯��
	const Singleton& operator=(const Singleton& single) = delete; // ע��ɾ����ֵ������
};

template<typename T>
class SingletonTemplate {
public:
	static T& GetInstance() {
		static T t;
		return t;
	}
private:
	SingletonTemplate(){}
	~SingletonTemplate(){}
	SingletonTemplate(const SingletonTemplate& s) = delete;
	const SingletonTemplate& operator=(const SingletonTemplate& s) = delete;
};

class A {
public:
	void Draw() {
		std::cout << "A::Draw()" << std::endl;
	}
};
#endif // !SINGLETON_H
