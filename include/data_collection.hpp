#ifndef DATA_COLLECTION_HPP
#define DATA_COLLECTION_HPP

#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <boost/tokenizer.hpp>

class data_collection
{
private:
    std::vector<std::vector<std::string>> data;

public:
    // getter
    std::vector<std::vector<std::string>> get_data() const { return data; }

    void read_csv(const std::string &filename);
};

#endif // DATA_COLLECTION_HPP