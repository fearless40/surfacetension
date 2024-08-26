#pragma once
#include <unordered_map>
#include <algorithm>
#include "GameEvent.h"

// Very simple class to map key presses to game defined callback with game defined data
template <typename CallBackFunctions, typename CallBackData> 
class KeyMapper {
public:
	using KeyID = decltype( Engine::KeyEvent::vkCode); 
	static const unsigned int MAX_FREQ_KEYS = 24;

private:
	struct UserData{
		CallBackFunctions * func;
		CallBackData data;
	}; 

	KeyID mFreqUsedKeys[MAX_FREQ_KEYS];
	UserData mFreqUsedFunc[MAX_FREQ_KEYS]; 

public:

	KeyMapper() {
		std::fill(mFreqUsedKeys, &mFreqUsedKeys[MAX_FREQ_KEYS + 1], 0);
	}

	void inject_key(const Engine::KeyEvent& evt) {
		for (std::size_t index = 0; index < MAX_FREQ_KEYS; ++index) {
			if (mFreqUsedKeys[index] == evt.vkCode) {
				std::invoke(mFreqUsedFunc[index].func, mFreqUsedFunc[index].data);
				return;
			}
		}
	}

	void set_key(KeyID keyId, CallBackFunctions func, CallBackData data) {
		const UserData userData = { func, data }; 
		for (std::size_t index = 0; index < MAX_FREQ_KEYS; ++index) {
			if (mFreqUsedKeys[index] == 0) {
				mFreqUsedKeys[index] = keyId;
				mFreqUsedFunc[index] = userData;
				return;
			}
		}
	}


};