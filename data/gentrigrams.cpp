#include <QtCore/QFile>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QDebug>

int main(int argc, char *argv[])
{
    if (argc < 3) {
        qWarning() << argv[0] << "corpus.txt outfile.trigram";
        return -1;
    }

    QFile file(QString::fromLocal8Bit(argv[1]));
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        qWarning() << "Unable to open corpus:" << argv[1];
        return -1;
    }
    QTextStream stream(&file);
    stream.setCodec("UTF-8");


    QFile outFile(QString::fromLocal8Bit(argv[2]));
    if (!outFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Unable to open output file" << argv[2];
        return -1;
    }

    QHash<QString,int> model;
    qDebug() << "Reading in" << file.size() << "bytes";
    QString trigram = stream.read(3);
    QString contents = stream.readAll();
    qDebug() << "finished reading!";
    qDebug() << "Building model...";
    for (int i=0; i<contents.size(); i++) {
        if (!contents[i].isPrint()) continue;
        model[trigram]++;
        trigram[0] = trigram[1];
        trigram[1] = trigram[2];
        trigram[2] = contents[i];
    }
    qDebug() << "model built!";

    qDebug() << "Sorting...";
    QMap<int, QString> orderedTrigrams;
    for (const QString &key : model.keys()) {
        const QChar* data=key.constData();
        bool hasTwoSpaces=(data[1].isSpace() && (data[0].isSpace() || data[2].isSpace()));

        if (!hasTwoSpaces) orderedTrigrams.insertMulti(model[key], key);
    }
    qDebug() << "Sorted!";

    qDebug() << "Weeding out...";
    QMap<int, QString>::iterator i = orderedTrigrams.begin();
    while (orderedTrigrams.size() > 300) {
        orderedTrigrams.erase(i);
        i++;
    }
    qDebug() << "Weeded!";

    qDebug() << "Storing...";
    i = orderedTrigrams.end();
    int count=0;
    QTextStream outStream(&outFile);
    outStream.setCodec("UTF-8");
    while (i != orderedTrigrams.begin()) {
        --i;
        outStream << *i << "\t\t\t" << count++ << '\n';
    }
}

