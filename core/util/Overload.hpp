#pragma once

namespace Util {
	template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
	template<class... Ts> overload(Ts...)->overload<Ts...>;


	template <class F, class Variant, size_t... N>
	decltype(auto) invoke_active(F&& f, Variant&& v, std::index_sequence<N...>)
	{
		using Ret = decltype(std::invoke(std::forward<F>(f), std::get<0>(std::forward<Variant>(v))));

		if constexpr (!std::is_same_v<void, Ret>)
		{
			Ret ret;
			if (!((v.index() == N ? (ret = std::invoke(std::forward<F>(f), std::get<N>(std::forward<Variant>(v))), true) : false) || ...))
				throw std::bad_variant_access{};

			return ret;
		}
		else
		{
			if (!((v.index() == N ? (std::invoke(std::forward<F>(f), std::get<N>(std::forward<Variant>(v))), true) : false) || ...))
				throw std::bad_variant_access{};
		}
	}
	template <class F, class Variant>
	decltype(auto) myvisit(F&& f, Variant&& var)
	{
		return invoke_active(std::forward<F>(f), std::forward<Variant>(var)
			, std::make_index_sequence<std::variant_size_v<std::remove_reference_t<Variant>>>{});
	}


	template< typename Variant, typename... Handlers>
	auto match(Variant && v, Handlers &&... handlers) {
		return std::visit(
			overload{ std::forward<Handlers>(handlers)... },
			std::forward<Variant>(v));
	}




}