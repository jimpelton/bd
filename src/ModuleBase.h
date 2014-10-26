


#include "BdTypes.h"

#include <vector>


namespace libsch
{

template<class T>
class ModuleBase 
{
public:

    ModuleBase();
    ModuleBase(const ModuleBase &other);
    ~ModuleBase();

    ModuleBase& operator=(const ModuleBase &rhs);
    
    virtual T tick(T val) = 0;

private:
    std::vector<T> buf;


};

template<class T>
ModuleBase<T>::ModuleBase() {}

template<class T>
ModuleBase<T>::ModuleBase(const ModuleBase<T> &other) {}

template<class T>
ModuleBase<T>::~ModuleBase() {}

template<class T>
ModuleBase<T>& operator=(const ModuleBase<T> &rhs) {}




} //namespace libsch




