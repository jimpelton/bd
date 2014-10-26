
#ifndef SoundFile_h__
#define SoundFile_h__

#include "BdTypes.h"
#include "prt_dbg.h"

#include <sndfile.h>
#include <iostream>
#include <string.h>
#include <string>

namespace libsch 
{

class SoundFile
{
private:
    std::string filename;
    SF_INFO sfInfo; 
    float *pcmData;

public:
    SoundFile(std::string const &fileName) 
        : filename(fileName)
        , pcmData(NULL) 
    {
        dbg_prt(__func__); 
    }


    SoundFile(SoundFile const &other)
    {
        dbg_prt(__func__); 
        filename = other.filename;
        sfInfo = other.sfInfo;
        pcmData = other.pcmData;
    }

    ~SoundFile() 
    {
        dbg_prt(__func__); 
        ReleasePCMData();
    }

    SCH_RESULT OpenFileAndFillDataBuffer()
    {
        SCH_RESULT rval = SCH_OK;
        if (NULL != pcmData ) { return SCH_ERR_FILE_ALREADY_OPEN; }
        SNDFILE *sfile;
        SF_INFO sfinfo;
        
        memset(&sfinfo, 0, sizeof(SF_INFO));
        sfile = sf_open(filename.c_str(), SFM_READ, &sfinfo);
        if (NULL == sfile) return SCH_ERR_FILE_NOT_OPEN;

        this->sfInfo = sfinfo;

        
        sf_count_t totalFrames = sfinfo.frames * sfinfo.channels;
        pcmData = new float[totalFrames];

        sf_count_t read = sf_read_float(sfile, pcmData, totalFrames);
        std::cout << "Read: "   << read          << " samples.\n"  ;
        std::cout << "Frames: " << sfinfo.frames << " frames.\n"  ;
        std::cout << "Sample rate: "  << sfinfo.samplerate             << '\n'; 
        std::cout << "Channels: "     << sfinfo.channels               << '\n';
        std::cout << "Format: 0x"     << std::hex << sfinfo.format     << '\n';
        std::cout << "Sections: "     << std::dec << sfinfo.sections   << '\n';
        std::cout << "Seekable: "     << sfinfo.seekable               << std::endl;

        sf_close(sfile);
        return rval;
    }

    void ReleasePCMData()
    {
    
        dbg_prt(__func__); 
        if (NULL != pcmData)
        {
            dbg_prt("Deleting PCM data array" ); 
            delete [] pcmData;
            pcmData = NULL;
        }
    }

    bool HasDataReady() const
    {
        return NULL != pcmData;
    }
     
    unsigned int SampleRate() const 
    {
        return sfInfo.samplerate;
    }

    unsigned int NumChannels()  const 
    {
        return sfInfo.channels;
    }

    unsigned int NumFrames() const 
    {
        return sfInfo.frames;
    }

    unsigned int TotalSamples() const
    {
        return NumChannels() * NumFrames();
    }

    double TotalSeconds() const
    {
        return NumFrames() / static_cast<double>(SampleRate());
    }

    sample_t const * GetPCMDataBuffer() const 
    {
        return pcmData;
    }


}; /* SoundFile */
} /* libsch */
#endif /* SoundFile_h__ */

