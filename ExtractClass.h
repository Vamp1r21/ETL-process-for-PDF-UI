#ifndef EXTRACTCLASS_H
#define EXTRACTCLASS_H

#include <QString>
#include <QMap>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <QProcess>

struct ShipTypeRule {
    QString type;
    QStringList keywords;
};

class ExtractClass
{
public:
    ExtractClass();
    ~ExtractClass();

    void ProcessFile(const QString& pdf);
    void ProcessDirectory(const QString& dirPath);

private:
    // Вспомогательные методы
    QString RemoveSpacesBetweenChars(const QString& input);
    QString RemoveIndents(const QString& content);
    bool NeedsSpaceRemoval(const QString& content);

    // Специальная обработка для дублирующихся букв
    QString FixDuplicateLetters(const QString& input);
    bool HasDuplicateLettersProblem(const QString& content);
    QString MergeBrokenFieldLines(const QString& input);

    // OCR методы
    bool IsTextMeaningful(const QString& content);
    bool RunOCR(const QString& pdf, const QString& txt);

    // CSV методы
    QString Trim(const QString& s);
    void ConvertTextToCSV(const QString& text, const QString& csvFile, const QString& baseName);
    void WriteToFile(QTextStream& out, const QString& nameFile, const QMap<QString, QString>& record);

    // Утилиты
    bool RunSystemCommand(const QString& command);
    bool FileExists(const QString& path);
    QString ReadFile(const QString& path);
    bool WriteFile(const QString& path, const QString& content);

    //Определение типа судна
    QString DetermineShipType(const QString& fullText);
    void InitializeShipTypeRules();

    // Словарь правил для определения типа
    QList<ShipTypeRule> shipTypeRules;

    // Пути
    QString tempDir;
    QString txtDir;
    QString csvDir;
};

#endif // EXTRACTCLASS_H
