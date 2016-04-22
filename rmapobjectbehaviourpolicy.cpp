#include "rmapobjectbehaviourpolicy.h"

//--------------------------------------------------------------------------------------
RMapObjectBehaviourPolicy::RMapObjectBehaviourPolicy(RMapObject *obj /* = 0 */) :
	currentMapObject(obj)
{}

//--------------------------------------------------------------------------------------
RMapObjectBehaviourPolicy::~RMapObjectBehaviourPolicy()
{}

//--------------------------------------------------------------------------------------
RMapObject *RMapObjectBehaviourPolicy::mapObject() const
{ return currentMapObject; }

//--------------------------------------------------------------------------------------
void RMapObjectBehaviourPolicy::setMapObject(RMapObject *obj)
{ currentMapObject = obj; }
