#pragma once

#include <string>
#include "HittableList.hpp"

class Scene {
public:
	virtual const std::string get_name() const = 0;
	virtual HittableList get_HittableList() const = 0;
};


// 生成简单的球测试场景
class TestScene final : public Scene
{
public:
	TestScene();
	virtual const std::string get_name() const override;
	virtual HittableList get_HittableList() const override;

private:
	HittableList _hitlist;
};

// 生成大量随机球测试场景
class RandomScene final : public Scene
{
public:
	RandomScene();
	virtual const std::string get_name() const override;
	virtual HittableList get_HittableList() const override;

private:
	HittableList _hitlist;
};

// 两个泊林噪声的球
class PerlinScene final : public Scene
{
public:
	PerlinScene();
	virtual const std::string get_name() const override;
	virtual HittableList get_HittableList() const override;

private:
	HittableList _hitlist;
};

// 生成单地球场景
class EarthScene final : public Scene
{
public:
	EarthScene();
	virtual const std::string get_name() const override;
	virtual HittableList get_HittableList() const override;

private:
	HittableList _hitlist;
};

// 生成矩形光源场景
class SimpleLightScene final : public Scene
{
public:
	SimpleLightScene();
	virtual const std::string get_name() const override;
	virtual HittableList get_HittableList() const override;

private:
	HittableList _hitlist;
};

// 最关键的场景: cornell盒
class CornelOriScene final : public Scene
{
public:
	CornelOriScene();
	virtual const std::string get_name() const override;
	virtual HittableList get_HittableList() const override;

private:
	HittableList _hitlist;
};

// 体积雾版本的cornell盒
class CornelSmokeScene final : public Scene
{
public:
	CornelSmokeScene();
	virtual const std::string get_name() const override;
	virtual HittableList get_HittableList() const override;

private:
	HittableList _hitlist;
};

// RT2的最终场景
class FinalScene final : public Scene
{
public:
	FinalScene();
	virtual const std::string get_name() const override;
	virtual HittableList get_HittableList() const override;

private:
	HittableList _hitlist;
};