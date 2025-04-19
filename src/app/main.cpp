#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QString>

using namespace Qt::Literals::StringLiterals;

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    
    QQmlApplicationEngine engine("qrc:/qt/qml/application/main.qml");
    //engine.loadFromModule(u"Main"_s, u"main"_s);

    return QGuiApplication::exec();
}
