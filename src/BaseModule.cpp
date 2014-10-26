/*
 * BaseModule.cpp

 *
 *  Created on: Jul 6, 2012
 *      Author: jim
 */

#include "BaseModule.h"
#include <assert.h>


namespace libsch
{


BaseModule::BaseModule(BaseModule *parent) 
    : BaseModule("unknown", parent)
{ }

BaseModule::BaseModule(const std::string &identifier, BaseModule *parent) 
    : BaseModule(0, identifier, parent)
{ }

BaseModule::BaseModule(size_t datalength, const std::string &identifier, BaseModule *parent) 
    : BaseModule(datalength, datalength, identifier, parent)
{ }

BaseModule::BaseModule( size_t inLength, size_t outLength, const std::string &identifier, BaseModule *parent/*=0*/ )
	: parent(parent)
	, id(identifier)
    , _invec(NULL)
    , _outvec(NULL)
    , _dataLength(outLength)
	, _inLength(inLength)
	, _outLength(outLength)
    , m_max(0.f)
{
    dbg_prt((std::string(__func__) + ": Created module: " + identifier).c_str());

	InDataLength(inLength);
    OutDataLength(outLength);

	std::string parentId = Parent() == NULL ? "no parent" : Parent()->Id();

    dbg_prt((std::string(__func__) + ": Init module: " + parentId + "<--" + Id()).c_str());

    if (parent != NULL)
    {
        //TODO: add check for already existing child in vector.
        this->parent->AddChild(this);
//        connect(parent, SIGNAL( UpdateChildren(realval_t *) ),
//                this,   SLOT( Update(realval_t *) ));
    }
}

BaseModule::~BaseModule()
{

	dbg_prt((std::string(__func__) + ": Cleaning up: " + id).c_str());

	for (BaseModuleIterator it = children.begin(); it != children.end(); it++)
	{
		delete *it;
	}
	if (NULL != _invec)  delete [] _invec;
	if (NULL != _outvec) delete [] _outvec;
}


const BaseModule *BaseModule::Parent() const
{
    return parent;
}


const BaseModule::BaseModuleVector& BaseModule::Children() const
{
    return children;
}


//TODO: need to disconnect old signal!
SCH_RESULT BaseModule::AddParent(BaseModule *p)
{
    SCH_RESULT rval = SCH_OK;
	if (p == this)
	{
		dbg_prt((std::string(__func__) + ": The parent you tried to add" 
            " was equal to itself. No change made." + id).c_str());
        rval = SCH_OK_PARENT_NOT_ADDED;
	}
    else
    {
        this->parent = p;
//        connect(p, SIGNAL( UpdateChildren(realval_t *) ), 
//                this, SLOT( Update(realval_t *) ));
    }
    return SCH_OK;
}

SCH_RESULT BaseModule::AddChild(BaseModule *child)
{
    SCH_RESULT rval = SCH_OK;
	if (child == this)
	{
		dbg_prt((std::string(__func__) + ": The child you tried to add" 
            " was equal to itself." + id).c_str());
		return SCH_ERR_ADD_CHILD;
	}

	BaseModuleIterator it;
	for (it = children.begin(); it != children.end(); it++)
	{
		if (*it == child)
		{
			dbg_prt((std::string(__func__) + ": Child: " + child->Id() + 
                        " already exists. Ignoring.").c_str());
			return SCH_OK_CHILD_EXISTS;
		}
	}
    children.push_back(child);
	return rval;
}



//TODO: should check for failed allocations here and return -1 on failure.
size_t BaseModule::DataLength(size_t sz)
{
    InDataLength(sz);
	OutDataLength(sz);
	_dataLength = sz;
	return sz;
}

const std::string& BaseModule::Id() const
{
    return id;
}

//!reallocate memory
SCH_RESULT BaseModule::InDataLength(size_t inSz)
{

    dbg_prt((std::string(__func__) + ": Creating invec size: " + 
                std::to_string(inSz) + " " + id).c_str());

    SCH_RESULT rval = SCH_OK;

	if (NULL == _invec)
	{
		_invec = new realval_t[inSz];
	}
	else
	{
		delete [] _invec;
		_invec = new realval_t[inSz];
	}
    memset(_invec,0,inSz*sizeof(realval_t));
    _inLength = inSz;
	return rval;
}

//!reallocate memory
SCH_RESULT BaseModule::OutDataLength(size_t outSz)
{

    dbg_prt((std::string(__func__) + ": Creating outvec size: " + 
                std::to_string(outSz) + " " + id).c_str());

    SCH_RESULT rval = SCH_OK;

	if (NULL == _outvec)
	{
		_outvec = new realval_t[outSz];
	}
	else
	{
		delete [] _outvec;
		_outvec = new realval_t[outSz];
	}
    memset(_outvec,0,outSz*sizeof(realval_t));
	_outLength = outSz;
    return rval;
}

void BaseModule::UpdateChildren(realval_t *input)
{
    for(auto &child : children) {
        child->Update(input);
    }
}

/**
 *  Does a memcpy of the supplied data from *in, and puts into _invec.
 *  Then _invec is worked on, with the results placed into _outvec (that
 *  work should be done in DoUpdate()).
 *  \c input should be null if this module is the pipeline head.
 */
void BaseModule::Update(realval_t *input)
{
    dbg_prt(("BaseModule: Update called on module: " + id).c_str());

    if (input != NULL) {
        memcpy(_invec, input, parent->OutDataLength()*sizeof(realval_t));
    }

    DoUpdate(); //DoUpdate() should copy data into _outvec.
    //emit UpdateChildren(_outvec);
}

}; /* namespace libsch */
