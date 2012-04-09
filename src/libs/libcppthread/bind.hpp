#ifndef __BIND_HPP__
#define	__BIND_HPP__

#include <list>

namespace CppThread
{
    class Functor
    {
    public:
        /**
         * Constructor
         *
         */
        Functor ( void )
        {
            /* FunctorObjectHandler::getInstance( )->addFunctor( this ); */
        }

        /**
         * Destructor
         *
         */
        virtual ~Functor ( void )
        {
            /* FunctorObjectHandler::getInstance( )->removeFunctor( this ); */
        }

        /**
         * Invokes the function/method that the functor wraps
         *
         */
        virtual void operator () ( void ) = 0;
    };


    template <typename CLASS>
    class Functor0 : public Functor
    {
    private:
        typedef void (CLASS::*METHOD) ( void );
        CLASS*  m_object;
        METHOD  m_method;

    public:
        /**
         * Constructor
         *
         */
        Functor0 ( METHOD m, CLASS *o )
        {
            m_object = o;
            m_method = m;
        }

        /**
         * Copy constructor
         *
         */
        Functor0 ( const Functor0 &f )
        {
            m_object = f.m_object;
            m_method = f.m_method;
        }

        /**
         * Invokes the function/method that the functor wraps
         *
         */
        virtual void operator () ( void )
        {
            (m_object->*m_method)( );
        }
    };

    template <typename CLASS, typename T0>
    class Functor1 : public Functor
    {
    private:
        typedef void (CLASS::*METHOD) ( T0 );
        CLASS*  m_object;
        METHOD  m_method;
        T0      m_param0;

    public:
        /**
         * Constructor
         *
         */
        Functor1 ( METHOD m, 
                   CLASS *o, 
                   T0 p0 )
        : m_param0( p0 )
        {
            m_object = o;
            m_method = m;
        }

        /**
         * Copy constructor
         *
         */
        Functor1 ( const Functor1 &f )
        : m_param0( f.m_param0 )
        {
            m_object = f.m_object;
            m_method = f.m_method;

        }

        /**
         * Invokes the function/method that the functor wraps
         *
         */
        virtual void operator () ( void )
        {
            (m_object->*m_method)( m_param0 );
        }
    };

    template <typename CLASS, typename T0, typename T1>
    class Functor2 : public Functor
    {
    private:
        typedef void (CLASS::*METHOD) ( T0, T1 );
        CLASS*  m_object;
        METHOD  m_method;
        T0      m_param0;
        T1      m_param1;

    public:
        /**
         * Constructor
         *
         */
        Functor2 ( METHOD m, 
                   CLASS *o, 
                   T0 p0, 
                   T1 p1 )
        : m_param0( p0 ), 
          m_param1( p1 )
        {
            m_object = o;
            m_method = m;
        }

        /**
         * Copy constructor
         *
         */
        Functor2 ( const Functor2 &f )
        : m_param0( f.m_param0 ), 
          m_param1( f.m_param1 )
        {
            m_object = f.m_object;
            m_method = f.m_method;
        }

        /**
         * Invokes the function/method that the functor wraps
         *
         */
        virtual void operator () ( void )
        {
            (m_object->*m_method)( m_param0, m_param1 );
        }
    };

    template <typename CLASS, typename T0, typename T1, typename T2>
    class Functor3 : public Functor
    {
    private:
        typedef void (CLASS::*METHOD) ( T0, T1, T2 );
        CLASS*  m_object;
        METHOD  m_method;
        T0      m_param0;
        T1      m_param1;
        T2      m_param2;

    public:
        /**
         * Constructor
         *
         */
        Functor3 ( METHOD m, 
                   CLASS *o, 
                   T0 p0, 
                   T1 p1, 
                   T2 p2 )
        : m_param0( p0 ), 
          m_param1( p1 ), 
          m_param2( p2 )
        {
            m_object = o;
            m_method = m;
        }

        /**
         * Copy constructor
         *
         */
        Functor3 ( const Functor3 &f )
        : m_param0( f.m_param0 ), 
          m_param1( f.m_param1 ), 
          m_param2( f.m_param2 )
        {
            m_object = f.m_object;
            m_method = f.m_method;
        }

        /**
         * Invokes the function/method that the functor wraps
         *
         */
        virtual void operator () ( void )
        {
            (m_object->*m_method)( m_param0, m_param1, m_param2 );
        }
    };

    template <typename CLASS, typename T0, typename T1, typename T2, typename T3>
    class Functor4 : public Functor
    {
    private:
        typedef void (CLASS::*METHOD) ( T0, T1, T2, T3 );
        CLASS*  m_object;
        METHOD  m_method;
        T0      m_param0;
        T1      m_param1;
        T2      m_param2;
        T3      m_param3;

    public:
        /**
         * Constructor
         *
         */
        Functor4 ( METHOD m, 
                   CLASS *o, 
                   T0 p0, 
                   T1 p1, 
                   T2 p2, 
                   T3 p3 )
        : m_param0( p0 ), 
          m_param1( p1 ), 
          m_param2( p2 ), 
          m_param3( p3 )
        {
            m_object = o;
            m_method = m;
        }

        /**
         * Copy constructor
         *
         */
        Functor4 ( const Functor4 &f )
        : m_param0( f.m_param0 ), 
          m_param1( f.m_param1 ), 
          m_param2( f.m_param2 ), 
          m_param3( f.m_param3 )
        {
            m_object = f.m_object;
            m_method = f.m_method;
        }

        /**
         * Invokes the function/method that the functor wraps
         *
         */
        virtual void operator () ( void )
        {
            (m_object->*m_method)( m_param0, m_param1, m_param2, m_param3 );
        }
    };

    template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
           class Functor6 : public Functor
           {
           private:
               typedef void (CLASS::*METHOD) ( T0, T1, T2, T3, T4, T5 );
               CLASS*  m_object;
               METHOD  m_method;
               T0      m_param0;
               T1      m_param1;
               T2      m_param2;
               T3      m_param3;
               T4      m_param4;
               T5      m_param5;

           public:
               /**
                * Constructor
                *
                */
               Functor6 ( METHOD m, CLASS *o, T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
               : m_param0( p0 ),
                 m_param1( p1 ),
                 m_param2( p2 ),
                 m_param3( p3 ),
                 m_param4( p4 ),
                 m_param5( p5 )
               {
                   m_object = o;
                   m_method = m;
               }

               /**
                * Copy constructor
                *
                */
               Functor6 ( const Functor6 &f )
               : m_param0( f.m_param0 ),
                 m_param1( f.m_param1 ),
                 m_param2( f.m_param2 ),
                 m_param3( f.m_param3 ),
                 m_param4( f.m_param4 ),
                 m_param4( f.m_param5 )
               {
                   m_object = f.m_object;
                   m_method = f.m_method;
               }

               /**
                * Invokes the function/method that the functor wraps
                *
                */
               virtual void operator () ( void )
               {
            	   (m_object->*m_method)( m_param0, m_param1, m_param2, m_param3, m_param4, m_param5 );
               }
           };
    

    class Functorf0 : public Functor
    {
    private:
        typedef void (*FUNCT) ( void );
        FUNCT   m_funct;

    public:
        /**
         * Constructor
         *
         */
        Functorf0 ( FUNCT f )
        {
            m_funct = f;
        }

        /**
         * Copy constructor
         *
         */
                    Functorf0 ( const Functorf0 &f ) : Functor()
        {
            m_funct = f.m_funct;
        }

        /**
         * Invokes the function/method that the functor wraps
         *
         */
        virtual void operator () ( void )
        {
            m_funct( );
        }
    };

    template <typename T0>
    class Functorf1 : public Functor
    {
    private:
        typedef void (*FUNCT) ( T0 );
        FUNCT   m_funct;
        T0		m_param0;

    public:
        /**
         * Constructor
         *
         */
        Functorf1 ( FUNCT f, T0 p0 )
        : m_param0( p0 )
        {
            m_funct = f;
        }

        /**
         * Copy constructor
         *
         */
        Functorf1 ( const Functorf1 &f )
        : m_param0( f.m_param0 )
        {
            m_funct = f.m_funct;
        }

        /**
         * Invokes the function/method that the functor wraps
         *
         */
        virtual void operator () ( void )
        {
            m_funct( m_param0 );
        }
    };

    template <typename T0, typename T1>
    class Functorf2 : public Functor
    {
    private:
        typedef void (*FUNCT) ( T0, T1 );
        FUNCT   m_funct;
        T0      m_param0;
        T1      m_param1;

    public:
        /**
         * Constructor
         *
         */
        Functorf2 ( FUNCT f, T0 p0, T1 p1 )
        : m_param0( p0 ),
          m_param1( p1 )
        {
            m_funct = f;
        }

        /**
         * Copy constructor
         *
         */
        Functorf2 ( const Functorf2 &f )
        : m_param0( f.m_param0 ),
          m_param1( f.m_param1 )
        {
            m_funct = f.m_funct;
        }

        /**
         * Invokes the function/method that the functor wraps
         *
         */
        virtual void operator () ( void )
        {
            m_funct( m_param0, m_param1 );
        }
    };

    template <typename T0, typename T1, typename T2>
    class Functorf3 : public Functor
    {
    private:
        typedef void (*FUNCT) ( T0, T1, T2 );
        FUNCT   m_funct;
        T0      m_param0;
        T1      m_param1;
        T2      m_param2;

    public:
        /**
         * Constructor
         *
         */
        Functorf3 ( FUNCT f, T0 p0, T1 p1, T2 p2 )
        : m_param0( p0 ),
          m_param1( p1 ),
          m_param2( p2 )
        {
            m_funct = f;
        }

        /**
         * Copy constructor
         *
         */
        Functorf3 ( const Functorf3 &f )
        : m_param0( f.m_param0 ),
          m_param1( f.m_param1 ),
          m_param2( f.m_param2 )
        {
            m_funct = f.m_funct;
        }

        /**
         * Invokes the function/method that the functor wraps
         *
         */
        virtual void operator () ( void )
        {
            m_funct( m_param0, m_param1, m_param2 );
        }
    };

    template <typename T0, typename T1, typename T2, typename T3>
    class Functorf4 : public Functor
    {
    private:
        typedef void (*FUNCT) ( T0, T1, T2, T3 );
        FUNCT   m_funct;
        T0      m_param0;
        T1      m_param1;
        T2      m_param2;
        T3      m_param3;

    public:
        /**
         * Constructor
         *
         */
        Functorf4 ( FUNCT f, T0 p0, T1 p1, T2 p2, T3 p3 )
        : m_param0( p0 ),
          m_param1( p1 ),
          m_param2( p2 ),
          m_param3( p3 )
        {
            m_funct = f;
        }

        /**
         * Copy constructor
         *
         */
        Functorf4 ( const Functorf4 &f )
        : m_param0( f.m_param0 ),
          m_param1( f.m_param1 ),
          m_param2( f.m_param2 ),
          m_param3( f.m_param3 )
        {
            m_funct = f.m_funct;
        }

        /**
         * Invokes the function/method that the functor wraps
         *
         */
        virtual void operator () ( void )
        {
            m_funct( m_param0, m_param1, m_param2, m_param3 );
        }
    };

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
	class Functorf6 : public Functor
	{
	private:
		typedef void (*FUNCT) ( T0, T1, T2, T3, T4, T5 );
		FUNCT   m_funct;
		T0      m_param0;
		T1      m_param1;
		T2      m_param2;
		T3      m_param3;
		T4      m_param4;
		T5      m_param5;

	public:
		/**
		 * Constructor
		 *
		 */
		Functorf6 ( FUNCT f, T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
		: m_param0( p0 ),
		  m_param1( p1 ),
		  m_param2( p2 ),
		  m_param3( p3 ),
		  m_param4( p4 ),
		  m_param5( p5 )
		{
			m_funct = f;
		}

		/**
		 * Copy constructor
		 *
		 */
		Functorf6 ( const Functorf6 &f )
		: m_param0( f.m_param0 ),
		  m_param1( f.m_param1 ),
		  m_param2( f.m_param2 ),
		  m_param3( f.m_param3 ),
		  m_param4( f.m_param4 ),
		  m_param4( f.m_param5 )
		{
			m_funct = f.m_funct;
		}

		/**
		 * Invokes the function/method that the functor wraps
		 *
		 */
		virtual void operator () ( void )
		{
			m_funct( m_param0, m_param1, m_param2, m_param3, m_param4, m_param5 );
		}
	};

    template <typename CLASS>
    Functor* bind ( void (CLASS::*m) ( void ), CLASS *o )
    {
        return new Functor0<CLASS>( m, o );
    }

    template <typename CLASS, typename T0>
    Functor* bind ( void (CLASS::*m) ( T0 ), CLASS *o, T0 p0 )
    {
        return new Functor1<CLASS, T0>( m, o, p0 );
    }

    template <typename CLASS, typename T0, typename T1>
    Functor* bind ( void (CLASS::*m) ( T0, T1 ), CLASS *o, T0 p0, T1 p1 )
    {
        return new Functor2<CLASS, T0, T1>( m, o, p0, p1 );
    }

    template <typename CLASS, typename T0, typename T1, typename T2>
    Functor* bind ( void (CLASS::*m) ( T0, T1, T2 ), CLASS *o, T0 p0, T1 p1, T2 p2 )
    {
        return new Functor3<CLASS, T0, T1, T2>( m, o, p0, p1, p2 );
    }

    template <typename CLASS, typename T0, typename T1, typename T2, typename T3>
    Functor* bind ( void (CLASS::*m) ( T0, T1, T2, T3 ), CLASS *o, T0 p0, T1 p1, T2 p2, T3 p3 )
    {
        return new Functor4<CLASS, T0, T1, T2, T3>( m, o, p0, p1, p2, p3 );
    }
    
    template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    Functor* bind ( void (CLASS::*m) ( T0, T1, T2, T3, T4, T5 ), CLASS *o, T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
    {
        return new Functor6<CLASS, T0, T1, T2, T3, T4, T5 >( m, o, p0, p1, p2, p3, p4, p5 );
    }

    template <int T>
    Functor* bind ( void (*f) ( void ) )
    {
        return new Functorf0( f );
    }

    template <typename T0>
    Functor* bind ( void (*f) ( T0 ), T0 p0 )
    {
        return new Functorf1<T0>( f, p0 );
    }

    template <typename T0, typename T1>
    Functor* bind ( void (*f) ( T0, T1 ), T0 p0, T1 p1 )
    {
        return new Functorf2<T0, T1>( f, p0, p1 );
    }

    template <typename T0, typename T1, typename T2>
    Functor* bind ( void (*f) ( T0, T1, T2 ), T0 p0, T1 p1, T2 p2 )
    {
        return new Functorf3<T0, T1, T2>( f, p0, p1, p2 );
    }

    template <typename T0, typename T1, typename T2, typename T3>
    Functor* bind ( void (*f) ( T0, T1, T2, T3 ), T0 p0, T1 p1, T2 p2, T3 p3 )
    {
        return new Functorf4<T0, T1, T2, T3>( f, p0, p1, p2, p3 );
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    Functor* bind ( void (*f) ( T0, T1, T2, T3, T4, T5 ), T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
    {
        return new Functorf6<T0, T1, T2, T3, T4, T5 >( f, p0, p1, p2, p3, p4, p5 );
    }
}

#endif	/* __BIND_HPP__ */
