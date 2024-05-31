#include "data_collection.hpp"

void data_collection::read_csv(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file\n";
        return;
    }

    std::string line;
    bool first_line = true; // for skipping the first line

    while (std::getline(file, line))
    {
        if (first_line) // for skipping the first line
        {
            first_line = false;
            continue;
        }
        boost::tokenizer<boost::escaped_list_separator<char>> tok(line);
        std::vector<std::string> row;

        for (const auto &t : tok)
        {
            row.push_back(t);
        }
        data.push_back(row);
    }

    file.close();
}