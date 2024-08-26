#pragma once

#include <../boost/mpl/assert.hpp>
#include "../util/MinTypeSize.hpp"
#include <cstdint>

namespace  mpl = boost::mpl;

namespace ComponentSys {
	// Sizes are in bits! Not bytes
	/*
	* Automatic Handle generator
	* The handle is stored as an integer that its size is automatically decided on by the compiler (it picks the smallest)
	* /param GenerationSize Number of BITS requested for the generation data
	* /param IndexSize Number of BITS requested for the index
	* /param UserSize Number of BITS requested for user data. Default is no user data
	*/
	template <int GenerationSize, int IndexSize, int UserSize = 0 >
	struct Handle {
		BOOST_MPL_ASSERT((mpl::less_equal< mpl::int_<GenerationSize + IndexSize + UserSize>, mpl::int_<64> >));

		// Type Defs
		typedef Handle<GenerationSize, IndexSize, UserSize> type;
		typedef typename Util::MinTypeSize<GenerationSize + IndexSize + UserSize>::type Handle_Type;
		typedef typename Util::MinTypeSize<GenerationSize>::type Generation_Type;
		typedef typename Util::MinTypeSize<IndexSize>::type Index_Type;
		typedef typename Util::MinTypeSize<UserSize>::type User_Type;

		// Masks for storing the data
		static const Handle_Type Generation_Mask = (1 << GenerationSize) - 1;
		static const Handle_Type Index_Mask = (1 << IndexSize) - 1;
		static const Handle_Type User_Mask = (1 << UserSize) - 1;


		// Actual Data Storage (just an integer)
		Handle_Type data = 0;

		// Constructors
		Handle() {};
		Handle(Handle_Type inType) : data(inType) { }
		Handle(Handle & gh) : data(gh.data) {}
		Handle(Handle_Type genBits, Handle_Type handBits, Handle_Type userBits = 0) {
			data = ((userBits & User_Mask) << (GenerationSize + IndexSize)) |
				((genBits & Generation_Mask) << (IndexSize)) |
				((handBits & Index_Mask));
		}

		static type make(Handle_Type genBits, Handle_Type handBits, Handle_Type userBits = 0) {
			Handle_Type v = ((userBits & User_Mask) << (GenerationSize + IndexSize)) |
				((genBits & Generation_Mask) << (IndexSize)) |
				((handBits & Index_Mask));
			return type(v);
		}

		Generation_Type generation() {
			return static_cast<Generation_Type>((data >> IndexSize) & Generation_Mask);
		}

		Index_Type index() {
			return static_cast<Index_Type>((data & Index_Mask));
		}

		User_Type user() {
			return static_cast<User_Type>((data >> (GenerationSize + IndexSize)) & User_Mask);
		}

		bool operator == (const type & comp) {
			return (data == comp.data);
		}

		bool operator != (const type & comp) {
			return (data != comp.data);
		}

	};
};