#include "RtAudioFeeder.h"


#include <iostream>
#include <fstream>




namespace libsch
{
/************************************************************************/
/*          RTAudio Static Callback                                     */
/************************************************************************/
int RtAudioFeeder::output( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *userdata )
{
    UserData *udat = static_cast<UserData*>(userdata);
    static const sample_t *data = udat->pcmDataPtr;
    static unsigned int deliveredFrames = 0;
    sample_t *rtdata = static_cast<sample_t *>(outputBuffer);
    int rval=0; 

    //returning 1 will cause RtAudio to play the rest of its buffer and stop.
    if (deliveredFrames + nBufferFrames > udat->totalFrames) {
        nBufferFrames = udat->totalFrames - deliveredFrames;
        rval=1;
    }

    deliveredFrames += nBufferFrames;

    while (nBufferFrames-- > 0) for (int i=0; i<udat->numChannels; ++i)
        *rtdata++ = *data++;
    
    
    return rval;
}

/************************************************************************/
/*          RtAudioFeeder Methods                                       */
/************************************************************************/

RtAudioFeeder::RtAudioFeeder(const std::string& fname, unsigned int bufSize)
    : m_dac(NULL)
    , m_udat()
    , m_sndFile(fname)
    , m_currentlyPlaying(false)
//    , m_curSampIdx(0)
    , m_rtBufferSize(bufSize)
{
    dbg_prt(__func__); 
}


RtAudioFeeder::~RtAudioFeeder()
{

    dbg_prt(__func__); 
	CloseStream(true); //releases stream data, deletes m_dac.
}

// Fill pcm data buffer from file, init the userdata struct used in the
// RtAudio callback function.
SCH_RESULT RtAudioFeeder::Init()
{
    m_sndFile.OpenFileAndFillDataBuffer();
	//m_udat.pcmDataPtr = m_sndFile.GetPCMDataBuffer();
	//m_udat.myself = this;
    //m_udat.totalFrames = m_sndFile.NumFrames();
    return SCH_OK;
}

SCH_RESULT RtAudioFeeder::OpenStream()
{
    SCH_RESULT result = SCH_OK;
    result = Init();
    if (! m_sndFile.HasDataReady()) {
        dbg_prt( "Tried to open RtAudio stream, but a sound file has "
                "not been opened yet, so there is no data to play." ); 
        return SCH_ERR_FILE_NOT_OPEN;
    }
    try {
        if (NULL == m_dac) {
            m_dac = new RtAudio();
            printDeviceInfo();
            m_dac->showWarnings(true);
        }

        if (m_dac->isStreamOpen()) {
            dbg_prt( "RtAudioFeeder::OpenStream(): Tried to open stream, "
                    "but it was already open, please close the stream first." );
            result = SCH_ERR_RTAUDIO;
        }
        else if (m_dac->getDeviceCount() < 1) {
            dbg_prt( "RtAudioFeeder::OpenStream(): No usable devices for RtAudio!" );
            result = SCH_ERR_RTAUDIO;
        }
        else {
            RtAudio::StreamParameters outParams;
            outParams.deviceId = m_dac->getDefaultOutputDevice();
            outParams.firstChannel = 0;
            outParams.nChannels = m_sndFile.NumChannels();

            m_udat.myself = this;
            m_udat.pcmDataPtr = m_sndFile.GetPCMDataBuffer();
            m_udat.totalFrames = m_sndFile.NumFrames();
            m_udat.numChannels = m_sndFile.NumChannels();

            unsigned int dl = static_cast<unsigned int>(m_rtBufferSize);

            m_dac->openStream(
                    &outParams, 
                    NULL, 
                    RTAUDIO_FLOAT32, 
                    m_sndFile.SampleRate(),
                    &dl, 
                    RtAudioFeeder::output, 
                    (void*) &m_udat);
        }
    }
    catch (RtError &e) {
        dbg_prt( e.getMessage().c_str() );
        result = SCH_ERR_RTAUDIO;
    }
    return result;
}

SCH_RESULT RtAudioFeeder::CloseStream(bool release)
{
    SCH_RESULT result = SCH_OK;
    try {
		if (NULL != m_dac) {
			m_dac->closeStream();
            if (true == release) {
			    delete m_dac;
			    m_dac=NULL;
		    }
        }
        m_currentlyPlaying=false;
    }
    catch (RtError &e) {
        dbg_prt(e.getMessage().c_str());
        result = SCH_ERR_RTAUDIO;
    }
    
    return result;
}

void RtAudioFeeder::CloseAndReleaseResources()
{
    CloseStream(true); 
    m_sndFile.ReleasePCMData();
}

void RtAudioFeeder::PCMDataAtTime(sample_t *out, unsigned int nframes)
{
    uint samplepos = static_cast<uint>(m_sndFile.SampleRate()*StreamTime());
    const sample_t *src = m_sndFile.GetPCMDataBuffer() + samplepos;
    nframes <<= m_sndFile.NumChannels();
    while (nframes-- > 0) *out++ = *src++;
}

unsigned int RtAudioFeeder::PCMDataTotalSamples() const
{
    return m_sndFile.TotalSamples();
}

double RtAudioFeeder::StreamTime() const
{
    if (IsReady())
        return m_dac->getStreamTime();
    else
        return -1.0;
}

double RtAudioFeeder::StreamLengthSeconds() const 
{
    return m_sndFile.TotalSeconds();
}

unsigned int RtAudioFeeder::Fs() const
{
	return m_sndFile.SampleRate();
}

unsigned int RtAudioFeeder::NumChannels() const
{
    return m_sndFile.NumChannels();
}

void RtAudioFeeder::StartStream()
{
    m_dac->startStream();
    m_currentlyPlaying=true;
}

void RtAudioFeeder::StopStream()
{
    m_dac->stopStream();
    m_currentlyPlaying=false;
}

bool RtAudioFeeder::IsReady() const
{
    return m_dac->isStreamRunning();
}

/*****************************************************************************
 *        PRIVATE METHODS
 ****************************************************************************/
void RtAudioFeeder::printDeviceInfo()
{
    unsigned int numDevs = m_dac->getDeviceCount();
    std::cout << "Devices: " << numDevs << '\n';
    for (uint i=0; i<numDevs; ++i) {
        RtAudio::DeviceInfo devInfo = m_dac->getDeviceInfo(i);
        std::cout << "---------(" << i << ")---------" << '\n';
        std::cout << "Name: " << devInfo.name << '\n';
        std::cout << "Probed: " << devInfo.probed << '\n';
        std::cout << "Output Channels: " << devInfo.outputChannels << '\n';
        std::cout << "Input Channels: " << devInfo.inputChannels << '\n';
        std::cout << "Is Default Output: " << devInfo.isDefaultOutput << '\n';
        std::cout << "---------------------" << std::endl;
    }
}
//void RtAudioFeeder::DoUpdate()
//{
//    dbg_prt_verbose("RtAudioFeeder: DoUpdate called on module: " << QString(id.c_str()));
//	
//    int curSamp = m_dac->getStreamTime() * m_sndFile.SampleRate(); //m_fs;
//    
//	memcpy(_outvec, &m_pcmData[curSamp], _outLength*sizeof(realval_t));
//    m_curSampIdx = curSamp + _outLength;
//    //m_curSampIdx = curSamp;
//}

};
