#ifndef TS_EXCEPTION_HPP_
#define TS_EXCEPTION_HPP_

#include <exception>
#include <string>
#include <sstream>

class TankSmasherException: public std::exception{
public:
	TankSmasherException(const std::string& m):mWhat(m){}

	~TankSmasherException() throw() {}

	const char* what() const throw() {
		std::ostringstream ss;
		ss << "Tank Smasher Exception: " << mWhat;
		return ss.str().c_str();
	}

private:
	std::string mWhat;
};

#endif