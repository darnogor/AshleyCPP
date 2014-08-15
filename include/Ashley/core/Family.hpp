/*******************************************************************************
 * Copyright 2014 See AUTHORS file.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#ifndef FAMILY_HPP_
#define FAMILY_HPP_

#include <cstdint>

#include <string>
#include <bitset>
#include <typeindex>
#include <unordered_map>
#include <memory>
#include <utility>

#include "Ashley/AshleyConstants.hpp"
#include "Ashley/core/Entity.hpp"

namespace ashley {

/**
 * Represents a group of {@link Component}s. It is used to describe what {@link Entity} objects an
 * {@link EntitySystem} should process.
 *
 * Example: {@code Family.getFor<PositionComponent, VelocityComponent>()}
 *
 * Families can't be instantiated directly but must be accessed via {@code Family.getFor()}, this is
 * to avoid duplicate families that describe the same components.
 *
 * <em>Java author: Stefan Bachmann</em>
 * @author Ashley Davis (SgtCoDFish)
 */
class Family {
private:
	// used to hide the constructor as private while still allowing shared_ptr of Family.
	struct use_getFor_not_constructor {
	};
	static use_getFor_not_constructor constructorHider_;

public:
	/**
	 * @return The family matching the specified {@link Component} type_indexes. Each set of component types will
	 * always return the same Family instance. The types are specified in an initializer_list of type_index types.
	 */
	static std::shared_ptr<ashley::Family> getFor(std::initializer_list<std::type_index> types);

	/**
	 * @return The family matching the specified {@link Component} type_index. Each component type will
	 * always return the same Family instance.
	 */
	static std::shared_ptr<ashley::Family> getFor(std::type_index index);

	/**
	 * <p>Returns a family with the passed {@link Component} classes as a descriptor. Each set of component types will
	 * always return the same Family instance.</p>
	 *
	 * @param all entities will have to contain all of the components in the set. See {@link ComponentType#getBitsFor}.
	 * @param one entities will have to contain at least one of the components in the set.See {@link ComponentType#getBitsFor}.
	 * @param exclude entities cannot contain any of the components in the set. See {@link ComponentType#getBitsFor}.
	 * @return The family
	 */
	static std::shared_ptr<ashley::Family> getFor(ashley::BitsType all, ashley::BitsType one,
			ashley::BitsType exclude);

	/**
	 * @return This family's unique index
	 */
	inline uint64_t getIndex() const {
		return index;
	}

	/**
	 * @return Whether the entity matches the family requirements or not
	 */
	bool matches(ashley::Entity &entity) const;

	/**
	 * <p>Note that use_getFor_not_constructor is a private struct defined in Family; this means you cannot call this constructor and shouldn't try to.</p>
	 * <p>Use the various getFor methods to retrieve Family instances, not this.</p>
	 */
	explicit Family(const Family::use_getFor_not_constructor&, ashley::BitsType all,
			ashley::BitsType one, ashley::BitsType exclude) :
			Family(all, one, exclude) {
	}

private:
	using FamilyHashType = std::string;
	static uint64_t familyIndex;
	static std::unordered_map<FamilyHashType, std::shared_ptr<Family>> families;

	BitsType all;
	BitsType one;
	BitsType exclude;

	uint64_t index;

	Family(ashley::BitsType all, ashley::BitsType one, ashley::BitsType exclude) :
			all(all), one(one), exclude(exclude), index(familyIndex++) {
	}

	static FamilyHashType getFamilyHash(ashley::BitsType all, ashley::BitsType one,
			ashley::BitsType exclude);
};

}

#endif /* FAMILY_HPP_ */
