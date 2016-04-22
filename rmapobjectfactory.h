#ifndef RMAPOBJECTFACTORY_H
#define RMAPOBJECTFACTORY_H

class RTable;
class RSection;
class QWidget;

//--------------------------------------------------------------------------------------
class RMapObjectFactory {
public:
    RMapObjectFactory();
	virtual ~RMapObjectFactory();

	virtual RSection *createSection(QWidget *parent) const = 0;
	virtual RTable *createTable(RSection *parentSection) const = 0;
};

//--------------------------------------------------------------------------------------
class RMapObjectSimpleFactory : public RMapObjectFactory {
public:
	virtual RSection *createSection(QWidget *parent) const;
	virtual RTable *createTable(RSection *parentSection) const;
};

//--------------------------------------------------------------------------------------
class RMapObjectGridFactory : public RMapObjectFactory {
public:
	RMapObjectGridFactory(int size);

	virtual RSection *createSection(QWidget *parent) const;
	virtual RTable *createTable(RSection *parentSection) const;

private:
	int gridSize;
};

#endif // RMAPOBJECTFACTORY_H
