#include "rmapobjectfactory.h"
#include "rtable.h"
#include "rsection.h"
#include "rmapobjectmovepolicy.h"
#include "rmapobjectresizepolicy.h"

//--------------------------------------------------------------------------------------
RMapObjectFactory::RMapObjectFactory()
{}

//--------------------------------------------------------------------------------------
RMapObjectFactory::~RMapObjectFactory()
{}


//--------------------------------------------------------------------------------------
RSection *RMapObjectSimpleFactory::createSection(QWidget *parent) const
{
	RSection *section = new RSection(parent);
	section->setMovePolicy(new RMapObjectSimpleMovePolicy);
	section->setResizePolicy(new RMapObjectSimpleResizePolicy);
	section->setGeometry(0, 0, 300, 300);
	return section;
}

//--------------------------------------------------------------------------------------
RTable *RMapObjectSimpleFactory::createTable(RSection *parentSection) const
{
	RTable *table = new RTable(parentSection);
	table->setMovePolicy(new RMapObjectSimpleMovePolicy);
    table->setResizePolicy(new RMapObjectNoResizePolicy);
	table->setGeometry(0, 0, 50, 50);
	return table;
}




//--------------------------------------------------------------------------------------
RMapObjectGridFactory::RMapObjectGridFactory(int size) :
	gridSize(size)
{}

//--------------------------------------------------------------------------------------
RSection *RMapObjectGridFactory::createSection(QWidget *parent) const
{
	RSection *section = new RSection(parent);
	section->setMovePolicy(new RMapObjectGridMovePolicy(gridSize));
    section->setResizePolicy(new RMapObjectGridResizePolicy(gridSize));
	section->setGeometry(0, 0, 10 * gridSize, 10 * gridSize);
	return section;
}

//--------------------------------------------------------------------------------------
RTable *RMapObjectGridFactory::createTable(RSection *parentSection) const
{
	RTable *table = new RTable(parentSection);
	table->setMovePolicy(new RMapObjectGridMovePolicy(gridSize));
    table->setResizePolicy(new RMapObjectNoResizePolicy);
	table->setGeometry(0, 0, 5 * gridSize, 5 * gridSize);
	return table;
}
