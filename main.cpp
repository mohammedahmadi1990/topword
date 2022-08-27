#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <regex>

 /********************************************************************
  TOP WORD APPLICATION
  Developed By: Me
  Course Name:
  Project: HW0:

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
    const std::regex rgx("\\w+");

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
                // it will continue till the getline can get != NULL
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
                        words.insert_or_assign(key, count);
                    } else {
                        // if word is new add and put 1 as frequency
                        words.insert_or_assign(key, 1);
                    }
                }
            }
            file.close();
        }
    } catch (std::ifstream::failure &e) {
        std::cout << e.what() << std::endl;
    }

    /*___ WRITE TO FILE ___*/
    try {
        std::ofstream file;
        file.open(output);
        for (auto it = words.begin(); it != words.end(); it++) {
            // for is considered since we are aware of the size of the map and we can iterate to print key/value
            file << it->first << " " << it->second << "\n";
        }
        file.close();
    } catch (std::ofstream::failure &e) {
        std::cout << e.what() << std::endl;
    }
}
