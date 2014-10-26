#include "ExtractorModule.h"
#include "Common.h"
#include "BlkDsp.h"
#include "AudioAnalysis.h"


namespace libsch{

ExtractorModule::ExtractorModule(uint winlength, size_t datalength, std::string &id,
				BaseModule *parent)
    : BaseModule(id, parent)
    , m_winLength(winlength)
{
    if (winlength > 8192)
    {
        dbg_prt((std::string(__func__)+" Supplied window length of: " + 
                    std::to_string(winlength) + " > 8192, using 8192 instead.").c_str());
        m_winLength = winlength = 8192;
    }

    init(datalength, winlength);
}

//! Generate half-Hann window and save it off.
void ExtractorModule::init(int dl, int wl)
{

    //Initialize data arrays.
    size_t datsz = icstdsp::BlkDsp::nexthipow2(dl+wl);
    InDataLength(datsz);
    OutDataLength(dl);

    //Generate and save upper (right) half of Hann window.
    m_window = new float[datsz];
    memset(m_window, 0, datsz*sizeof(float));
    
    //Calc a full period of Hann window.
    uint dblWinLen = wl << 1;
    float *twin = new float[dblWinLen];
    icstdsp::BlkDsp::hann(twin, dblWinLen);
    
    //Take the right half of the full period.
    float *start = &(twin[wl-1]);
    memcpy(m_window, start, wl*sizeof(float));


    delete [] twin;
}

ExtractorModule::~ExtractorModule(void)
{
    delete [] m_window;
}


void ExtractorModule::DoUpdate()
{
//    dbg_prt((std::string("DoUpdate called on module: ") + id).c_str());

    size_t inlength = InDataLength();
    size_t outlength = OutDataLength();

    //    float *twin = new float[inlength];
    //    memcpy(twin, m_window, m_winLength*sizeof(float));
    //
    //
    //    icstdsp::BlkDsp::abs(_invec,outlength);
    //    icstdsp::BlkDsp::fconv(_invec, twin, outlength, m_winLength);
    //    float *start = 	&(_invec[(m_winLength-1)>>1]);  //div by 2.

    float c = 0.0f;
    icstdsp::AudioAnalysis::envelope(_invec, _outvec, c, outlength, 200.0f, 2000.0f, 0);

   // normTo1(_outvec, _outvec, outlength);

//    delete [] twin;
}


} /*namespace libsch*/
