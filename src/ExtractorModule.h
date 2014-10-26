
#ifndef EXTRACTORMODULE_H
#define EXTRACTORMODULE_H

#include "Export.h"
#include "BaseModule.h"

#include <math.h>

namespace libsch
{
    /*!
     * \class   ExtractorModule ExtractorModule.h
     * \brief   Extract the envelope of the input signal.
     * 
     *          The actual size of the input vector for ExtractorModule is
     *  		a little larger than the requested size. In fact, the number of bytes
     *  		allocated for working data is given by:
     *  		       allocated_bytes = sizeof(float)*nexthipow2(datalength+WIN_LENGTH)
     *  		This extra data is required because of the fast convolution which
     *  		is optimized for powers of two.
     *  		However, the extra samples are not made available in the output of
     *  	    this module.
     *
     *  	    The module uses the method described by Schierer.
     *  	    //TODO: provide reference to article.
     */
    class DllExport ExtractorModule : public BaseModule
    {

    public:
         /*!
         * \brief Construct a new ExtractorModule. Allocates 
         *        \c nexthipow2(winlength+datalength) sample buffers.
         *
         * In order to accommodate the convolution used
         * in window extraction, the actual memory allocated for the input array is
         * the next higher power of 2 of winlength+datalength.
         *
         * \param winlength
         * \param datalength
         * \param id
         * \param parent
         */
        ExtractorModule(uint winlength, size_t datalength, std::string &id, BaseModule *parent=0);
        ~ExtractorModule(void);

    protected:
        virtual void DoUpdate() override;

    private:
        //! half-hanning window
        float *m_window;
        uint m_winLength;
                
        void init(int datalength, int winlength);
    };



}
#endif /* EXTRACTORMODULE_H */
