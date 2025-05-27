#include <json.hpp>
#include <fstream>
#include <iostream>

using namespace std;
using json = nlohmann::json;

int main(){
    json js;
    ofstream ofs;
    ofs.open("dbconfig.json", ios::out);
    js["ip"] = "127.0.0.1";
    js["port"] = 3306;
    js["username"] = "root";
    js["password"] = "wmiracle";
    js["dbName"] = "chat";
    js["minSize"] = 4;
    js["maxSize"] = 8;
    js["maxIdleTime"] = 60;
    js["timeout"] = 60;
    ofs << js.dump();
    ofs.close();
    return 0;
}