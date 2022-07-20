#include "Serializer.hpp"
#include "Tree.hpp"
#include "Graph.hpp"
#include <cstring>

#pragma once

void readData(std::string path){
  Serializer sz;
  sz.openFile(path, MODE::READ);
  // Determine which data structure it is
  // Initialize correspond data structure
  // and return pointer (or object?)
  // Note: maybe a better idea to merge tree & graph class?
}
