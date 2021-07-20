#pragma once

#include <string>
#include "HittableList.hpp"

class Scene {
public:
	virtual const std::string get_name() const = 0;
	virtual HittableList get_HittableList() const = 0;
};


// 生成少量随机球测试场景
class TestScene final : public Scene
{
public:
	TestScene();
	virtual const std::string get_name() const override;
	virtual HittableList get_HittableList() const override;

private:
	HittableList _hitlist;
};
