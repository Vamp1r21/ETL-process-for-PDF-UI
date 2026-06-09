#include "ExtractClass.h"
#include <QProcess>
#include <QRegularExpression>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QTemporaryDir>

ExtractClass::ExtractClass()
{
    txtDir = "TXTFiles";
    csvDir = "../../CSVFiles";
    tempDir = "ocr_workdir";

    QDir().mkdir(txtDir);
    QDir().mkdir(csvDir);

    // Инициализация словаря типов судов
    InitializeShipTypeRules();
}

ExtractClass::~ExtractClass()
{
}

// Инициализация словаря типов судов
void ExtractClass::InitializeShipTypeRules()
{
    shipTypeRules = {
        // Наливные суда
        {"Tanker", {"tanker", "crude oil", "product tanker", "chemical carrier",
                    "VLCC", "ULCC", "Suezmax", "Aframax", "oil tanker"}},
        {"LNG Carrier", {"LNG", "liquefied natural gas", "methane carrier",
                         "membrane tank", "moss tank", "LNG carrier"}},
        {"LPG Carrier", {"LPG", "liquefied petroleum gas", "LPG carrier"}},
        {"Chemical Tanker", {"chemical tanker", "IMO type 2", "IMO type 3",
                             "stainless steel tanks", "parcel tanker"}},

        // Сухогрузные суда
        {"Container Ship", {"container", "TEU", "cellular container", "boxship",
                            "feeder", "container ship", "box ship"}},
        {"Bulk Carrier", {"bulk carrier", "bulker", "ore carrier", "coal carrier",
                          "Panamax", "Capesize", "Handymax", "Handysize", "Supramax"}},
        {"Ro-Ro", {"ro-ro", "ro ro", "roll-on", "roll on", "vehicle carrier",
                   "car carrier", "PCTC", "PCTC ship", "truck ferry", "roro",
                   "car truck carrier", "vehicle deck"}},
        {"General Cargo", {"general cargo", "multi-purpose", "multipurpose",
                           "breakbulk", "break bulk", "general cargo ship"}},
        {"Reefer", {"refrigerated", "reefer", "cool storage", "cold storage",
                    "perishable", "reefer ship", "refrigerated cargo"}},
        {"Heavy-Lift", {"heavy lift", "heavy-lift", "project cargo", "heavy load",
                        "heavy lift vessel", "project carrier"}},

        // Пассажирские суда
        {"Passenger Ship", {"passenger ship", "cruise", "liner", "passenger vessel",
                            "cruise ship"}},
        {"Ferry", {"ferry", "catamaran", "ro-pax", "ropax", "fast ferry",
                   "high speed ferry", "catamaran ferry", "double-ended",
                   "double end"}},

        // Специальные суда
        {"Tug", {"tug", "tugboat", "harbour tug", "ocean tug", "towing",
                 "bollard pull", "tug boat", "pusher", "towboat"}},
        {"Supply Vessel", {"supply", "PSV", "AHTS", "anchor handling",
                           "platform supply", "offshore supply", "supply vessel"}},
        {"Dredger", {"dredger", "dredge", "hopper", "suction", "dredging"}},
        {"Fishing Vessel", {"fishing", "trawler", "seiner", "factory trawler",
                            "fish", "fishery"}},
        {"Research Vessel", {"research", "survey", "oceanographic", "hydrographic",
                             "scientific", "research vessel"}},
        {"Icebreaker", {"icebreaker", "ice breaker", "polar", "ice class 1A",
                        "polar class"}},
        {"Barge", {"barge", "pontoon", "deck cargo barge"}},
        {"Yacht", {"yacht", "superyacht", "mega yacht", "pleasure craft"}},

        // Военные корабли
        {"Naval Vessel", {"naval", "warship", "frigate", "destroyer", "corvette",
                          "patrol", "military", "navy"}},

        // Технический флот
        {"Cable Layer", {"cable layer", "cable ship", "cable laying"}},
        {"Pipe Layer", {"pipe layer", "pipe laying", "pipelay"}},
        {"Drill Ship", {"drill ship", "drillship", "drilling vessel"}},
        {"FPSO", {"FPSO", "floating production", "FSO"}},

        // Прочие
        {"Livestock Carrier", {"livestock", "cattle", "sheep carrier"}},
        {"Bunkering Tanker", {"bunkering", "bunker", "bunkering tanker"}},
        {"Pilot Boat", {"pilot boat", "pilot vessel"}},
        {"Fire Fighting Vessel", {"fire fighting", "firefighting", "fire boat"}},
        {"Salvage Vessel", {"salvage", "salvage vessel"}},
        {"Buoy Tender", {"buoy tender", "buoy layer"}},
        {"Mooring Launch", {"mooring", "mooring launch"}}
    };
}

// Определение типа судна по первому совпадению в тексте
// Определение типа судна по первому совпадению в тексте
QString ExtractClass::DetermineShipType(const QString& fullText)
{
    // Разбиваем текст на строки
    QStringList lines = fullText.split('\n');

    // Структура для хранения найденного совпадения
    struct MatchInfo {
        QString type;
        int lineNumber;
        int position;
    };

    MatchInfo bestMatch;
    bestMatch.lineNumber = -1;
    bestMatch.position = -1;

    // Проходим по всем строкам
    for (int lineNum = 0; lineNum < lines.size(); lineNum++) {
        QString line = lines[lineNum];

        // Проверяем, есть ли в строке двоеточие
        if (!line.contains(":")) {
            continue;  // Пропускаем строки без двоеточия
        }

        QString lineLower = line.toLower();

        // Проходим по всем правилам
        for (const ShipTypeRule& rule : shipTypeRules) {
            // Ищем ключевые слова в этой строке
            for (const QString& keyword : rule.keywords) {
                int pos = lineLower.indexOf(keyword.toLower());

                // Если слово найдено в этой строке
                if (pos != -1) {
                    // Если это первое найденное совпадение
                    if (bestMatch.lineNumber == -1) {
                        bestMatch.type = rule.type;
                        bestMatch.lineNumber = lineNum;
                        bestMatch.position = pos;
                    }
                    // Если совпадение в той же строке, но раньше
                    else if (lineNum == bestMatch.lineNumber && pos < bestMatch.position) {
                        bestMatch.type = rule.type;
                        bestMatch.position = pos;
                    }
                    // Если совпадение в более ранней строке
                    else if (lineNum < bestMatch.lineNumber) {
                        bestMatch.type = rule.type;
                        bestMatch.lineNumber = lineNum;
                        bestMatch.position = pos;
                    }
                }
            }
        }
    }

    // Если ничего не найдено - возвращаем пустую строку
    if (bestMatch.lineNumber == -1) {
        return QString();
    }

    return bestMatch.type;
}

// Удаление лишних пробелов между символами
QString ExtractClass::RemoveSpacesBetweenChars(const QString& input)
{
    QString result;
    result.reserve(input.length());

    for (int i = 0; i < input.length(); i++) {
        QChar c = input[i];

        if (c == ' ') {
            bool hasCharBefore = (i > 0) &&
                                 (input[i-1].isLetterOrNumber() || input[i-1] == '.' ||
                                  input[i-1] == ',' || input[i-1] == '(' || input[i-1] == ')' || input[i-1] == '/');
            bool hasCharAfter = (i < input.length() - 1) &&
                                (input[i+1].isLetterOrNumber() || input[i+1] == '.' ||
                                 input[i+1] == ',' || input[i+1] == '(' || input[i+1] == ')');

            if (hasCharBefore && hasCharAfter) {
                continue;
            } else {
                result += c;
            }
        } else {
            result += c;
        }
    }

    return result;
}

// Удаление отступов
QString ExtractClass::RemoveIndents(const QString& content)
{
    QString result;
    QStringList lines = content.split('\n');
    QString prevLine;

    for (const QString& line : lines) {
        bool shouldRemoveNewline = false;

        if (!prevLine.isEmpty()) {
            QChar lastChar = prevLine.back();
            if (lastChar == ',' || lastChar == '/') {
                shouldRemoveNewline = true;
            }
        }

        if (!line.isEmpty() && !shouldRemoveNewline) {
            QChar firstChar = line.front();
            if (!firstChar.isUpper()) {
                shouldRemoveNewline = true;
            }
        }

        if (shouldRemoveNewline) {
            if (!result.isEmpty() && result.back() != ' ') {
                result += ' ';
            }
            result += line;
        } else {
            if (!result.isEmpty()) {
                result += '\n';
            }
            result += line;
        }

        prevLine = line;
    }

    return result;
}

// Автоматическое определения необходимости удаления пробелов
bool ExtractClass::NeedsSpaceRemoval(const QString& content)
{
    int spaceBetweenCharsCount = 0;
    int totalCharPairs = 0;

    for (int i = 1; i < content.length() - 1; i++) {
        QChar prev = content[i-1];
        QChar curr = content[i];
        QChar next = content[i+1];

        if (curr == ' ' && prev.isLetterOrNumber() && next.isLetterOrNumber()) {
            spaceBetweenCharsCount++;
        } else if (prev.isLetterOrNumber() && next.isLetterOrNumber()) {
            totalCharPairs++;
        }
    }

    if (totalCharPairs > 0) {
        double ratio = static_cast<double>(spaceBetweenCharsCount) / totalCharPairs;
        return ratio > 0.3;
    }

    return false;
}

// Проверка, является ли текст читаемым
bool ExtractClass::IsTextMeaningful(const QString& content)
{
    int alphaCount = 0;
    for (QChar c : content) {
        if (c.isLetter()) {
            alphaCount++;
        }
    }
    return alphaCount > 20000;
}

// Выполнение системной команды
bool ExtractClass::RunSystemCommand(const QString& command)
{
    return QProcess::execute(command) == 0;
}

// Проверка существования файла
bool ExtractClass::FileExists(const QString& path)
{
    return QFile::exists(path);
}

// Чтение файла
QString ExtractClass::ReadFile(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }
    QTextStream in(&file);
    return in.readAll();
}

// Запись файла
bool ExtractClass::WriteFile(const QString& path, const QString& content)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream out(&file);
    out << content;
    return true;
}

// OCR процесс
bool ExtractClass::RunOCR(const QString& pdf, const QString& txt)
{
    const QString imgPrefix = "ocr_temp_pg";

    QDir().mkdir(tempDir);

    QString pdfToImg = QString("pdftoppm -q -r 200 -gray \"%1\" %2/%3")
                           .arg(pdf)
                           .arg(tempDir)
                           .arg(imgPrefix);

    if (!RunSystemCommand(pdfToImg)) {
        qDebug() << "Ошибка при конвертации PDF в изображения";
        return false;
    }

    QStringList pages;
    QDir tempDirObj(tempDir);
    QStringList filters;
    filters << "*.pgm" << "*.ppm";
    QFileInfoList entries = tempDirObj.entryInfoList(filters, QDir::Files);

    for (const QFileInfo& entry : entries) {
        pages.append(entry.absoluteFilePath());
    }

    pages.sort();

    QFile outFile(txt);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "Не удалось открыть файл для записи:" << txt;
        return false;
    }
    QTextStream out(&outFile);

    bool processed = false;
    QRegularExpression regex("[ce!\\.\\-]{4,}");

    for (const QString& pageImg : pages) {
        QString ocrCmd = QString("tesseract \"%1\" stdout -l eng --psm 3 --oem 3 quiet")
        .arg(pageImg);

        QProcess process;
        process.start(ocrCmd);
        process.waitForFinished(-1);

        if (process.exitCode() == 0) {
            QString pageContent = process.readAllStandardOutput();
            pageContent.replace(regex, " ");
            out << pageContent << "\n\f\n";
            processed = true;
        }

        QFile::remove(pageImg);
    }

    outFile.close();
    QDir(tempDir).removeRecursively();

    return processed;
}

// Убираем лишние пробелы
QString ExtractClass::Trim(const QString& s)
{
    int start = 0;
    while (start < s.length() && (s[start] == ' ' || s[start] == '\t' || s[start] == '.')) {
        start++;
    }

    int end = s.length() - 1;
    while (end >= start && (s[end] == ' ' || s[end] == '\t' || s[end] == '.')) {
        end--;
    }

    if (start > end) {
        return QString();
    }

    return s.mid(start, end - start + 1);
}

// Добавление в файл
void ExtractClass::WriteToFile(QTextStream& out, const QString& nameFile,
                               const QMap<QString, QString>& record)
{
    out << "\"" << nameFile << "\","
        << "\"" << record.value("VesselName") << "\","
        << "\"" << record.value("Builder") << "\","
        << "\"" << record.value("Designer") << "\","
        << "\"" << record.value("OwnerOperator") << "\","
        << "\"" << record.value("Country") << "\","
        << "\"" << record.value("DeliveryDate") << "\","
        << "\"" << record.value("Length") << "\","
        << "\"" << record.value("MaxSpeed") << "\","
        << "\"" << record.value("IMONumber") << "\","
        << "\"" << record.value("Gross") << "\","
        << "\"" << record.value("SisterShip") << "\","
        << "\"" << record.value("Displacement") << "\","
        << "\"" << record.value("MainEngineDesign") << "\","
        << "\"" << record.value("MainEngineModel") << "\","
        << "\"" << record.value("NumberOfEngines") << "\","
        << "\"" << record.value("ShipType") << "\"\n";
}

// Исправление дублирования букв
QString ExtractClass::FixDuplicateLetters(const QString& input)
{
    QString result;
    QStringList lines = input.split('\n');

    for (const QString& line : lines) {
        QString fixedLine;

        for (int i = 0; i < line.length(); i++) {
            if (line[i] == '.') {
                fixedLine += line[i];
                continue;
            }

            if (line[i].isLetterOrNumber()) {
                if (i + 2 < line.length() &&
                    line[i+1] == ' ' &&
                    line[i+2] == line[i]) {
                    fixedLine += line[i];
                    i += 2;
                    continue;
                }

                if (i + 2 < line.length() &&
                    line[i+1] == ' ' &&
                    line[i+2].isLetterOrNumber()) {
                    fixedLine += line[i];
                    i += 1;
                    continue;
                }

                if (i + 2 < line.length() &&
                    line[i+1] == line[i] &&
                    line[i+2] == line[i]) {
                    fixedLine += line[i];
                    fixedLine += line[i];
                    i += 2;
                    continue;
                }
            }

            fixedLine += line[i];
        }

        QString cleanedLine;
        bool lastWasSpace = false;
        for (QChar c : fixedLine) {
            if (c == ' ') {
                if (!lastWasSpace) {
                    cleanedLine += c;
                    lastWasSpace = true;
                }
            } else {
                cleanedLine += c;
                lastWasSpace = false;
            }
        }

        if (!result.isEmpty()) {
            result += "\n";
        }
        result += cleanedLine;
    }

    return result;
}

// Проверка наличия проблемы с дублирующимися буквами
bool ExtractClass::HasDuplicateLettersProblem(const QString& content)
{
    QRegularExpression regex(R"(([A-Za-z])\s+([A-Za-z])\s+([A-Za-z])\s+([A-Za-z]))");
    int count = 0;

    QRegularExpressionMatchIterator i = regex.globalMatch(content);
    while (i.hasNext()) {
        i.next();
        count++;
        if (count >= 2) {
            return true;
        }
    }

    return false;
}

// Объединение разорванных строк
QString ExtractClass::MergeBrokenFieldLines(const QString& input)
{
    QStringList lines = input.split('\n');
    QString currentField;
    QStringList result;

    QStringList fieldMarkers = {
        "Shipbuilder", "Builder", "Vessel", "Deliverydate",
        "IMO number", "Owner", "Operator", "Designer"
    };

    auto startsWithMarker = [&](const QString& line) {
        for (const QString& marker : fieldMarkers) {
            if (line.startsWith(marker, Qt::CaseInsensitive)) {
                return true;
            }
        }
        return false;
    };

    auto isNewFieldLine = [&](const QString& line) {
        int colonPos = line.indexOf(':');
        if (colonPos != -1 && colonPos < 25) return true;

        if (line.contains("....")) return true;
        if (startsWithMarker(line)) return true;

        return false;
    };

    auto isIndented = [](const QString& line) {
        return !line.isEmpty() && line[0].isSpace();
    };

    auto endsLikeCompleteSentence = [](const QString& line) {
        if (line.isEmpty()) return true;
        QChar last = line.back();
        return last == '.' || last == ':' || last == ';';
    };

    auto shouldMerge = [&](const QString& prev, const QString& current) {
        if (isIndented(current)) return true;
        if (!endsLikeCompleteSentence(prev)) return true;
        return false;
    };

    for (const QString& line : lines) {
        if (line.isEmpty()) {
            if (!currentField.isEmpty()) {
                result.append(currentField);
                currentField.clear();
            }
            result.append("");
            continue;
        }

        if (isNewFieldLine(line)) {
            if (!currentField.isEmpty()) {
                result.append(currentField);
            }
            currentField = line;
        } else {
            if (!currentField.isEmpty() && shouldMerge(currentField, line)) {
                currentField += ' ';
                currentField += line;
            } else {
                if (!currentField.isEmpty()) {
                    result.append(currentField);
                }
                currentField = line;
            }
        }
    }

    if (!currentField.isEmpty()) {
        result.append(currentField);
    }

    return result.join('\n');
}

void ExtractClass::ConvertTextToCSV(const QString& text, const QString& csvFile,
                                    const QString& baseName)
{
    QFile allShipFile(csvDir + "/allShip.csv");
    allShipFile.open(QIODevice::Append | QIODevice::Text);
    QTextStream allShipOut(&allShipFile);

    QFile csvOutFile(csvFile);
    if (!csvOutFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Не удалось создать CSV:" << csvFile;
        return;
    }
    QTextStream out(&csvOutFile);

    out << "FileName,VesselName,Builder,Designer,OwnerOperator,Country,DeliveryDate,"
           "Length(m),MaxSpeed(knots),ImoNumber,Gross,SisterShip,Displacement(t),"
           "MainEngineDesigner,MainEngineModel,NumberOfEngines,ShipType\n";

    QStringList lines = text.split('\n');
    QMap<QString, QString> record;

    // Буфер для накопления текста текущей записи
    QString currentRecordText;

    int designer = 0;
    int i = 0;
    int mainEngine = 0;
    int vessel = 0;
    int engine = 0;
    int model = 0;
    int number = 0;
    int flag = 0;

    for (const QString& line : lines) {
        // Добавляем строку в текст текущей записи
        currentRecordText += line + "\n";

        if (line.contains("Main engine") || line.contains("Mainengine") ||
            line.contains("Main Engine") || line.contains("MainEngine")) {
            mainEngine++;
        }

        if (!line.contains(":") && !line.contains("..")) {
            continue;
        }

        int pos = line.indexOf(":");
        if (pos == -1) pos = line.indexOf("..");
        if (pos == -1) pos = line.indexOf(",,,");
        if (pos == -1) continue;

        QString key = Trim(line.left(pos));
        QString value = Trim(line.mid(pos + 1));

        // Название судна
        if (key.contains("Vessel") && vessel == 0) {
            vessel++;
            record["VesselName"] = value;
        }
        else if (key.contains("Builder", Qt::CaseInsensitive) ||
                 key.contains("Shipbuilder", Qt::CaseInsensitive) ||
                 key.contains("Builder's name", Qt::CaseInsensitive)) {
            if (!record.isEmpty() && !record["Builder"].isEmpty()) {
                // НОВОЕ: Определяем тип по тексту ТОЛЬКО этой записи
                record["ShipType"] = DetermineShipType(currentRecordText);

                vessel = 0;
                mainEngine = 0;
                designer = 0;
                engine = 0;
                model = 0;
                number = 0;
                flag =0;
                i = 0;
                if (record["VesselName"].isEmpty()) {
                    record.clear();
                    record["Builder"] = value;
                    currentRecordText = line + "\n";  // Начинаем новый буфер
                    continue;
                }
                WriteToFile(out, baseName, record);
                WriteToFile(allShipOut, baseName, record);
                record.clear();
                currentRecordText.clear();  // Очищаем буфер
            }
            record["Builder"] = value;
            continue;
        }
        // Проектировщик судна
        else if ((key.contains("Designer", Qt::CaseInsensitive) ||
                  key.contains("Design", Qt::CaseInsensitive)) && designer <= 0) {
            record["Designer"] = value;
            designer++;
        }
        // Владелец судна
        else if (key.contains("Owner", Qt::CaseInsensitive) ||
                 key.contains("Operator", Qt::CaseInsensitive)) {
            record["OwnerOperator"] = value;
        }
        // Страна
        else if ((key.contains("Country") ||
                 key.contains("Flag")) && flag == 0) {
            record["Country"] = value;
            flag++;
        }
        // Дата доставки
        else if (key.contains("Delivery date", Qt::CaseInsensitive) ||
                 key.contains("Deliverydate", Qt::CaseInsensitive)) {
            record["DeliveryDate"] = value;
        }
        // Длина судна - КОНТРОЛЬНАЯ ТОЧКА
        else if (key.contains("Length, oa", Qt::CaseInsensitive) ||
                 key.contains("Length oa", Qt::CaseInsensitive) ||
                 key.contains("Length,oa", Qt::CaseInsensitive) ||
                 key.contains("Lengthoa", Qt::CaseInsensitive)) {
            if (!record.isEmpty() && !record["Length"].isEmpty()) {
                record["ShipType"] = DetermineShipType(currentRecordText);

                mainEngine = 0;
                designer = -100;
                i = 0;
                engine = 0;
                model = 0;
                vessel = 0;
                number = 0;
                flag=0;
                if (record["VesselName"].isEmpty()) {
                    record.clear();
                    record["Length"] = value;
                    currentRecordText = line + "\n";  // Начинаем новый буфер
                    continue;
                }
                WriteToFile(out, baseName, record);
                WriteToFile(allShipOut, baseName, record);
                record.clear();
                currentRecordText.clear();  // Очищаем буфер
            }
            record["Length"] = value;
            continue;
        }
        // Максимальная скорость
        else if (key.contains("Max speed", Qt::CaseInsensitive) ||
                 key.contains("Maximum speed", Qt::CaseInsensitive) ||
                 key.contains("Speed service 2", Qt::CaseInsensitive) ||
                 key.contains("Speed, service", Qt::CaseInsensitive) ||
                 key.contains("Speed,service", Qt::CaseInsensitive) ||
                 key.contains("Maxspeed", Qt::CaseInsensitive) ||
                 key.contains("Maximumspeed", Qt::CaseInsensitive) ||
                 key.contains("Speedservice 2", Qt::CaseInsensitive) ||
                 key.contains("Speed (light load)", Qt::CaseInsensitive)) {
            record["MaxSpeed"] = value;
        }
        else if (key.contains("IMO number", Qt::CaseInsensitive)) {
            record["IMONumber"] = value;
        }
        // Вместимость
        else if (key.contains("Gross", Qt::CaseInsensitive)) {
            record["Gross"] = value;
        }
        // Число кораблей сестер
        else if (key.contains("Total number of sister ships already completed", Qt::CaseInsensitive)) {
            record["SisterShip"] = value;
        }
        // Водоизмещение
        else if (key.contains("Displacement", Qt::CaseInsensitive)) {
            record["Displacement"] = value;
        }
        // Производитель двигателя
        else if ((key.contains("Design", Qt::CaseInsensitive) ||
                  key.contains("Make", Qt::CaseInsensitive)) && mainEngine > 0 && engine == 0) {
            engine++;
            record["MainEngineDesign"] = value;
        }
        // Модель двигателя
        else if (key.contains("Model", Qt::CaseInsensitive) && mainEngine > 0 && model == 0) {
            model++;
            record["MainEngineModel"] = value;
        }
        else if ((key.contains("Number of engines", Qt::CaseInsensitive) ||
                  key.contains("Number", Qt::CaseInsensitive)) && mainEngine > 0 && number == 0) {
            number++;
            record["NumberOfEngines"] = value;
        }

        if (mainEngine > 0) {
            i++;
        }
    }

    if (record["VesselName"].isEmpty()) {
        record.clear();
    }

    if (!record.isEmpty()) {
        // НОВОЕ: Определяем тип для последней записи
        record["ShipType"] = DetermineShipType(currentRecordText);

        WriteToFile(out, baseName, record);
        WriteToFile(allShipOut, baseName, record);
        record.clear();
    }

    csvOutFile.close();
    allShipFile.close();
}

void ExtractClass::ProcessFile(const QString& pdf)
{
    QFileInfo pdfInfo(pdf);
    QString baseName = pdfInfo.baseName();
    QString txt = txtDir + "/" + baseName + ".txt";
    QString csv = csvDir + "/" + baseName + ".csv";

    // Попытка обычного извлечения текста
    QString convertCmd = QString("pdftotext -raw -nopgbrk -enc UTF-8 \"%1\" \"%2\"")
                             .arg(pdf)
                             .arg(txt);

    if (!RunSystemCommand(convertCmd)) {
        qDebug() << "Ошибка: pdftotext для файла" << pdf;
        return;
    }

    QString content = ReadFile(txt);
    if (content.isEmpty()) {
        qDebug() << "Не удалось прочитать файл:" << txt;
        return;
    }

    // Если текста нет или он не "осмысленный" (скан), запускаем OCR
    if (!IsTextMeaningful(content)) {
        qDebug() << "PDF looks like a scan, starting OCR (English)...";
        if (!RunOCR(pdf, txt)) {
            qDebug() << "Ошибка OCR для файла:" << pdf;
            return;
        }

        content = ReadFile(txt);
        if (content.isEmpty()) {
            qDebug() << "Не удалось прочитать результат OCR";
            return;
        }
    }

    if (content.isEmpty()) {
        qDebug() << "Предупреждение: Текст не обнаружен даже после OCR.";
        return;
    }

    QString cleanedContent = RemoveIndents(content);

    if (NeedsSpaceRemoval(cleanedContent)) {
        cleanedContent = RemoveSpacesBetweenChars(cleanedContent);
    } else if (HasDuplicateLettersProblem(cleanedContent)) {
        cleanedContent = MergeBrokenFieldLines(cleanedContent);
        cleanedContent = FixDuplicateLetters(cleanedContent);
    }

    ConvertTextToCSV(cleanedContent, csv, baseName);

    if (!WriteFile(txt, cleanedContent)) {
        qDebug() << "Не удалось записать файл:" << txt;
        return;
    }

    qDebug() << "Успешно обработано:" << pdf;
}

void ExtractClass::ProcessDirectory(const QString& dirPath)
{
    QDir dir(dirPath);
    QStringList filters;
    filters << "*.pdf" << "*.PDF";
    QStringList pdfFiles = dir.entryList(filters, QDir::Files);

    qDebug() << "Найдено PDF файлов:" << pdfFiles.size();

    for (const QString& pdfFile : pdfFiles) {
        ProcessFile(dirPath + "/" + pdfFile);
    }
}
