#include "AppBase.h"

void AppBase::Init()
{
	engine = Singleton<EngineCore>::GetInstance();
}

void AppBase::BindKeyDown(KEY k, void (AppBase::* func)())
{
	engine->BindFunc(k, func);
}

void AppBase::Create() {
	ABR_DEBUG_OUTPUT("Defalut Create!");
}