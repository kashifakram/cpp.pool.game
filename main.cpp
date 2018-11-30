/**
  *     Written by Tim Burr
  *     2018/05/18
  */

#include "dialog.h"
#include "game.h"
#include "utils.h"
#include "gamebuilder.h"
#include "stagetwobuilder.h"
#include <QApplication>
#include <QFile>
#include <iostream>
#include <QString>
#include <QJsonObject>
#include <ctime>
#include <QJsonDocument>

QJsonObject loadConfig() {
    // load json from config file
    QFile conf_file(config_path);
    conf_file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString content = conf_file.readAll();
    conf_file.close();
    QJsonObject config = QJsonDocument::fromJson(content.toUtf8()).object();
    return config;
}

int main(int argc, char *argv[])
{
    QJsonObject conf = loadConfig();

    // seed our RNG
    srand(time(nullptr));

    // create our game based on our config
    GameDirector director(&conf);
    // use builder2 if we're stage two (defaults to false), otherwise no

//    int stage2Flag = -1;
//    int stage3Flag = -1;
//    int i = 0;

//    for(auto key:conf.keys())
//    {
//        if(key=="stage2")
//        {
//            stage2Flag = i;
//        } else if (key == "stage3") {
//            stage3Flag = i;
//        }
//        i++;
//    }

//    if (stage2Flag > stage3Flag)
//    {
//        director.setBuilder(new StageTwoBuilder());
//    } else if (stage3Flag > stage2Flag) {
//        director.setBuilder(new StageThreeBuilder());
//    } else {
//        director.setBuilder(new StageOneBuilder());
//    }


    if (conf.value("stage2").toBool(false) == true) {
       director.setBuilder(new StageTwoBuilder());
    } else if (conf.value("stage3").toBool(false) == true) {
        // set and transfer ownership of this builder to the director
//        director.setBuilder(new StageThreeBuilder());
    } else {
        // set and transfer ownership of this builder to the director
        director.setBuilder(new StageOneBuilder());
    }

    Game* game = director.createGame();

    // display our dialog that contains our game and run
    QApplication a(argc, argv);
    Dialog w(game, nullptr);
    w.show();

    return a.exec();
}
