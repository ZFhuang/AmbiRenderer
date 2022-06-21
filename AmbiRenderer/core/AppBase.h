#pragma once
class EngineCore;

#include "EngineCore.h"
#include "Utils.h"

class AppBase {
public:
	virtual void Init();
	virtual void Update(float delta_time) = 0;
	virtual void Destroy() = 0;
	virtual void PreInit() final;

protected:
};

inline void AppBase::Update(float delta_time) {
	;
}

inline void AppBase::Destroy() {
	ABR_DEBUG_OUTPUT("Defalut Destroy!");
}