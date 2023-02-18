#pragma once
#ifndef SCANNER_H
#define SCANNER_H
#include <iostream>
class Scanner {
public:
	Scanner(std::istream& in) :_inputStream(in) {

	}
	virtual ~Scanner() {}
	virtual void Scan() {}
private:
	std::istream& _inputStream;
};
#endif // !SCANNER_H
