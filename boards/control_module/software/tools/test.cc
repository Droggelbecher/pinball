#include <map>
#include <array>
#include "coordinate.h"

Coordinate<> size { 11, 5 };
std::map<char, std::array<unsigned char, 55>> font = { 
{ 97, {  // [a]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
    0,    0,    0,    0,  255,  
    0,  255,  255,  255,  255,  
  255,    0,    0,    0,  255,  
    0,  255,  255,  255,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 98, {  // [b]
    0,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,  255,  255,    0,  
  255,  255,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,  255,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 99, {  // [c]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,  255,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
    0,  255,  255,  255,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 100, {  // [d]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,  255,  
    0,    0,    0,    0,  255,  
    0,    0,    0,    0,  255,  
    0,  255,  255,  255,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,  255,  255,  
    0,  255,  255,    0,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 101, {  // [e]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
  255,  255,  255,  255,  255,  
  255,    0,    0,    0,    0,  
    0,  255,  255,  255,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 102, {  // [f]
    0,    0,    0,    0,    0,  
    0,    0,  255,  255,    0,  
    0,  255,    0,    0,    0,  
    0,  255,    0,    0,    0,  
    0,  255,  255,  255,    0,  
    0,  255,    0,    0,    0,  
    0,  255,    0,    0,    0,  
    0,  255,    0,    0,    0,  
    0,  255,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 103, {  // [g]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,  255,  255,  
    0,  255,  255,    0,  255,  
    0,    0,    0,    0,  255,  
    0,  255,  255,  255,    0,  
  } },
{ 104, {  // [h]
    0,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,  255,  255,    0,  
  255,  255,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 105, {  // [i]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 106, {  // [j]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
  255,  255,    0,    0,    0,  
  } },
{ 107, {  // [k]
    0,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,  255,    0,  
  255,    0,  255,    0,    0,  
  255,  255,  255,    0,    0,  
  255,    0,    0,  255,    0,  
  255,    0,    0,    0,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 108, {  // [l]
    0,    0,    0,    0,    0,  
    0,  255,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,  255,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 109, {  // [m]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  255,  255,  255,  255,    0,  
  255,    0,  255,    0,  255,  
  255,    0,  255,    0,  255,  
  255,    0,  255,    0,  255,  
  255,    0,  255,    0,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 110, {  // [n]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  255,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 111, {  // [o]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,  255,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 112, {  // [p]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  255,    0,  255,  255,    0,  
  255,  255,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,  255,  255,  255,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  } },
{ 113, {  // [q]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,  255,  255,  
    0,  255,  255,    0,  255,  
    0,    0,    0,    0,  255,  
    0,    0,    0,    0,  255,  
  } },
{ 114, {  // [r]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  255,    0,  255,  255,    0,  
  255,  255,    0,    0,  255,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 115, {  // [s]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
  255,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
    0,    0,    0,    0,  255,  
  255,  255,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 116, {  // [t]
    0,    0,    0,    0,    0,  
    0,  255,    0,    0,    0,  
    0,  255,    0,    0,    0,  
    0,  255,    0,    0,    0,  
  255,  255,  255,  255,    0,  
    0,  255,    0,    0,    0,  
    0,  255,    0,    0,    0,  
    0,  255,    0,    0,    0,  
    0,    0,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 117, {  // [u]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,  255,  255,  
    0,  255,  255,    0,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 118, {  // [v]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,  255,    0,  255,    0,  
    0,  255,    0,  255,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 119, {  // [w]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  255,    0,    0,    0,  255,  
  255,    0,  255,    0,  255,  
  255,    0,  255,    0,  255,  
  255,    0,  255,    0,  255,  
    0,  255,    0,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 120, {  // [x]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  255,    0,    0,    0,  255,  
    0,  255,    0,  255,    0,  
    0,    0,  255,    0,    0,  
    0,  255,    0,  255,    0,  
  255,    0,    0,    0,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 121, {  // [y]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,  255,  255,  
    0,  255,  255,    0,  255,  
    0,    0,    0,    0,  255,  
    0,  255,  255,  255,    0,  
  } },
{ 122, {  // [z]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  255,  255,  255,  255,  255,  
    0,    0,    0,  255,    0,  
    0,    0,  255,    0,    0,  
    0,  255,    0,    0,    0,  
  255,  255,  255,  255,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 65, {  // [A]
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,  255,  255,  255,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 66, {  // [B]
    0,    0,    0,    0,    0,  
  255,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,  255,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 67, {  // [C]
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,  255,  
    0,  255,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 68, {  // [D]
    0,    0,    0,    0,    0,  
  255,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,  255,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 69, {  // [E]
    0,    0,    0,    0,    0,  
  255,  255,  255,  255,  255,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,  255,  255,  255,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,  255,  255,  255,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 70, {  // [F]
    0,    0,    0,    0,    0,  
  255,  255,  255,  255,  255,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,  255,  255,  255,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 71, {  // [G]
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,    0,  
  255,    0,  255,  255,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,  255,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 72, {  // [H]
    0,    0,    0,    0,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,  255,  255,  255,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 73, {  // [I]
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,  255,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 74, {  // [J]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,  255,  
    0,    0,    0,    0,  255,  
    0,    0,    0,    0,  255,  
    0,    0,    0,    0,  255,  
    0,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,  255,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 75, {  // [K]
    0,    0,    0,    0,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,  255,    0,  
  255,    0,  255,    0,    0,  
  255,  255,    0,    0,    0,  
  255,    0,  255,    0,    0,  
  255,    0,    0,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 76, {  // [L]
    0,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,  255,  255,  255,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 77, {  // [M]
    0,    0,    0,    0,    0,  
  255,    0,    0,    0,  255,  
  255,  255,    0,  255,  255,  
  255,    0,  255,    0,  255,  
  255,    0,  255,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 78, {  // [N]
    0,    0,    0,    0,    0,  
  255,    0,    0,    0,  255,  
  255,  255,    0,    0,  255,  
  255,  255,    0,    0,  255,  
  255,    0,  255,    0,  255,  
  255,    0,  255,    0,  255,  
  255,    0,    0,  255,  255,  
  255,    0,    0,  255,  255,  
  255,    0,    0,    0,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 79, {  // [O]
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,  255,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 80, {  // [P]
    0,    0,    0,    0,    0,  
  255,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,  255,  255,  255,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 81, {  // [Q]
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,  255,    0,  255,  
  255,    0,    0,  255,    0,  
    0,  255,  255,    0,  255,  
    0,    0,    0,    0,  255,  
    0,    0,    0,    0,    0,  
  } },
{ 82, {  // [R]
    0,    0,    0,    0,    0,  
  255,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,  255,  255,  255,    0,  
  255,    0,    0,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 83, {  // [S]
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
    0,    0,    0,    0,  255,  
    0,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,  255,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 84, {  // [T]
    0,    0,    0,    0,    0,  
  255,  255,  255,  255,  255,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 85, {  // [U]
    0,    0,    0,    0,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,  255,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 86, {  // [V]
    0,    0,    0,    0,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,  255,    0,  255,    0,  
    0,  255,    0,  255,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 87, {  // [W]
    0,    0,    0,    0,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,  255,    0,  255,  
  255,    0,  255,    0,  255,  
  255,    0,  255,    0,  255,  
    0,  255,    0,  255,    0,  
    0,  255,    0,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 88, {  // [X]
    0,    0,    0,    0,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,  255,    0,  255,    0,  
    0,    0,  255,    0,    0,  
    0,  255,    0,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 89, {  // [Y]
    0,    0,    0,    0,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,  255,    0,  255,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 90, {  // [Z]
    0,    0,    0,    0,    0,  
  255,  255,  255,  255,  255,  
    0,    0,    0,    0,  255,  
    0,    0,    0,  255,    0,  
    0,    0,  255,    0,    0,  
    0,  255,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,  255,  255,  255,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 48, {  // [0]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,  255,  255,  
  255,    0,  255,    0,  255,  
  255,  255,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,  255,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 49, {  // [1]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,  255,    0,    0,  
    0,  255,  255,    0,    0,  
  255,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 50, {  // [2]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
    0,    0,    0,    0,  255,  
    0,    0,    0,  255,    0,  
    0,    0,  255,    0,    0,  
    0,  255,    0,    0,    0,  
  255,  255,  255,  255,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 51, {  // [3]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
    0,    0,    0,    0,  255,  
    0,    0,  255,  255,    0,  
    0,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,  255,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 52, {  // [4]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,  255,    0,  
    0,    0,  255,  255,    0,  
    0,  255,    0,  255,    0,  
  255,    0,    0,  255,    0,  
  255,  255,  255,  255,  255,  
    0,    0,    0,  255,    0,  
    0,    0,    0,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 53, {  // [5]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  255,  255,  255,  255,  255,  
  255,    0,    0,    0,    0,  
  255,  255,  255,  255,    0,  
    0,    0,    0,    0,  255,  
    0,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,  255,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 54, {  // [6]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
  255,    0,    0,    0,    0,  
  255,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,  255,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 55, {  // [7]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  255,  255,  255,  255,  255,  
    0,    0,    0,    0,  255,  
    0,    0,    0,  255,    0,  
    0,    0,    0,  255,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 56, {  // [8]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,  255,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 57, {  // [9]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,    0,    0,  255,  
    0,  255,  255,  255,  255,  
    0,    0,    0,    0,  255,  
    0,    0,    0,    0,  255,  
    0,  255,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 33, {  // [!]
    0,    0,    0,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 64, {  // [@]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
  255,    0,  255,  255,  255,  
  255,    0,  255,    0,  255,  
  255,    0,  255,  255,  255,  
  255,    0,    0,    0,    0,  
    0,  255,  255,  255,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 35, {  // [#]
    0,    0,    0,    0,    0,  
    0,  255,    0,  255,    0,  
    0,  255,    0,  255,    0,  
  255,  255,  255,  255,  255,  
    0,  255,    0,  255,    0,  
  255,  255,  255,  255,  255,  
    0,  255,    0,  255,    0,  
    0,  255,    0,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 36, {  // [$]
    0,    0,    0,    0,    0,  
    0,    0,  255,    0,    0,  
    0,  255,  255,  255,    0,  
  255,    0,  255,    0,  255,  
  255,    0,  255,    0,    0,  
    0,  255,  255,  255,    0,  
    0,    0,  255,    0,  255,  
  255,    0,  255,    0,  255,  
    0,  255,  255,  255,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 37, {  // [%]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,    0,    0,  255,  
  255,    0,  255,    0,  255,  
    0,  255,    0,  255,    0,  
    0,    0,  255,    0,    0,  
    0,  255,    0,  255,    0,  
  255,    0,  255,    0,  255,  
  255,    0,    0,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 94, {  // [^]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,  255,    0,    0,  
    0,  255,    0,  255,    0,  
  255,    0,    0,    0,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 38, {  // [&]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,    0,    0,  
  255,    0,    0,  255,    0,  
  255,    0,  255,    0,    0,  
    0,  255,    0,    0,    0,  
  255,    0,  255,    0,  255,  
  255,    0,    0,  255,    0,  
    0,  255,  255,    0,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 42, {  // [*]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,  255,    0,    0,  
  255,    0,  255,    0,  255,  
    0,  255,  255,  255,    0,  
  255,    0,  255,    0,  255,  
    0,    0,  255,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 40, {  // [(]
    0,    0,    0,    0,    0,  
    0,    0,    0,  255,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,  255,    0,    0,    0,  
    0,  255,    0,    0,    0,  
    0,  255,    0,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,  255,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 41, {  // [)]
    0,    0,    0,    0,    0,  
    0,  255,    0,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,  255,    0,  
    0,    0,    0,  255,    0,  
    0,    0,    0,  255,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,  255,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 123, {  // [{]
    0,    0,    0,  255,  255,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
  255,  255,    0,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,  255,  255,  
  } },
{ 125, {  // [}]
  255,  255,    0,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,  255,  255,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
  255,  255,    0,    0,    0,  
  } },
{ 91, {  // [[]
    0,    0,    0,    0,    0,  
    0,    0,  255,  255,  255,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,  255,  255,  
    0,    0,    0,    0,    0,  
  } },
{ 93, {  // []]
    0,    0,    0,    0,    0,  
  255,  255,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
  255,  255,  255,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 47, {  // [/]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,  255,  
    0,    0,    0,    0,  255,  
    0,    0,    0,  255,    0,  
    0,    0,    0,  255,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,  255,    0,    0,    0,  
    0,  255,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  255,    0,    0,    0,    0,  
  } },
{ 63, {  // [?]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,  255,    0,  
  255,    0,    0,    0,  255,  
    0,    0,    0,    0,  255,  
    0,    0,    0,  255,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 61, {  // [=]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  255,  255,  255,  255,  255,  
    0,    0,    0,    0,    0,  
  255,  255,  255,  255,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 43, {  // [+]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
  255,  255,  255,  255,  255,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 45, {  // [-]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  255,  255,  255,  255,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 95, {  // [_]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  255,  255,  255,  255,  255,  
    0,    0,    0,    0,    0,  
  } },
{ 92, {  // [\]
    0,    0,    0,    0,    0,  
    0,  255,    0,    0,    0,  
    0,  255,    0,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,  255,    0,  
    0,    0,    0,  255,    0,  
    0,    0,    0,    0,  255,  
    0,    0,    0,    0,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 124, {  // [|]
    0,    0,    0,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 39, {  // [']
    0,    0,    0,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 34, {  // ["]
    0,    0,    0,    0,    0,  
    0,  255,    0,  255,    0,  
    0,  255,    0,  255,    0,  
    0,  255,    0,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 44, {  // [,]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,    0,    0,  
    0,  255,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,  255,    0,    0,    0,  
  } },
{ 60, {  // [<]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,  255,  
    0,    0,    0,  255,    0,  
    0,    0,  255,    0,    0,  
    0,  255,    0,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,  255,    0,  
    0,    0,    0,    0,  255,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 46, {  // [.]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,    0,    0,  
    0,  255,  255,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 62, {  // [>]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,    0,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,  255,    0,  
    0,    0,    0,    0,  255,  
    0,    0,    0,  255,    0,  
    0,    0,  255,    0,    0,  
    0,  255,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 96, {  // [`]
    0,    0,    0,    0,    0,  
    0,  255,    0,    0,    0,  
    0,    0,  255,    0,    0,  
    0,    0,    0,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 126, {  // [~]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,    0,    0,    0,  
  255,    0,  255,    0,  255,  
    0,    0,    0,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
{ 59, {  // [;]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,    0,    0,  
    0,  255,  255,    0,    0,  
    0,    0,    0,    0,    0,  
    0,  255,  255,    0,    0,  
    0,  255,  255,    0,    0,  
    0,    0,  255,    0,    0,  
    0,  255,    0,    0,    0,  
  } },
{ 58, {  // [:]
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
    0,    0,  255,  255,    0,  
    0,    0,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,  255,  255,    0,  
    0,    0,  255,  255,    0,  
    0,    0,    0,    0,    0,  
    0,    0,    0,    0,    0,  
  } },
};
