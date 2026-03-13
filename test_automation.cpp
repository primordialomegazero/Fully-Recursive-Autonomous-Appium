// Test script for Appium automation
// Compile with: g++ -o test_automation test_automation.cpp -lcurl -lpthread

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <cstring>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total = size * nmemb;
    output->append((char*)contents, total);
    return total;
}

int main() {
    CURL* curl = curl_easy_init();
    if(!curl) {
        std::cerr << "Failed to initialize curl" << std::endl;
        return 1;
    }
    
    std::string response;
    
    // Create session
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:6790/wd/hub/session");
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    
    const char* data = "{\"capabilities\": {\"platformName\": \"Android\"}}";
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    
    CURLcode res = curl_easy_perform(curl);
    
    if(res == CURLE_OK) {
        std::cout << "Session created: " << response << std::endl;
    } else {
        std::cerr << "Curl failed: " << curl_easy_strerror(res) << std::endl;
    }
    
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    
    return 0;
}
