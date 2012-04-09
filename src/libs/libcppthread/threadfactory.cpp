#include <cstdlib>
#include "threadfactory.hpp"
#include "pthreadfactory.hpp"

using namespace CppThread;

/**
 * Initialize singleton instance field
 */
ThreadFactory *ThreadFactory::m_instance = NULL;

/**
 * Constructor
 *
 *
 */
ThreadFactory::ThreadFactory( void )
{

}

/**
 * Destructor
 *
 *
 */
ThreadFactory::~ThreadFactory( void )
{
    m_instance = NULL;
}

/**
 * Removes a thread from the live list
 *
 * @param t     The thread to remove
 *
 */
void ThreadFactory::removeThread ( Thread *t )
{
    m_liveThreadList.remove( t );
}

/**
 * Deletes all the threads
 *
 */
void ThreadFactory::deleteThreads ( void )
{
    //
    // The destructor of implementation of the thread should
    // free all the platform dependent resources. The destructor
    // of the base class will remove it from the live list.
    //
    while ( m_liveThreadList.empty( ) == false )
        delete m_liveThreadList.front( );
}

/**
 * Returns the singleton instance of the factory.
 * the method is thread safe.
 *
 */
ThreadFactory* ThreadFactory::getInstance ( void )
{
    if ( m_instance != NULL )
        return m_instance;

    return PThreadFactory::getInstance( );
}
