#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <memory>

namespace frequency_dictionary
{
    struct IFrequencyDictionary
    {
        virtual ~IFrequencyDictionary() = default;

        virtual bool Fill(const char* fileNameIn) = 0;
        virtual bool SortedResult(const char* fileNameOut) = 0;
    };

    class FrequencyDictionary : public IFrequencyDictionary
    {
    public:
        bool Fill(const char* fileNameIn) override
        {
            try
            {
                std::ifstream stream{ fileNameIn };

                if (!stream.is_open())
                    return false;

                std::string str;
                std::string word;
                std::string::iterator pos, prev;
                while (std::getline(stream, str))
                {
                    for (pos = std::find_if(str.begin(), str.end(), isalpha); pos != str.end();
                        pos = std::find_if(prev, str.end(), isalpha))
                    {
                        prev = std::find_if_not(pos, str.end(), isalpha);
                        word = {pos, prev};
                        if (!word.empty())
                        {
                            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
                            dictionary_[std::move(word)]++;
                        }
                    }
                } 
            }
            catch (std::exception& e)
            {
                std::cout << e.what() << "\n";
                return false;
            }

            return true;
        }

        bool SortedResult(const char* fileNameOut) override
        {
            try
            {
                std::ofstream stream{ fileNameOut, std::ios::out | std::ios::binary | std::ios::trunc };

                if (!stream.is_open())
                    return false;

                std::vector<std::pair<std::string, int>> sortedDictionary;
                sortedDictionary.reserve(dictionary_.size());

                std::for_each(std::cbegin(dictionary_), std::cend(dictionary_), [&sortedDictionary](auto&& pair) {
                    sortedDictionary.push_back(pair);
                });

                std::sort(sortedDictionary.begin(), sortedDictionary.end(), [](auto&& lhs, auto&& rhs) {
                    if (lhs.second == rhs.second)
                        return lhs.first < rhs.first;

                    return lhs.second > rhs.second;
                });

                for (auto&& [word, freq] : sortedDictionary)
                {
                    stream << freq << " " << word << "\n";
                }
            }
            catch (std::exception& e)
            {
                std::cout << e.what() << "\n";
                return false;
            }

            return true;
        }

    private:
        std::unordered_map<std::string, int> dictionary_;
    };
}

int main(int argc, char* argv[])
{
    auto dictionary = std::make_unique<frequency_dictionary::FrequencyDictionary>();
    if (!dictionary->Fill(argv[1]))
    {
        std::cout << "Error of filling dictionary";
        return 1;
    }   
    if (!dictionary->SortedResult(argv[2]))
    {
        std::cout << "Error of geting result";
        return 1;
    }

    return 0;
}