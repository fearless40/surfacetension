#pragma once

#include <../boost/mpl/vector.hpp>
#include <../boost/mpl/find_if.hpp>
#include <../boost/mpl/less_equal.hpp>
#include <../boost/mpl/int.hpp>
#include <cstdint>

namespace  mpl = boost::mpl;

namespace Util {

	typedef mpl::vector< std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t > Size_Types_Int;

	/// Calculate the min size that the bits can be held in. Really only works with 
	//		intrinsic integer values. Use Size_Types_Int as the default
	// sz = integer representing number of bits requested
	// vec = mpl::sequence type (should be intrinsic value types)
	template <int sz, typename vec = Size_Types_Int>
	struct MinTypeSize {
		typedef mpl::int_<sz> BitSize;

		template< int item  >
		struct compare {
			template <class T> struct apply {
				typedef  typename mpl::less_equal< mpl::int_<item>, mpl::int_<sizeof(T) * 8> > type;
			};
		};

		typedef typename mpl::find_if<vec, compare<sz> >::type iter;
		typedef typename mpl::deref<iter>::type type;
	};
};