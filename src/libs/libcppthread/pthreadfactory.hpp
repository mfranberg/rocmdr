#ifndef __PTHREADFACTORY__
#define __PTHREADFACTORY__

#include <thread.hpp>
#include <threadfactory.hpp>

namespace CppThread
{

    class PThreadFactory : public ThreadFactory
    {
    public:
        /**
         * Default constructor.
         */
        PThreadFactory();

        /**
         * Destructor, clears all threads.
         */

        ~PThreadFactory();
        /**
         * Creates a c++ pthread wrapper.
         */        
        Thread *createThread();

        /**
         * Returns the singleton instance of the factory.
         *
         * The method is thread safe.
         */
        static ThreadFactory *getInstance();
    };
}

#endif /* End of __PTHREADFACTORY__ */
