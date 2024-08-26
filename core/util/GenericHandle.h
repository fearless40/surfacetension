#pragma once


template < int GenerationBits, int HandleBits >
struct GenericHandle {
	uint32_t mGeneration	: GenerationBits;
	uint32_t mIndex			: HandleBits;

	GenericHandle(uint32_t gen, uint32_t index) : mGeneration(gen), mIndex(index) { }
	GenericHandle() : mGeneration(0), mIndex(0) {}

	bool operator == (const GenericHandle<GenerationBits, HandleBits> & compare) 
	{
		return  (*(uint32_t*)this == *(uint32_t*)compare ? true : false);
	}

	bool operator != (const GenericHandle<GenerationBits, HandleBits>& compare)
	{
		return (*(uint32_t*)this != *(uint32_t*)&compare ? true : false );
	}

	bool isNull() const { return mIndex == 0 ? true : false; }
};

template <int UserBits, int GenerationBits, int HandleBits>
struct GenericHandle3 
{
	uint32_t mUserData		: UserBits;
	uint32_t mGeneration	: GenerationBits;
	uint32_t mIndex			: HandleBits;

	GenericHandle3( uint32_t user, uint32_t gen, uint32_t index ) :
		mUserData( user ),
		mGeneration( gen ),
		mIndex( index ) { }

	GenericHandle3( uint32_t gen, uint32_t index ) :
		mUserData( 0 ),
		mGeneration( gen ),
		mIndex( index ) { }

	GenericHandle3( ) : 
		mUserData( 0 ),
		mGeneration( 0 ),
		mIndex( 0 ) { }

	inline uint32_t asInt() { return *((uint32_t *)this); }

	bool operator == ( const GenericHandle3<UserBits, GenerationBits, HandleBits> & compare ) const
	{
		return (asInt() == compare.asInt() ? true : false);
	}


	bool operator != ( const GenericHandle3<UserBits, GenerationBits, HandleBits> & compare ) const
	{
		return (asInt() != compare.asInt() ? true : false);
	}

	bool isNull() const
	{
		return (mIndex == 0 ? true : false);
	}
};
