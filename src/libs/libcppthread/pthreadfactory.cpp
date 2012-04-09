#include "pthread.hpp"

#include "pthreadfactory.hpp"

using namespace CppThread;

PThreadFactory::PThreadFactory()
{

}

PThreadFactory::~PThreadFactory()
{
    deleteThreads( );
}

Thread *
PThreadFactory::createThread()
{
    PThread *pThread = new PThread( this );

    m_liveThreadList.push_back( pThread );

    return pThread; 
}

ThreadFactory *
PThreadFactory::getInstance()
{
    if( m_instance == NULL )
    {
        m_instance = new PThreadFactory( );
    }

    return m_instance;
}
