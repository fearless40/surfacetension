//////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------- SurfaceTension ---------------------------------------------//
//    																								//
// Date Created : 4/5/2002																			//
// File name    : refcount.h  																		//
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

#include <atomic>


#pragma once


class InvasiveRefCount
{
public:
	InvasiveRefCount() {
		std::atomic_store(&_ref,0);
	}
	void addRef()	{ std::atomic_fetch_add( &_ref, 1); }
	void decRef()	{ std::atomic_fetch_add( &_ref, -1); }
	int_fast32_t getRef() { return std::atomic_load(&_ref);	}

private:
	std::atomic_int_fast32_t _ref;
};

typedef InvasiveRefCount RefCount;
