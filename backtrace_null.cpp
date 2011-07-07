#include "backtrace.hpp"

#include <iostream>

using namespace std;


debug::backtrace::backtrace (void):
    m_frames (DEFAULT_DEPTH) 
{ ; }


ostream&
operator <<(ostream &os, const debug::backtrace &rhs) {
    os << "null backtrace";
    return os;
}
