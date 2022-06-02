#pragma once
#include <memory>
#include <cassert>

template<typename T>
class Singleton {
public:
	template<typename... Args>
	static T* Initialize(Args&&... args) {
		if (m_pInstance == nullptr) {
			m_pInstance = new T(std::forward<Args>(args)...);
		}
		return m_pInstance;
	}

	static void Destory() {
		delete m_pInstance;
		m_pInstance = nullptr;
	}

	static T* GetInstance() {
		// ERROR: Singleton not initialized
		assert(m_pInstance != nullptr);
		return m_pInstance;
	}

private:
	Singleton(void) = delete;
	virtual ~Singleton(void) = delete;
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

private:
	static T* m_pInstance;
};

template<typename T> T* Singleton<T>::m_pInstance = nullptr;