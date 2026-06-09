#ifndef TRANSFORMCLASS_H
#define TRANSFORMCLASS_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QHash>
#include <QRegularExpression>
#include <QFile>

class TransformClass
{
private:
    // Вспомогательные методы
    QString ToLower(const QString& s);
    QString Trim(const QString& line);
    QString CleanGarbage(const QString& input);
    QString ExtractNumber(const QString& line);
    int ExtractFirstNumber(const QString& line);
    bool IsNumericOrMeasurement(const QString& line);
    QString FixDuplicateDelimiters(const QString& line);
    int FixYear(int year);

    // CSV
    QStringList ParseCSVLine(const QString& line);
    QString ReplacementDotToComma(const QString& line);

    // Нормализация текстовых полей
    QString NormalizeTextField(const QString& line);
    QString NormalizeCountryField(const QString& line);

    // Преобразование даты доставки(выпуска) судна
    QString MonthToNumber(const QString& month);
    QString NormalizeDate(const QString& line);

    // Преобразование длины судна
    double TransformationToMeters(double number, const QString& type);
    QString NormalizeLength(const QString& line);

    // Преобразование максимальной скорости судна
    double TransformationToKnots(double number, const QString& type);
    QString NormalizeSpeed(const QString& line);

    // Преобразование вместимости судна
    QString NormalizeGross(const QString& line);

    // Преобразование водоизмещения судна
    double TransformationToTonnes(double number, const QString& type);
    QString NormalizeDisplacement(const QString& line);

    // Engine методы
    bool IsDesignerGarbage(const QString& designer);
    QString DetectEngineDesigner(const QString& model);
    QString CleanEngineModel(QString model, const QString& designer);
    void FixEngineFields(QString& designer, QString& model);

public:
    TransformClass();
    bool TransformCSVFile(const QString& filename = "CSVFiles/allShip.csv");
};

#endif // TRANSFORMCLASS_H
