#ifndef TS_LOGGER_HPP_
#define TS_LOGGER_HPP_

#include <string>
#include <fstream>
#include <cassert>

#include "TankSmasherException.hpp"
#include "Common.hpp"

class Logger{
public:
	static Logger* GetInstance(){
		static Logger instance;
		return &instance;
	}

	void Log(const std::string& m){
		assert(mFile.is_open());

		mFile << m << std::endl;
	}

private:
	Logger(){
		mFile.open("TankSmasher.log", std::ios::trunc);

		if(!mFile.is_open()){
			throw TankSmasherException("Failed to open log file. Make sure you have permission to create the file.");
		}

		mFile << "Tank Smasher by skwee (s.kwee357 [at] gmail.com) version " << TS_VERSION_MAJOR << "." <<
			TS_VERSION_MINOR << std::endl;
		Log("Logger created");
	}

	~Logger(){
		Log("Logger destroyed");

		if(mFile.is_open()){
			mFile.flush();
			mFile.close();
		}
	}

	std::ofstream mFile;
};

#endif