//////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------- SurfaceTension ---------------------------------------------//
//    																								//
// Date Created : 4/6/2002																			//
// File name    : pointer.h   																		//
// Code type    : standard c/c++ header file  														//
// Description  : 																					//
// Author       : Adam Spivack																		//
//    																								//
//---------------------------------- Copyright Information -----------------------------------------//
//<$COPYRIGHT> <$VER=1>   																			//
// Copyright LiquidSpace  																			//
//	A company started by Adam Spivack and Daniel Spivack   											//
//	None of the code in the this file may be used without the permission of the author or authors. 	//
//	This code may become open source and if it does you are free to do with it what you want.  		//
//	Just leave this header in and add your name after this section 									//
//<$ENDCOPYRIGHT> 																					//
//////////////////////////////////////////////////////////////////////////////////////////////////////



// Note: This type needs to have your class publicly inherit from the class stRefCount
//		 otherwise you will get a compile time error

// Example to use this code:
/*
main() 
{
	class someClass : public stRefCount {};
	
	stPointer<someClass> ptr(new stdword);	// refcount = 1
	stPointer<someClass>  ptr2( ptr );			// refcount = 2

	ptr.release();	// refcount = 1
	ptr2.release();	// refcount = 0, object gets deleted
}
*/


#pragma once




template <class type>
class  RefCountPointer
{
	protected:
		type * data;

		typedef RefCountPointer<type> rPtr;

		
	
		
	public:
		
		// Unsafe
		type * get() const {return data;}

		RefCountPointer() : data(nullptr) { }
		RefCountPointer(type * d) : data(d)  { if(data) data->addRef(); }
		
		// Copy constructor
		RefCountPointer(const RefCountPointer<type> & stp) : data(stp.get()) { if(data) data->addRef(); }
		
		// Move constructor
		RefCountPointer(RefCountPointer<type> && stp) { 
			data = stp.data;
			stp.data = nullptr;
		}

		// Desctuction 
		~RefCountPointer() { 
			if(!data) return; 
			data->decRef(); 
			if(data->getRef() <= 0) 
				delete data;  
		}
		
		type & operator *() { return *data; }
		type * operator ->(){ return data;  }
		
		RefCountPointer<type> & operator =(const RefCountPointer<type> & stp) 
		{
			if(data)
				release();
			
			data = stp.data;
			if( data )
				data->addRef();
			return *this;
		}
		

		// Release
		void release()	{
			data->decRef(); 
			if(data->getRef() <= 0) 
				delete data; 
			
			// Make this pointer now invalid
			data = nullptr; 
		}

		operator bool () { return (data != nullptr ? true : false ); }

		bool operator == ( const RefCountPointer<type> & d ) const {
			return ( data == d.data ? true : false ); 
		}

		bool operator != ( const RefCountPointer<type> & d ) const {
			return ( data != d.data ? true : false );
		}

};

//template <class type> struct RefPtr : public RefCountPointer<type> {};


