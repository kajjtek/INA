#ifndef FILEREADER_H
#define FILEREADER_H

#include "../header/Graph.h"
#include <string>

class FileReader {
    public:
    Graph createGraphFromFile(std::string path);
};

#endif