#pragma once
#include <iostream>
#include <sstream>
#include <CGAL/IO/io.h>

template<typename T>
std::string toString(T o) {
    std::ostringstream ss;
    CGAL::set_pretty_mode(ss);
    ss << o;
    return ss.str();
}
