

#ifndef BASEMODULE_H_
#define BASEMODULE_H_

#include "BdTypes.h"
#include "Export.h"
#include "prt_dbg.h"

#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <string.h>

#ifdef _WIN32
#define __func__ __FUNCTION__
#endif




namespace libsch {
    /*!

    \class  BaseModule BaseModule.h

    \brief  The basic module type for the beat detection pipeline.

    This is an abstract class so you need to instantiate an
    extending class that implements DoUpdate().
    Defines a parent-child module relationship as illustrated:

    Signal:                Socket:
    ---------------------------------
    UpdateChildren() ----> Update()
    ----------             ---------
    | PARENT | ----------> | CHILD |
    ----------             ---------
    &_outvec               &_invec
    ----------------------------------

    The contents of the parent's _outvec are memcpy'ed to _invec.
    There could be many children for 1 parent, but each child may have
    only 1 parent.

    */
    class DllExport BaseModule {

    public:        
    /*!
      \typedef typedef std::vector<BaseModule*> BaseModuleVector;
      \brief vector type for holding child BaseModules of a parent.
     */
    typedef std::vector<BaseModule*> BaseModuleVector;

    /*!
      \typedef typedef std::vector<BaseModule*>::iterator BaseModuleIterator;
      \brief Iterator over a std::vector of BaseModule*.
     */
    typedef std::vector<BaseModule*>::iterator BaseModuleIterator;

    public:
        /*! \fn BaseModule(BaseModule *parent = 0);
        *   \brief Does not allocate any memory for the data arrays. 
        * 
        *  This is useful when extra calculations need to be done to calculate 
        *  the correct data array size.
        *  When using this constructor be sure that _outvec and _invec are both
        *  allocated (use DataLength()) before using the module.
        *  
        *  Sets id to "unknown"
        *  
        *  \param parent the parent of this module
        */
        explicit BaseModule(BaseModule *parent = 0);

        /*! \fn BaseModule::BaseModule(std::string &identifier, BaseModule *parent=0);
        *   \brief Does not allocate any memory for data arrays, but sets the identifier.
        *
        *   \param identifier
        *   \param parent
        *   \see   BaseModule()
        */
        BaseModule(const std::string &identifier, BaseModule *parent=0);

        /*!
        *  \brief Make a base module with these settings.
        *
        *  \param dataLength the length of the working data for the module
        *  \param identifier a unique string to id this module
        *  \param parent the parent of this module, defaults to NULL.
        */
        BaseModule(size_t inOutLength, const std::string &identifier, BaseModule *parent = 0);

		/*!
		 *  \brief Make a base module with different inLength and outLengths.
		 *         Useful for multi data child outputs when the child data are
		 *         concatenated sequentially in the output vector.
		 */
		BaseModule(size_t inLength, size_t outLength, const std::string &identifier, BaseModule *parent=0);
        
		/**
        * Deleteyyy!!!
        */
        virtual ~BaseModule();


        /*!
        * \brief Make parent this module's parent. This also means that
        *        this module will respond to the UpdateChildren() signal
        *        from that parent.
        *
        * NOTE: AddParent, as implemented right now, does not disconnect the signal from
        * the current parent module. This means that this module will react to signals
        * from the old parent. I suppose the disconnect functionallity should
        * be added...some day...
        * 
        * \param parent the parent to add.
        */
        virtual SCH_RESULT AddParent(BaseModule *parent);

        /*!
        * \brief Add a new child to this module.
        *
        * Currently, AddChild does not check if the child already exists in the
        * list of children. You've been warned!
        *
        * \param child The child to add.
        */
        virtual SCH_RESULT AddChild(BaseModule *child);

        /*!
        * \brief Get this modules parent.
        * 
        * Currently a module may only have one parent.
        * 
        * \return A const reference to the parent.
        */
        const BaseModule* Parent() const;

        /*!
        * \brief  Get this modules children as a BaseModuleVector (a std::vector<BaseModule*>)
        *
        * \see    BaseModuleVector()
        * \return The const reference to a BaseModuleVector that is a list of children of this module.
        */
        const BaseModuleVector& Children() const;

		/*!
		* \brief Return pointers to the \c _invec and \c _outvec data arrays.
		*/
		inline realval_t* InVec() const;
		inline realval_t* OutVec() const;
		
		/*!
		 *  \brief Sets the length of the buffer(s) for this module, and allocates
         *         new memory for both invec and outvec. Both are allocated
         *         for the same amount of memory.
		 * 
         *         This could be overridden to allocate space in a module's 
		 *         buffers if a different method is needed.
		 * 
		 * \return The size of the buffer
		 */
        virtual size_t DataLength(size_t sz);

		/*!
		 * \brief Get the size of the buffer length.
		 * \deprecated
		 * 
		 * \return The buffer length.
		 */
        //virtual size_t DataLength();

		inline size_t InDataLength() const;
		inline size_t OutDataLength() const;
		virtual SCH_RESULT InDataLength(size_t inSz);
		virtual SCH_RESULT OutDataLength(size_t outSz);

        /*!
        * \brief Get a reference to this modules string identifier. 
        *
        * The identifier is useful for, among other things, to id the 
        * module in debugging output.
        *
        * \return the modules string identifier.
        */
        const std::string& Id() const;

        /*!
         *   \brief In-lined normalization to range of 0..1.
         *          d[0..sz-1] --> r[0..size-1]/max(d)
         *
         *   \param d Data array
         *   \param r Result array (normalized values)
         *   \param sz The size of d and r.
         */
        inline void normTo1(float *dst, float *src, size_t sz);

        inline void findMax(float &max, float const *src, size_t sz);

        void UpdateChildren(realval_t *);

        /*!
        * \brief Called when this module should update it's children. 
        * 
        * In short, this Update slot is executed if this modules parent emits 
        * the \c UpdateChildren() signal. In turn, this module will then do some 
        * processing on the data contained in \c in and emit \c UpdateChildren().
        *
        * NOTE: Emits the \c UpdateChildren(t_relvec) signal.
        *
        * \param in The new data input, needing processing.
        */
        virtual void Update(realval_t *in);

    protected:
        /*!
        *  Pointer to one and only parent for this module.
        */
        BaseModule   *parent;

        /*!
        * An std::vector of children
        */
        BaseModuleVector      children;
        std::string  id;

		realval_t    *_invec;
		realval_t    *_outvec;

        /*!
        *  Length of the data arrays in samples.
        *  Recommend using _inLength and _outLength.
        */
		size_t _dataLength;
		
		//! Length of the input data array in samples
        size_t _inLength;
		//! Length of the output data array in samples.
		size_t _outLength;

		float m_max;

        /*!
        * DoUpdate() should contain the business/processing logic for this
        * module.
        */
        virtual void DoUpdate() = 0;

    
}; /* BaseModule */



    /************************************************************************/
    /*       INLINE DEFINITIONS                                             */
    /************************************************************************/

    inline realval_t* BaseModule::InVec() const
	{
		return _invec;
	}

    inline realval_t* BaseModule::OutVec() const
	{
		return _outvec;
	}

    inline size_t BaseModule::InDataLength() const
	{
		return _inLength;
	}

    inline size_t BaseModule::OutDataLength() const
	{
		return _outLength;
	}

	//! dst[0..sz-1] = src[0..sz-1]/fabs(max(d))
    inline void BaseModule::normTo1(float *dst, float *src, size_t sz)
    {
        m_max = ::fabs(src[0]);
        for (size_t idx = 1; idx < sz; idx++)
        {
            float abs_src = ::fabs(src[idx]);
            m_max = (m_max < abs_src) ? abs_src : m_max;
        }

        if (m_max != 0.f)
        {
            for (size_t idx = 0; idx < sz; idx++)
            {
                dst[idx] = src[idx] / m_max;
            }
        }
    }

    inline void BaseModule::findMax(float &max, float const *src, size_t sz)
    {
        float tmax = ::fabs(src[0]);
        for (size_t idx = 1; idx < sz; idx++)
        {
            float abs_src = ::fabs(src[idx]);
            tmax = (tmax < abs_src) ? abs_src : tmax;
        }
        m_max = max = tmax;
    }

}; /* namespace libsch */


#endif /* BASEMODULE_H_ */
