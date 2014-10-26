
#ifndef Export_h__
#define Export_h__


#ifdef _WIN32
#ifdef LIBSCH_EXPORT_DLL                          
#define DllExport __declspec(dllexport)   
#else                                     
#define DllExport __declspec(dllimport)   
#endif                                              
#else                                     
#define DllExport                                   
#endif                                               

#endif  /* EXPORT_H */
