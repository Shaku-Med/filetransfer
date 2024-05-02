#include <crow.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

#include <crow/middleware/multipart.h>

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/api")
        .methods("POST"_method)
        .multipart()([](const crow::request &req, crow::response &res)
                     {
            if (req.files.count("file") == 1) {
                auto& file = req.files["file"];
                std::string uuid = crow::utility::uuid::random();
                std::string ext = ".txt";
                std::string mimetype = file.mime;
                if (mimetype.find("video") != std::string::npos) {
                    ext = ".mp4";
                } else if (mimetype.find("audio") != std::string::npos) {
                    ext = ".mp3";
                } else {
                    ext = "." + mimetype.substr(mimetype.find("/") + 1);
                }
                std::string filename = "Files/" + file.name + "_" + uuid + ext;
                std::ofstream ofs(filename, std::ios::binary);
                ofs << file.blob;
                ofs.close();
                res.write(crow::json::dump({{"success", true}}));
                res.end();
            } else {
                res.code = 400;
                res.end();
            } });

    CROW_ROUTE(app, "/<string>")
    ([&](const std::string &path)
     {
            crow::mustache::context ctx;
            std::ifstream file("Files/" + path);
            std::string content((std::istreambuf_iterator<char>(file)),
                                (std::istreambuf_iterator<char>()));
            return content; });

    app.port(3000).multithreaded().run();
    return 0;
}
