#include "object.h"
#include "transformation.h"

class Operation
{
public:

	vector<Object*> targets;
	Transformation* transformation;

	void apply();

	void rollback();

};