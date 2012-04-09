#include <cstdlib>
#include "thread.hpp"
#include "threadfactory.hpp"

using namespace CppThread;

/**
 * Constructor
 *
 * @param i     Pointer to the factory
 */
Thread::Thread ( ThreadFactory *i )
: m_functor( NULL ),
m_factInstance( i )
{
    
}


/**
 * Destructor
 *
 */
Thread::~Thread ( void )
{
    if ( m_factInstance != NULL )
        m_factInstance->removeThread( this );
}
