#pragma once
#pragma once

#include <chrono>
#include <iostream>


namespace util
{

	using namespace std::chrono_literals;

	constexpr std::chrono::nanoseconds _30fps{ 33333332ns };
	constexpr std::chrono::nanoseconds _60fps{ 16666666ns };
	constexpr std::chrono::nanoseconds _120fps{ 8333333ns };
	constexpr std::chrono::nanoseconds _144fps{ 6944444ns };

	class TimeStep
	{
	public:
		void start() noexcept
		{
			_start = clock::now();
		}

		void startFrame() noexcept
		{
			auto deltaTime = clock::now() - _start;
			_start = clock::now();
			_lag += std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTime);
		}

		bool isUpdateRequired() const noexcept
		{
			return (_lag >= _timestep);
		}

		void performUpdate() noexcept
		{
			_lag -= _timestep;
		};

		float getInterpolation() const noexcept
		{
			return static_cast<float>(_lag.count()) / _timestep.count();
		}

	private:
		using clock = std::chrono::steady_clock;
		std::chrono::nanoseconds _lag{ 0ns };
		std::chrono::nanoseconds _timestep{ _60fps };
		clock::time_point _start{ clock::now() };
	};
}