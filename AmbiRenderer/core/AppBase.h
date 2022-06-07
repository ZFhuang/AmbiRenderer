#pragma once
#include "Utils.h"

class AppBase {
public:
	virtual void Create() = 0;
	virtual void Update(float delta_time) = 0;
	virtual void Destory() = 0;
};

inline void AppBase::Create() {
	ABR_DEBUG_OUTPUT("Defalut Create!");
}

inline void AppBase::Update(float delta_time) {
	ABR_DEBUG_OUTPUT(delta_time);
}

inline void AppBase::Destory() {
	ABR_DEBUG_OUTPUT("Defalut Destory!");
}