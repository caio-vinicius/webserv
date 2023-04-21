#include <iostream>
#include <string>

std::string find_file_path(const std::string& root, const std::string& location, const std::string& request_uri)
{
    // Remove a barra inicial do request_uri, se houver
    std::string uri = request_uri;
    // if (uri[0] == '/') {
    //     uri.erase(0, 1);
    // }

    // Remove a localização do request_uri e adicione o root
    if (uri.find(location) == 0) {
        // Remove a localização do URI da solicitação
        std::string path = uri.substr(location.length());

        // Adiciona o caminho raiz e uma barra no início do caminho
        path = root + path;

        return path;
    }
    std::cerr << "A localização não foi encontrada no URI da solicitação." << std::endl;
    return "";`
}

int main()
{
    std::string root = "/tmp/www";
    std::string location = "/kapouet";
    std::string request_uri = "/kapouet/pouic/toto/pouet";

    std::string file_path = find_file_path(root, location, request_uri);

    std::cout << "Caminho do arquivo: " << file_path << std::endl;

    return 0;
}
