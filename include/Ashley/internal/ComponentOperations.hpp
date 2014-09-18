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

#ifndef COMPONENTOPERATIONS_HPP_
#define COMPONENTOPERATIONS_HPP_

#include <memory>

#include "Ashley/util/ObjectPools.hpp"

namespace ashley {
class Component;
class Entity;
/**
 * <p>Interface representing possible actions on {@link Component}s with respect to {@link Entity}s.</p>
 *
 * <p>Internal class; you probably don't need this.</p>
 */
class ComponentOperationHandler {
public:
	virtual ~ComponentOperationHandler() {
	}

	virtual void add(ashley::Entity *entity,
			std::shared_ptr<ashley::Component> component) = 0;
	virtual void remove(ashley::Entity *entity,
			std::shared_ptr<ashley::Component> component) = 0;
};

/**
 * <p>Struct representing all the details required about a component operation.</p>
 *
 * <p>Internal class; you probably don't need this.</p>
 */
struct ComponentOperation : public ashley::Poolable {
	enum class Type {
		ADD, REMOVE, NONE
	};

	Type type;

	ashley::Entity *entity = nullptr;
	std::shared_ptr<ashley::Component> component = nullptr;

	ComponentOperation() :
			type(Type::NONE) {
	}
	virtual ~ComponentOperation() {
	}

	inline void makeAdd(ashley::Entity *entity,
			std::shared_ptr<ashley::Component> component) {
		this->type = Type::ADD;

		this->entity = entity;
		this->component = component;
	}

	inline void makeRemove(ashley::Entity *entity,
			std::shared_ptr<ashley::Component> component) {
		this->type = Type::REMOVE;

		this->entity = entity;
		this->component = component;
	}

	void reset() override {
		entity = nullptr;
		component = nullptr;
		this->type = Type::NONE;
	}
};

}

#endif /* COMPONENTOPERATIONS_HPP_ */
