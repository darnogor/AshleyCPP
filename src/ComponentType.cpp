/*******************************************************************************
 * Copyright 2014, 2015 See AUTHORS file.
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

#include <cstdint>

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

#include "Ashley/core/ComponentType.hpp"
#include "Ashley/core/Entity.hpp"
#include "Ashley/AshleyConstants.hpp"

uint64_t ashley::ComponentType::typeIndex = 0;
std::unordered_map<std::type_index, ashley::ComponentType> ashley::ComponentType::componentTypes;

ashley::ComponentType::ComponentType() :
		        index(typeIndex++) {
}

ashley::ComponentType& ashley::ComponentType::getFor(std::type_index index) {
	// if the given index doesn't exist it's created by the container
	return componentTypes[index];
}

uint64_t ashley::ComponentType::getIndexFor(std::type_index index) {
	return ashley::ComponentType::getFor(index).getIndex();
}

ashley::BitsType ashley::ComponentType::getBitsFor(std::initializer_list<std::type_index> components) {
	ashley::BitsType retVal;
	std::for_each(components.begin(), components.end(),
	        [&](std::type_index i) {retVal[ashley::ComponentType::getIndexFor(i)] = true;});

	return retVal;
}

bool ashley::ComponentType::operator ==(const ashley::ComponentType &other) const {
	return this->index == other.getIndex();
}

bool ashley::ComponentType::operator !=(const ashley::ComponentType &other) const {
	return this->index != other.getIndex();
}
