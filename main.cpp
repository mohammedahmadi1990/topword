#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <regex>


 /********************************************************************
  TOP WORD APPLICATION
  Developed By: Med
  Course Name: --
  Project: HW0: Finding the most frequent word(s) in a file

  Requirements to Run:
    GNU C++ (g++)
  Command to Run the Code
    topword "input=f.txt;output=out.txt"
  Description:
    You can specify any name as the input/output file since the application can parse it but the default
    argument for the application to start is as below:

    topword "input=f.txt;output=out.txt"
 *********************************************************************/

/*___ CONST ERROR MESSAGES ___*/
const std::string INPUT_ARG_ERROR = "Error! INPUT argument is not based on template string as below:\n"
                                    "       topword \"input=f.txt;output=out.txt\" \n";
const std::string OUTPUT_ARG_ERROR = "Error! OUTPUT argument is not based on template string as below:\n"
                                    "       topword \"input=f.txt;output=out.txt\" \n";
const std::string ARG_ERROR = "Error! You require to enter input/output files as arguments as below:\n"
                                    "       topword \"input=f.txt;output=out.txt\" \n";

int main(int argc, char *argv[]) {

    /*___ FIELDS ___*/
    std::string input;
    std::string output;
    // Map since it's a key\value based data structure which saves non repetitive words
    // beside number of repetition inside the text. Also map keeps data as sorted which
    // has enabled us to search/find with O(log n) time complexity since it internally utilizes BalancedBinarySearchTree.
    std::map<std::string, int> words;
    const std::regex rgx("\\w+"); // regex for matching unlimited number of words
    std::vector<std::pair<std::string, int>> words_vector; // to keep words in a vector to sort based on freq and letter

    /*___ PROCESS ARGUMENT ___*/
    if (argc == 2) { // check if number of arguments are correct
        std::string arg = argv[1];
        int args_splitter_index = arg.find(";"); // semicolon as delimiter with input/output filenames
        input = arg.substr(0, args_splitter_index);
        output = arg.substr(args_splitter_index + 1);
        if (input.substr(0, 6) == "input=") // check if the keyword exists
            input = input.substr(6);
        else
            std::cout << INPUT_ARG_ERROR;
        if (output.substr(0, 7) == "output=") // check if the keyword exists
            output = output.substr(7);
        else
            std::cout << OUTPUT_ARG_ERROR;
    } else {
        std::cout << ARG_ERROR;
        return 0;
    }

    /*___ READ FROM FILE ___*/
    try {
        std::ifstream file(input);
        if (file.is_open()) {
            std::string line;
            while (getline(file, line)) {
                // it will continue till the getline() can get != NULL
                for (std::sregex_iterator it(line.begin(), line.end(), rgx), it_end; it != it_end; it++) {
                    // iterator of REGEX which can iterate result set of the content matching our REGEX
                    std::string word = (*it)[0];
                    std::string key = "";
                    for(auto& c : word)
                    {
                        key += tolower(c);
                    }
                    if (words[key]) {
                        // if there is a match inside the map add 1 to the frequency of the word
                        int count = words[key];
                        count++;
                        words.erase(key);
                        words.insert(std::pair<std::string,int>(key, count));
                    } else {
                        // if word is new add and put 1 as frequency
                        words.erase(key);
                        words.insert(std::pair<std::string,int>(key, 1));
                    }
                }
            }
            file.close();
        }
    } catch (std::ifstream::failure &e) {
        std::cout << e.what() << std::endl;
    }

    // To copy all from Map to a Vector in order to sort based on Value and then key
    std::copy(words.begin(),
              words.end(),
              std::back_inserter<std::vector<std::pair<std::string, int>>>(words_vector));

    // Sorting based on internal STD sorting hybrid algorithm which consists of QuickSort, HeapSort, InsertionSort based
    // on size and randomness of data
    std::sort(words_vector.begin(), words_vector.end(),
              [](const std::pair<std::string, int> &l, const std::pair<std::string, int> &r)
              {
                  if (l.second != r.second)
                      return l.second > r.second;
                  return l.first < r.first;
              });


    /*___ WRITE TO FILE ___*/
    try {
        std::ofstream file;
        file.open(output);
        for (auto const &record: words_vector) {
            file << record.second << " " << record.first << "\n";
        }
        file.close();
    } catch (std::ofstream::failure &e) {
        std::cout << e.what() << std::endl;
    }
}

