#include "List.h"

template <typename T> class Quene : public List<T> {
public:
    void enquene ( T const& e ) { this->insertAsLast ( e ); }
    T dequene() { return this->remove ( this->first() ); }
    T& front() { return this->first()->data; }
};


