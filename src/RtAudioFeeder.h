
#ifndef RTAUDIOFEEDER_H
#define RTAUDIOFEEDER_H

#include "BdTypes.h"
#include "Export.h"
#include "prt_dbg.h"
#include "SoundFile.h"

#include <RtAudio.h>
#include <string>

namespace libsch
{
/*!
*  \class RtAudioFeeder RtAudioFeeder.h
*  \brief Feeds the Pipeline with frames of audio data, and also feeds
*         the RtAudio subsystem with audio data.
*
*  The frame pointer value to return is determined by the time
*  in the stream as reported by the RtAudio system.
*
*  Terminology:
*    sample: 1 value from some channel in the signal.
*    frame:  1 value from 1 or more channels in the signal.
*            (or, a vector of values, sampled at time t).
*    buffer: A bunch of frames.
*/
class DllExport RtAudioFeeder 
{

public:
    /*!
      \struct struct DllExport UserData;
      \brief callback data used in the RtAudio callback function.
      */
    struct DllExport UserData
    {
        const sample_t *pcmDataPtr;  
        RtAudioFeeder *myself;
        unsigned int totalFrames;
        unsigned int numChannels;

        UserData() : pcmDataPtr(NULL), myself(NULL), totalFrames(0) {}
    };

public:
    /*!
     * \fn RtAudioFeeder();
     * \param soundFileName The path to some sound file that libsndfile will
     * open.
     * \param bufferSize The size of RtAudio's buffer.
     */
    RtAudioFeeder(const std::string& soundFileName, unsigned int bufferSize=512);
    RtAudioFeeder(const RtAudioFeeder&) = delete;
    virtual ~RtAudioFeeder();

    //! The callback for RtAudio. Reads data into outputBuffer from m_pcmData.
    static int output(void *outputBuffer, void *inputBuffer,
            unsigned int nBufferFrames, double streamTime,
            RtAudioStreamStatus status, void *userdata);

    //! Initialize the feeder object. Call immediatly after construction.
//    SCH_RESULT Init();

    /*!
     *	\fn SCH_RESULT OpenStream();
     *	\brief Open the audio file and data stream for the RtAudioFeeder. 
     *
     *	After this function is called, the PCM buffer should be full 
     *	of useful data and the stream can be started.
     */
    SCH_RESULT OpenStream();

    /*!
      \fn SCH_RESULT CloseStream();
      \brief Close the data stream for the RtAudioFeeder.
      */
    SCH_RESULT CloseStream(bool releaseRtAudio);

    /*!
     * \fn void CloseAndReleaseResources();
     * \brief Close the RtAudio stream and release the rtaudio resources.
     * Also removes the PCM data held in memory by this RtAudioFeeder.
     */
    void CloseAndReleaseResources();

    /*!
     *  \fn const realval_t* PCMDataPtr() const;
     *  \brief Get PCMData Pointer.
     *  \return A pointer to the beginning of the pcm data array.
     */
    const sample_t* PCMDataPtr() const { return m_sndFile.GetPCMDataBuffer(); }

    /*!
     * \fn const sample_t* PCMDataAtTime();
     * \brief Copy nFrames into pcmdat.
     * \param The destination array of sample_t.
     * \param Max number of frames to copy.
     * \return number of frames copied.
     *
     * Copies data into the supplied pcmdat array. The data starts
     * at the current frame in the stream.
     *
     */
    void PCMDataAtTime(sample_t* pcmdat, unsigned int nFrames);

    /*!
     * \fn unsigned int PCMDataTotalSamples() const;
     * \return total samples in sound file.
     */
    unsigned int PCMDataTotalSamples() const;

    /*!
     *  \fn double StreamTime() const;
     *  \brief Get the RtAudio time (in seconds) in the stream.
     *  \return A double that is the elapsed seconds into the stream.
     */
    double StreamTime() const;

    /*!
     * \fn double StreamLengthSeconds() const;
     * \brief Get the total running length of the stream based on number of
     *  frames in the stream.
     *
     *    total_seconds = num_frames / sample_rate
     *
     *  \return A double that is the running time of the stream.
     */
    double StreamLengthSeconds() const;


    /*!
     *  \fn unsigned int Fs() const;
     *  \brief Get the sampling frequency.
     *  \return The sampling rate as uint.
     */
    unsigned int Fs() const;

    unsigned int NumChannels() const;

    /*!
     *  \fn void StartStream();
     *  \brief Start the data stream playing.
     */
    void StartStream();

    /*!
     * \fn void StopStream();
     * \brief Stop the data stream.
     */
    void StopStream();

    //        /*!
    //         * \fn void RestartStream()
    //         * \brief Restarts the data stream at the begining.
    //         */
    //        void RestartStream();


    /*!
     * \fn bool IsReady() const; 
     * \return true is RtAudio stream is running.
     */
    bool IsReady() const; 

    //TODO: seek stream to sample smp
    //void SeekStream(unsigned int smp);

private:
    //! sheeit...
    RtAudio::StreamParameters m_oParams;

    //! makes it all possible.
    RtAudio *m_dac;

    //! RtAudio callback user data struct.
    UserData m_udat;

    //! SoundFile that reads file and holds sound info and data array.
    SoundFile m_sndFile;

    //! Playing status of rtaudio stream.
    bool m_currentlyPlaying;

    //! The array index of the current sample.
    //unsigned int m_curSampIdx; 

    //! data length of rtaudio buffer.
    size_t m_rtBufferSize;    

    //! initialize the stuff and things.
    SCH_RESULT Init();

    //! print info about all playback devices that RtAudio found.
    void printDeviceInfo();

    //		void DoUpdate();

}; /* class RtAudioFeeder */

}; /* namespace libsch */
#endif // RTAUDIOFEEDER_H
