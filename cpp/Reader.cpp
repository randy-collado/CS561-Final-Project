#include "Graph.hpp"
#include "Serializer.hpp"
#include "Tree.hpp"
#include <cstring>

#pragma once

void readData(std::string path) {
  Serializer *sz = new Serializer();
  sz->openFile(path, MODE::READ);

  S_MetaData *sm = sz->readMetadata();
  switch (sm->dataType) {
  case 1:
    /* Tree */
    Tree *tree = new Tree(0, 0);
    tree->init_serializer(sz);
    break;
  case 2:
    /* Graph */
    Graph *graph = new Graph();
    graph->init_serializer(sz);
    break;
  default:
    break;
  }
  // Determine which data structure it is
  // Initialize correspond data structure
  // and return pointer (or object?)
}
