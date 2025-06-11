#pragma once
#include "stdafx.h"

template<class T>
class singleton
{
public:
    static std::shared_ptr<T> Instance()
    {
        struct EnableMakeShared : public T { EnableMakeShared() : T() {} };

        std::call_once(m_onceFlag, []() { m_instance = std::make_shared<EnableMakeShared>(); });

        return m_instance;
    }

protected:
    singleton() = default;
    virtual ~singleton() = default;      

    BLOCK_COPY_MOVE(singleton);

private:
    static std::shared_ptr<T> m_instance;
    static std::once_flag m_onceFlag;
};

template<class T>
std::shared_ptr<T> singleton<T>::m_instance = nullptr;

template<class T>
std::once_flag singleton<T>::m_onceFlag;