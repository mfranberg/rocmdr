#include <pthread.h>

#include "pthread.hpp"

using namespace CppThread;

/**
 * Wrapper function to run the functor.
 */
void *
runFunctor(void *functor)
{
    Functor *f = (Functor *) functor;
    (*f)( );

    return NULL;
}

PThread::PThread(ThreadFactory *factory)
{


}

PThread::~PThread()
{
}

void
PThread::start(Functor *f)
{
    pthread_create( &m_thread, NULL, runFunctor, f ); 
}

void
PThread::join()
{
    pthread_join( m_thread, NULL );
}
