#include "object.h"
#include "transformation.h"

#pragma once

class Operation
{
public:

	vector<Object*> targets;
	Transformation* transformation;

	void apply();

	void rollback();

};