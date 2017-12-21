#ifndef CONFIG_H
#define CONFIG_H

class Config {
    public:
        int winX = 1280;
        int winY = 720;
        bool savingEnabled = false;
        std::string currentMap;
};
#endif // CONFIG_H
