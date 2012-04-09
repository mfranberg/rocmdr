#ifndef __PTHREAD__
#define __PTHREAD__

#include <pthread.h>

#include "bind.hpp"
#include "thread.hpp"
#include "pthreadfactory.hpp"

namespace CppThread
{
    /**
     * Wrapper class for a c pthread.
     */
    class PThread : public Thread
    {
        friend class PThreadFactory;
        
    private:
        /**
         * Constructor.
         *
         * Only accessed by the thread factory.
         */
        PThread(ThreadFactory *factory = NULL);

    public:
        virtual ~PThread();

        /**
         * Starts the thread.
         *
         * @param f Pointer to the functor to invoke in the thread.
         */
        virtual void start(Functor *f);

        /**
         * Waits for the thread to end.
         */
        virtual void join();

    private:
        /**
         * Internal pthread struct.
         */
        pthread_t m_thread;
    };
}

#endif /* End of __PTHREAD__ */
