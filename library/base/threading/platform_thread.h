
#ifndef __base_threading_platform_thread_h__
#define __base_threading_platform_thread_h__

#pragma once

#include <windows.h>

#include "base/basic_types.h"

namespace base
{

    // PlatformThreadHandle should not be assumed to be a numeric type, since the
    // standard intends to allow pthread_t to be a structure.  This means you
    // should not initialize it to a value, like 0.  If it's a member variable, the
    // constructor can safely "value initialize" using () in the initializer list.
    typedef DWORD PlatformThreadId;
    typedef void* PlatformThreadHandle; // HANDLE
    const PlatformThreadHandle kNullThreadHandle = NULL;

    const DWORD kInvalidThreadId = 0;

    // A namespace for low-level thread functions.
    class PlatformThread
    {
    public:
        // ʵ�ֱ��ӿ�, �ں�̨�߳���ִ�д���. �´������̻߳����ThreadMain����.
        class Delegate
        {
        public:
            virtual ~Delegate() {}
            virtual void ThreadMain() = 0;
        };

        // Gets the current thread id, which may be useful for logging purposes.
        static PlatformThreadId CurrentId();

        // Yield the current thread so another thread can be scheduled.
        static void YieldCurrentThread();

        // Sleeps for the specified duration (units are milliseconds).
        static void Sleep(int duration_ms);

        // ���öԵ������ɼ����߳�����. û�е������Ļ�, �����κ�����.
        static void SetName(const char* name);

        // ����һ�����߳�. |stack_size|��������Ϊ0, ��ʾʹ��ȱʡ��ջ�ռ��С. ���
        // �ɹ�, |*thread_handle|����ֵΪ�´������߳̾��, |delegate|��ThreadMain
        // ���������߳���ִ��.
        // ע��: �߳̾������ʹ��ʱ, �������Join�����ͷ��߳���ص�ϵͳ��Դ. ȷ��
        // Delegate�������߳��˳�ǰ����.
        static bool Create(size_t stack_size, Delegate* delegate,
            PlatformThreadHandle* thread_handle);

        // CreateNonJoinable() does the same thing as Create() except the thread
        // cannot be Join()'d.  Therefore, it also does not output a
        // PlatformThreadHandle.
        static bool CreateNonJoinable(size_t stack_size, Delegate* delegate);

        // ����һ��ͨ��Create�����������߳�. �������ö���ֱ��Ŀ���߳��˳�. �ᵼ��
        // |thread_handle|�Ƿ�.
        static void Join(PlatformThreadHandle thread_handle);

    private:
        DISALLOW_IMPLICIT_CONSTRUCTORS(PlatformThread);
    };

} //namespace base

#endif //__base_threading_platform_thread_h__