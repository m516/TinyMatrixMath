// A reimplementation of enable_if_t, a modern C++ tool built into the standard library.

#pragma once

// Sample usage: enable_if_t<condition, function_return_type> function_name(){...; return ...;}

// Example:
//      enable this_function only if n==1 and m==1, and its return type is Scalar
//      enable_if_t<(n==1&&m==1), insert_return_type_here>::type() this_function(){return insert_return_type_here();}

namespace tmm{

    template<bool Cond, class T = void> struct enable_if {};
    template<class T> struct enable_if<true, T> { typedef T type; };

    template< bool B, class T = void >
    using enable_if_t = typename enable_if<B,T>::type;

}