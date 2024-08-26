
template <class Type, unsigned int NbrInitialVectors = 4, unsigned int SizeOfVectors = 10>
class VectorCache {
	using type = std::vector<Type>;

public:
	VectorCache() {
		vectors.reserve(NbrInitialVectors);
		for (auto & x : vectors) {
			x.reserve(SizeOfVectors);
		}
	}
	
	VectorCache(VectorCache & ) = delete;
	VectorCache(VectorCache && ) = delete;
	VectorCache & operator = (VectorCache &) = delete;


	type & get() {
#ifdef METRICS
		++NbrUsedVectors;
#endif 

		if (freelist.size() > 0) {
			auto value = freelist.back();
			freelist.pop_back();
			return vectors[value];
		}
		else
		{
			vectors.emplace_back();
			vectors.back().reserve(SizeOfVectors);
			return vectors.back();
		}
	}

	void recycle(type & ty) {
#ifdef METRICS
		--NbrUsedVetors;
		MaxVectorSize = std::max(ty.size(), MaxVectorSize);
		MinVectorSize = std::min(ty.size(), MinVectorSize);
#endif

		freelist.push_back(position(ty));
		ty.clear();
	}

	void resetall() {
		for (auto & x : vectors) {
			x.clear();
			freelist.push_back(position(x));
		}
	}

private:
	std::ptrdiff_t position(const type & x) {
		return &x - &vectors[0];
	}

	std::vector<type> vectors;
	std::vector<std::size_t> freelist;

#ifdef METRICS
	std::size_t NbrUsedVectors;
	std::size_t MaxUsedVectors;
	std::size_t MaxVectorSize;
	std::size_t MinVectorSize;
#endif

};