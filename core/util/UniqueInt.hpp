#pragma once
#include <cstdint>

namespace util
{

	/// Very simple class representing an opaque value
	// Template Parameter: Type is only used by the compiler to prevent ID from being
	//		crossed into different classes.

	template <class TypeTag, typename UnderLyingType = std::uint32_t>
	struct UniqueId
	{
		using value_type = UnderLyingType;
		value_type value;

		struct hash
		{
			std::size_t operator()(const value_type &k) const
			{
				return std::hash<value_type>()(k.value);
			}
		};

		value_type get() const { return value; }

		bool operator==(const UniqueId &k) const
		{
			return value == k.value;
		}

		bool operator!=(const UniqueId &k) const
		{
			return value != k.value;
		}

		bool operator<(const UniqueId &k) const
		{
			return value < k.value;
		}

		bool operator>(const UniqueId &k) const
		{
			return value > k.value;
		}

		bool operator<=(const UniqueId &k) const
		{
			return value <= k.value;
		}

		bool operator>=(const UniqueId &k) const
		{
			return value >= k.value;
		}
	};
}
