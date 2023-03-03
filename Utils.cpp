#include <string>
#include <iterator>

std::string remove_chr(std::string str, char ch) {
    for (std::string::iterator it = str.begin(); it != str.end(); ) {
        if (*it == ch) {
            it = str.erase(it);
        } else {
            ++it;
        }
    }
    return (str);
}
