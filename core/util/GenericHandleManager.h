

#pragma once

namespace GenericHandleManager 
{

	template< class ValueType >
	struct ItemDescription
	{
		uint32_t mGeneration;
		ValueType mValue;
	};

	// handles are 1 based while the underlying vector is 0 based
	// Handles are one base as a handle value of 0 is considered null

	template < class HandleType , class ValueType >
	ValueType & fromHandle( HandleType handle, std::vector< ItemDescription< ValueType > > & mItems ) 
	{
	#ifdef _DEBUG
			//ASSERT( isValidHandle( handle ) == true );
	#endif
			return  mItems[handle.mIndex-1].mValue;
	}

	template < class HandleType , class ValueType >
	bool isValidHandle( HandleType handle, std::vector< ItemDescription< ValueType > > & mItems )
	{
		if( handle.mGeneration == 0 || handle.mIndex > mItems.size()+1 || handle.mGeneration != mItems[handle.mIndex-1].mGeneration )
			return false;

		return true;
	}

	template < class HandleType , class ValueType >
	HandleType newHandle(ValueType & vt, std::vector< ItemDescription< ValueType > > & mItems, std::vector<uint32_t> & mFreeList )
	{
		ItemDescription<ValueType> it = { 1, vt };
		uint32_t indexPos = 0;
		if( mFreeList.size() > 0 )
		{
			indexPos = mFreeList.back();
			mFreeList.pop_back();
			it.mGeneration = mItems[indexPos].mGeneration + 1;
			mItems[indexPos] = it;
			return HandleType( it.mGeneration, indexPos + 1  );
		}
		else
		{
			mItems.push_back(it);
			return HandleType( mItems.back().mGeneration, std::distance(mItems.begin() , mItems.end() ) );
		}
		
	}

	template < class HandleType , class ValueType >
	void deleteHandle( HandleType handle, std::vector< ItemDescription< ValueType > > & mItems, std::vector<uint32_t> & mFreeList )
	{
		mFreeList.push_back( handle.mIndex-1 );
		++mItems[ handle.mIndex-1 ].mGeneration;
	}

	template < class ValueType >
	void clearAll( std::vector< ItemDescription< ValueType > > & mItems, std::vector<uint32_t> & mFreeList)
	{
		for( auto item : mItems ) 
		{
			++item.mGeneration;
		}
		mItems.clear();
		mFreeList.clear();
	}

	template <class HandleType, class ValueType>
	HandleType clone( HandleType item, std::vector< ItemDescription< ValueType > > & mItems, std::vector<uint32_t> & mFreeList )
	{
		return newHandle<HandleType, ValueType>( fromHandle( item, mItems ), mItems, mFreeList );
	}
};


#define DECLARE_MANAGER( ValueType, HandleType ) public: static ValueType & fromHandle( HandleType handle ); \
	static void deleteHandle( HandleType handle ); \
	static void clearAll(); \
	static bool isValidHandle( HandleType handle );\
	static HandleType newHandle(); \
	static HandleType clone( HandleType handle ); \
	protected: \
	typedef std::vector< GenericHandleManager::ItemDescription< ValueType > > ItemArray; \
	static ItemArray mItems; static std::vector<uint32_t> mFreeList;


#define IMPLEMENT_MANAGER( ValueType, HandleType, ClassName ) \
	ClassName::ItemArray ClassName::mItems; std::vector<uint32_t> ClassName::mFreeList; \
	ValueType & ClassName::fromHandle( HandleType handle ) { return GenericHandleManager::fromHandle( handle, mItems ); } \
	void ClassName::deleteHandle( HandleType handle ) { GenericHandleManager::deleteHandle( handle, mItems, mFreeList ); } \
	void ClassName::clearAll() { GenericHandleManager::clearAll( mItems, mFreeList ); } \
	bool ClassName::isValidHandle( HandleType handle ) { return GenericHandleManager::isValidHandle( handle, mItems ); } \
	HandleType ClassName::newHandle() { ValueType item; return GenericHandleManager::newHandle<HandleType, ValueType> ( item, mItems, mFreeList ); } \
	HandleType ClassName::clone( HandleType handle ) { return GenericHandleManager::clone( handle, mItems, mFreeList ); } 



