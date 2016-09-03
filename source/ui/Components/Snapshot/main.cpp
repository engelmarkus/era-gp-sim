#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <iostream>
#include <QQMLContext>
#include <QQmlEngine>

#include "snapshotmodel.hpp"



int main(int argc, char *argv[])
{//file for the output, will not be added to Master-branche
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    SnapshotModel snapmodel(engine.rootContext());
    std::string array[]={"Apple","Banana", "Strawberry", "Cherry", "Tangerine", "Pineapple", "Mango", "Cranberries", "Raspberry", "Blackberry", "Pear", "Peach", "Lemon",
                         "Lime", "Aprciot", "Kiwi", "Melon", "Passion fruit", "Pomegranate", "Coconut", "Plum", "Grape", "Currant"};
    snapmodel.addList(array);
//    snapmodel.add("Apple");
//    snapmodel.add("Banana");
//    snapmodel.add("Strawberry");
//    snapmodel.add("Cherry");
//    snapmodel.add("Tangerine");
//    snapmodel.add( "Pineapple");
//    snapmodel.add( "Mango");
//    snapmodel.add("Cranberries");
//    snapmodel.add( "Raspberry");
//    snapmodel.add( "Blackberry");
//    snapmodel.add( "Pear");
//    snapmodel.add("Peach");
//    snapmodel.add( "Lemon");
//    snapmodel.add( "Lime");
//    snapmodel.add( "Aprciot");
//    snapmodel.add("Kiwi");
//    snapmodel.add( "Melon");
//    snapmodel.add( "Passion fruit");
//    snapmodel.add( "Pomegranate");
//    snapmodel.add( "Coconut");
//    snapmodel.add( "Plum");
//    snapmodel.add("Grape");
//    snapmodel.add( "Currant");
//    engine.rootContext()->setContextProperty("snapshotModel",  &snapmodel);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}


