#pragma once
class EngineCore;

#include "EngineCore.h"
#include "Utils.h"

class AppBase {
protected:
	virtual void Create();
	virtual void Update(float delta_time) = 0;
	virtual void Destroy() = 0;
	virtual void Init() final;

protected:
	typedef void (AppBase::* pFunc)();
	virtual void BindKeyDown(KEY k, void (AppBase::* func)()) final;

protected:
	EngineCore* engine = nullptr;

private:
	friend class EngineCore;
};

inline void AppBase::Update(float delta_time) {
	//ABR_DEBUG_OUTPUT(delta_time);
}

inline void AppBase::Destroy() {
	ABR_DEBUG_OUTPUT("Defalut Destroy!");
}