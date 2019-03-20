#pragma once

#include "SetCover.h"

namespace carl::covering {

/**
 * Represents a set cover problem where a set is represented by some type.
 * It actually wraps a SetCover class and takes care of mapping the custom set type to an id type.
 */
template<typename Set>
class TypedSetCover {
public:
	template<typename T>
	friend std::ostream& operator<<(std::ostream& os, const TypedSetCover<T>& tsc);
private:
	/// The actual set cover.
	SetCover mSetCover;
	/// Maps id to set.
	std::vector<Set> mSets;
	/// Maps set to id.
	std::map<Set, std::size_t> mSetMap;

	/// Gets id for a set, creates a new id if necessary.
	std::size_t get_set_id(const Set& s) {
		auto it = mSetMap.try_emplace(s, mSetMap.size());
		if (it.second) {
			assert(it.first->second == mSets.size());
			mSets.emplace_back(s);
		}
		return it.first->second;
	}
public:
	/// States that s covers the given element.
	void set(const Set& s, std::size_t element) {
		mSetCover.set(get_set_id(s), element);
	}
	/// States that s covers the given elements.
	void set(const Set& s, const Bitset& elements) {
		mSetCover.set(get_set_id(s), elements);
	}

	/// Returns the underlying set cover.
	operator const SetCover&() const {
		return mSetCover;
	}

	/// Convenience function to run the given heuristic on this set cover.
	template<typename F>
	std::vector<Set> get_cover(F&& heuristic) {
		std::vector<Set> res;
		for (std::size_t id: heuristic(mSetCover)) {
			res.emplace_back(mSets[id]);
		}
		return res;
	}
};

/// Print the typed set cover to os.
template<typename T>
std::ostream& operator<<(std::ostream& os, const TypedSetCover<T>& tsc) {
	return os << static_cast<const SetCover&>(tsc);
}

}