/*
 * ObjectPools.hpp
 *
 * Created on: 17 Sep 2014
 * Author: Ashley Davis (SgtCoDFish)
 */

#ifndef OBJECTPOOLS_HPP_
#define OBJECTPOOLS_HPP_

#include <stack>
#include <utility>

namespace ashley {

/**
 * <p>Interface implemented by classes wishing to be poolable; i.e. have an ObjectPool manage a pool
 * of instances of the class to avoid dynamic allocation.</p>
 */
class Poolable {
public:
	virtual ~Poolable() {
	}

	/**
	 * <p>Called when an object is "free"ed by the pool, to reset all the arguments to their default state.
	 */
	virtual void reset() = 0;
};

/**
 * <p>Implements an object pool which allocates a number of objects using new and stores them for retrieval using
 * obtain.</p>
 */
template<typename T> class ObjectPool {
private:
	std::stack<T*> pool;

	int64_t peakEntities;

	void createObject() {
		peakEntities++;
		pool.emplace(new T);
	}

public:
	ObjectPool(int64_t startEntities = 100) :
			peakEntities(0) {
		if (startEntities < 1) {
			throw std::invalid_argument("startEntities must be greater than or equal to 1!");
		}

		for (int i = 0; i < startEntities; i++) {
			createObject();
		}
	}

	virtual ~ObjectPool() {
		while (pool.size() > 0) {
			T *obj = pool.top();
			pool.pop();

			if (obj != nullptr) {
				delete obj;
			}
		}
	}

	/**
	 * <p>Returns a pre-allocated object from the stack. If the stack is empty, creates and returns a new object.</p>
	 * <p>Note that the returned object needs to be deleted; either call {@link ObjectPool#free} with the object to
	 * free it for later use, or delete it yourself under the knowledge that the stack will permanently be one object down.
	 * Best practise is to use free().</p>
	 * @return a pointer to an allocated object, will not be nullptr.
	 */
	T *obtain() {
		if (pool.size() == 0) {
			createObject();
		}

		T *obj = pool.top();
		pool.pop();
		return obj;
	}

	/**
	 * <p>Resets the object and returns it to the stack to be used again.</p>
	 * @param object the object, obtained from {@link ObjectPool#obtain} to be freed. Don't use it after passing it to this function.
	 */
	void free(T *object) {
		object->reset();

		pool.push(object);
	}

	/**
	 * @return the highest number of allocations ever required in this {@link ObjectPool}'s lifetime.
	 * Useful for optimising the starting value passed to the constructor.
	 */
	inline int64_t getPeakEntities() {
		return peakEntities;
	}
};

}

#endif /* OBJECTPOOLS_HPP_ */