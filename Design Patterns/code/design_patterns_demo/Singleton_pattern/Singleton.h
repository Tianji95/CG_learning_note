#pragma once
#ifndef SINGLETON_H
#define SINGLETON_H
#include <iostream>

class Singleton {
public:
	static Singleton& GetInstance() { // 一定是static的，并且返回的是引用，这种是在C++11以后是线程安全的，C++11以前是不安全的。
		static Singleton single;
		return single;
	}
	void Draw() {
		std::cout << "Singleton Draw" << std::endl;
	}
private:
	Singleton(){ }
	virtual ~Singleton(){}
	Singleton(const Singleton& single) = delete;//注意删除拷贝构造函数
	const Singleton& operator=(const Singleton& single) = delete; // 注意删除赋值函数。
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
