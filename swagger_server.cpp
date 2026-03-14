#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>

void handle_client(int client_socket) {
    char buffer[4096] = {0};
    read(client_socket, buffer, 4095);
    
    std::string request(buffer);
    std::string response;
    std::string content;
    std::string content_type;
    
    if (request.find("GET /swagger.json") != std::string::npos) {
        std::ifstream file("/home/singularitynode/Fully-Recursive-Autonomous-Appium/swagger/swagger.json");
        if (file) {
            std::stringstream ss;
            ss << file.rdbuf();
            content = ss.str();
            content_type = "application/json";
        }
    } else {
        std::ifstream file("/home/singularitynode/Fully-Recursive-Autonomous-Appium/swagger/index.html");
        if (file) {
            std::stringstream ss;
            ss << file.rdbuf();
            content = ss.str();
            content_type = "text/html";
        }
    }
    
    if (content.empty()) {
        response = "HTTP/1.1 404 Not Found\r\n";
        response += "Content-Type: text/html\r\n";
        response += "\r\n";
        response += "<html><body><h1>404 Not Found</h1></body></html>";
    } else {
        response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: " + content_type + "\r\n";
        response += "Content-Length: " + std::to_string(content.length()) + "\r\n";
        response += "Access-Control-Allow-Origin: *\r\n";
        response += "\r\n";
        response += content;
    }
    
    send(client_socket, response.c_str(), response.length(), 0);
    close(client_socket);
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Socket failed" << std::endl;
        return 1;
    }
    
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(server_fd);
        return 1;
    }
    
    if (listen(server_fd, 10) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(server_fd);
        return 1;
    }
    
    std::cout << "Swagger UI at http://localhost:8080" << std::endl;
    
    while (true) {
        int client = accept(server_fd, nullptr, nullptr);
        if (client >= 0) {
            std::thread(handle_client, client).detach();
        }
    }
    
    close(server_fd);
    return 0;
}
