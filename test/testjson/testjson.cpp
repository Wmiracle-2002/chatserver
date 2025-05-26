#include "json.hpp"
using json = nlohmann::json;

#include <iostream>
#include <vector>
#include <map>
#include <string>

void func1(){
    json js;
    js["msg_type"] = 2;
    js["from"] = "zhang san";
    js["to"] = "li si";
    js["msg"] = "hello, what are you doing now?";
    std::string sendBuf = js.dump();
    std::cout << sendBuf.c_str() << std::endl;
}

int main(){
    func1();
    return 0;
}