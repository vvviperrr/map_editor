#ifndef RMAPOBJECTBEHAVIOURPOLICY_H
#define RMAPOBJECTBEHAVIOURPOLICY_H

class RMapObject;

class RMapObjectBehaviourPolicy {
public:
	RMapObjectBehaviourPolicy(RMapObject *obj = 0);

	virtual ~RMapObjectBehaviourPolicy();

	RMapObject *mapObject() const;
	void setMapObject(RMapObject *obj);

	virtual RMapObjectBehaviourPolicy *clone() const = 0;

private:
	RMapObject *currentMapObject;
};

#endif // RMAPOBJECTBEHAVIOURPOLICY_H
