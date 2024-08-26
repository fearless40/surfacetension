
#include <iterator>

#include <type_traits>


namespace Util {
	template <typename Fn, typename Argument, std::size_t... Ns>
	auto tuple_transform_impl(Fn&& fn, Argument&& argument, std::index_sequence<Ns...>) {
		if constexpr (sizeof...(Ns) == 0) return std::tuple<>(); // empty tuple
		else if constexpr (std::is_same_v<decltype(fn(std::get<0>(argument))), void>) {
			(fn(std::get<Ns>(argument)), ...); // no return value expected
			return;
		}
		// then dispatch lvalue, rvalue ref, temporary
		else if constexpr (std::is_lvalue_reference_v<decltype(fn(std::get<0>(argument)))>) {
			return std::tie(fn(std::get<Ns>(argument))...);
		}
		else if constexpr (std::is_rvalue_reference_v<decltype(fn(std::get<0>(argument)))>) {
			return std::forward_as_tuple(fn(std::get<Ns>(argument))...);
		}
		else {
			return std::tuple(fn(std::get<Ns>(argument))...);
		}
	}

	template <typename Fn, typename... Ts>
	auto tuple_transform(Fn&& fn, const std::tuple<Ts...>& tuple) {
		return tuple_transform_impl(std::forward<Fn>(fn), tuple,
			std::make_index_sequence<sizeof...(Ts)>());
	}

	template<typename ... types>
	class MultiIterator {
		std::size_t num{ 0 };
		std::tuple<types * ...> start{};
	public:
		
		class EndIterator {
			std::size_t last{ 0 };

		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using pointer = std::size_t;
			using reference = std::size_t;

			explicit EndIterator(std::size_t last) : last{ last } {}
			EndIterator& operator++() { return *this; }
			EndIterator operator++(int) { return *this }
			bool operator==(EndIterator other) const { return num == other.num; }
			bool operator!=(EndIterator other) const { return num != other.num; }
			value_type operator *() const { return last; }

		};


		using iterator_category = std::random_access_iterator_tag;
		using value_type = std::tuple<types *...>;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type * ;
		using reference = value_type &;

		explicit MultiIterator(std::size_t _num = 0, types * ... values) : num{ _num } {
			start = { values ... };
		}
		MultiIterator& operator++() { ++num; return *this; }
		MultiIterator operator++(int) { iterator retval = *this; ++(*this); return retval; }
		bool operator==(MultiIterator other) const { return num == other.num; }
		bool operator!=(MultiIterator other) const { return num != other.num; }
		bool operator==(EndIterator other) const { return num == *other; }
		bool operator!=(EndIterator other) const { return num != *other; }
//		bool operator!=(value_type other) const { other != *this; }

		value_type operator*() const {
			return tuple_transform( [num = this->num](auto val) {
				return val += num; }
			, start);
		};

	
	};
}