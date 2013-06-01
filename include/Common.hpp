#ifndef TS_COMMON_HPP_
#define TS_COMMON_HPP_

#define TS_VERSION_MAJOR 2
#define TS_VERSION_MINOR 5

template<typename T>
void SafeDelete(T*& p){
        if(p != 0){
		delete p;
                p = 0;
	}
}

const int gAnimationCorrector = 20;
const float gDegreesToRadians = 0.01745329251994329576923690768489f;

#endif
