#include "node.h"
#include <string>

void writeNodeOffset(HANDLE hFile, Node *p);

int writeTreeToFile(std::string fName, Node *root);
