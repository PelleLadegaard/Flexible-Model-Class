#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <assert.h>

#include "Defines.h"
#include "Enums.h"
#include "Globals.h"

class Vertex;
// Model class, storing all the data of a model, which attributes define the model, and the size of a single vertex in bytes
class Model {
public:
	// Create a model and specify which attribute types define the model and how many vertices it must have
	Model(const std::vector<Attribute>& attributes, uint vertCount);
	// Copy constructor
	Model(const Model &other);
	// Frees model data on destruction
	~Model();
	// Assignment constructor
	Model& operator = (const Model &other);
	// Move constructor
	Model& operator=(Model&& other);
	// When accessing the model using the [] operator (which specifies what vertex you want to access), return an object holding the data of said vertex
	Vertex operator[](size_t idx);
	// Retrieve a void pointer to the start of the data. This can be used to copy the data over to another Model object, or to assign it to an OpenGL VBO
	void* data();
	// Retrieve the amount of vertices found in the model
	size_t get_size();
	// Retrieve a constant reference telling which vertex attributes can be found in the model's vertices
	const std::vector<Attribute>& get_attributes();

private:
	char* m_data = nullptr;
	std::vector<Attribute> m_attributes;
	std::map<Attribute, size_t> m_attrLocs;
	size_t m_vertexSize;
	size_t m_vertexCount;
};

class VertexElement;
// Vertex class, holding a pointer that points towards the starting position of its data in the model data buffer
class Vertex {
	friend Model;
public:
	// When accessing the model using the [] operator (which specifies what vertex element you want to access), return an object holding the date of said vertex element
	VertexElement operator[](Attribute attrib);

private:
	char* m_data;
	std::map<Attribute, size_t>* m_attrLocs;
};

// Vertex element class, holding a pointer that points towards the starting position of its data in the model data buffer
class VertexElement {
public:
	// When assigning a value to the vertex element, store the value inside of the model data buffer
	template <class T>
	VertexElement& operator=(T value) {
		assertm(typeid(T).hash_code() == AttributeData::get_type(m_attr), "Incorrect type in attribute writing");
		if (this != (VertexElement*)&value) {
			*(T*)m_data = value;
		}
		return *this;
	}

	// When assigning the vertex element to an object, cast the vertex element's data to the object's type and assign it to the object
	template <class T>
	operator T() const {
		assertm(typeid(T).hash_code() == AttributeData::get_type(m_attr), "Incorrect type in attribute reading");
		return *reinterpret_cast<T*>(m_data);
	}

	char* m_data;
	Attribute m_attr;
};