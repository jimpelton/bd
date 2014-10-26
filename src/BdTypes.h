#ifndef SCHTYPES_H
#define SCHTYPES_H

#ifdef _WIN32
#define __func__ __FUNCTION__
#endif // _WIN32

namespace libsch
{

/*!
  \typedef typedef float realval_t;
  \brief libSch real number floating value.
 */
typedef float realval_t;

/*!
  \typedef typedef signed short SAMPLE_TYPE;
  \brief The data type for audio samples in the file
*/
typedef float sample_t;

/*!
  \typedef typedef enum SCH_RESULT;
  \brief Result types for operations in libsch.
 */
typedef enum
{
    SCH_ERR,                        //!< Misc. error from libsch.
    SCH_ERR_RTAUDIO,                //!< RtAudio error.
    SCH_ERR_FMOD,                   //!< Error interacting with fmod.
    SCH_ERR_OUTOFBOUNDS,            //!< A requested size/index was out of buffer bounds.
    SCH_ERR_FILE_IO,                //!< Misc File IO error.
    SCH_ERR_FILE_NOT_OPEN,          //!< Could not open file.
    SCH_ERR_FILE_ALREADY_OPEN,      //!< File was already open, so didn't open again.
	SCH_ERR_ADD_CHILD,              //!< Child wasn't added to pipeline.

    SCH_OK_PARENT_NOT_ADDED,        //!< Given parent not added to this module.
    SCH_OK_CHILD_EXISTS,            //!< Given child already child of this module--not added.
    SCH_OK                          //!< Nice!
} SCH_RESULT;

};

#endif // SCHTYPES_H
