#ifdef _DEBUG
#include <vld.h>
#endif

#include <windows.h>

#include "Game.hpp"

int main(int argc, char* argv[]){
	Game g;
	try{
		g.Run(argc, argv);
	}catch(TankSmasherException& e){
		MessageBoxA(NULL, e.what(), "Tank Smasher Exception Occurred", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}catch(std::exception& e){
		MessageBoxA(NULL, e.what(), "Exception Occurred", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}catch(...){
		MessageBoxA(NULL, "FooLolBlah exception", "Exception Occurred", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}

	return 0;
}