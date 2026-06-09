#include "transformclass.h"
#include <QDebug>
#include <QTextStream>
#include <QSet>
#include <QHash>
#include <QRegularExpression>

// ---------------- ENGINE BRANDS ALIASES ----------------
// Структура для приведения совпадающих/сокращенных названий к единому производителю
struct BrandAlias {
    QString canonical;
    QVector<QString> aliases;
};

static const QVector<BrandAlias> ENGINE_BRAND_ALIASES = {
    {"MAN B&W", {"man b&w", "man b & w", "man-b&w", "man", "b&w", "b & w"}},
    {"Wärtsilä", {"wärtsilä", "wartsila"}},
    {"Anglo Belgian Corporation", {"anglo belgian corporation", "abc", "anglobelgian"}},
    {"Rolls-Royce", {"rolls-royce", "rolls royce"}},
    {"Sulzer", {"sulzer"}},
    {"WinGD", {"wingd", "winterthur"}},
    {"MTU", {"mtu"}},
    {"Caterpillar", {"caterpillar", "cat", "mak"}},
    {"Cummins", {"cummins"}},
    {"Deutz", {"deutz"}},
    {"Yanmar", {"yanmar"}},
    {"Hyundai", {"hyundai", "hhi"}},
    {"Mitsubishi", {"mitsubishi", "mhi"}},
    {"Niigata", {"niigata"}}
};

// ---------------- COUNTRY ALIASES ----------------
// Словарь для приведения синонимов стран к единому стандарту (ГОСТ-стиль)
struct CountryAlias {
    QString canonical;
    QVector<QString> aliases;
};

static const QVector<CountryAlias> COUNTRY_ALIASES = {
    {"United Kingdom", {"uk", "u.k.", "united kingdom", "great britain", "england"}},
    {"United States",  {"usa", "u.s.a.", "united states", "united states of america"}},
    {"South Korea",    {"south korea", "korea", "republic of korea", "korea (south)"}},
    {"China",          {"china", "prc", "peoples republic of china"}},
    {"Russia",         {"russia", "russian federation", "rf"}}
};

TransformClass::TransformClass()
{
}

// ---------------- UTILS ----------------
QString TransformClass::ToLower(const QString& s)
{
    return s.toLower();
}

QString TransformClass::Trim(const QString& line)
{
    return line.trimmed();
}

QString TransformClass::CleanGarbage(const QString& input)
{
    QString result = input;

    // убираем многоточия и мусор
    result.replace(QRegularExpression(R"([\.…]{2,})"), " ");

    // Исправление проблемы: "Marine & & Engineering" -> "Marine & Engineering"
    result.replace(QRegularExpression(R"(&(\s*&)+)"), "&");

    // Исправление проблемы: "( (Astillero Barreras) )" -> "(Astillero Barreras)"
    result.replace(QRegularExpression(R"(\((\s*\()+)"), "(");
    result.replace(QRegularExpression(R"(\)(\s*\))+)"), ")");

    // убираем мусор в самом начале строки
    result.remove(QRegularExpression(R"(^[\s,:;./&]+)"));

    // убираем повторяющиеся разделители
    result.replace(QRegularExpression(R"([,;:/]{2,})"), ",");

    // убираем "висячие" скобки
    int open = result.count('(');
    int close = result.count(')');
    if (close > open)
        result.remove(QRegularExpression(R"(\))"));
    else if (open > close)
        result.remove(QRegularExpression(R"(\()"));

    // убираем мусор в конце строки
    result.remove(QRegularExpression(R"([\s,:;./&]+$)"));

    // нормализация пробелов
    result.replace(QRegularExpression(R"(\s+)"), " ");

    return Trim(result);
}

QString TransformClass::ExtractNumber(const QString& line)
{
    QString result;
    for (QChar c : line) {
        if (c.isDigit() || c == '.') result += c;
    }
    return result;
}

int TransformClass::ExtractFirstNumber(const QString& line)
{
    QRegularExpressionMatch match;
    QRegularExpression r(R"(\d+)");

    if ((match = r.match(line)).hasMatch())
        return match.captured(0).toInt();

    return -1;
}

QString TransformClass::FixDuplicateDelimiters(const QString& line)
{
    QString result = line;

    result.replace(QRegularExpression(R"([\.…]{2,})"), " ");
    result.replace(QRegularExpression(R"([,;:/]{2,})"), ",");
    result.replace(QRegularExpression(R"(\s+)"), " ");

    return result;
}

int TransformClass::FixYear(int year)
{
    int currentYear = 2026;

    if (year < 1998 || year > currentYear) {
        QString y = QString::number(year);
        if (y.length() >= 2) {
            int firstTwo = y.left(2).toInt();
            return 2000 + firstTwo;
        }
    }
    return year;
}

// ---------------- CSV ----------------
QStringList TransformClass::ParseCSVLine(const QString& line)
{
    QStringList result;
    QString field;
    bool inQuotes = false;

    for (int i = 0; i < line.length(); ++i) {
        QChar c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes;
        }
        else if (c == ',' && !inQuotes) {
            result.append(field);
            field.clear();
        }
        else {
            field += c;
        }
    }
    result.append(field);
    return result;
}

QString TransformClass::ReplacementDotToComma(const QString& line)
{
    if (line.length() == 10 && line[2] == '.' && line[5] == '.') {
        return line;
    }
    QString result = line;
    return result.replace('.', ',');
}

// ---------------- NORMALIZE TEXT FIELD ----------------
QString TransformClass::NormalizeTextField(const QString& line)
{
    if (line.isEmpty()) return "N/A";

    QString result = CleanGarbage(line);

    // Если есть запятые — берем самую длинную часть
    if (result.contains(',')) {
        QStringList parts = result.split(',', Qt::SkipEmptyParts);
        QString longest = "";
        for (const QString& part : parts) {
            QString p = part.trimmed();
            if (p.length() > longest.length()) {
                longest = p;
            }
        }
        result = longest;
    }

    if (result.count(QRegularExpression(R"([^-\s])")) == 0)
        return "N/A";

    result.replace(QRegularExpression(R"(-{2,})"), "-");
    result.replace(QRegularExpression(R"([/\\])"), " ");
    result.replace(QRegularExpression(R"(,)"), " ");
    result.replace(QRegularExpression(R"(\s+)"), " ");

    result = Trim(result);

    if (result.isEmpty()) return "N/A";

    return result;
}

// ---------------- NORMALIZE COUNTRY FIELD ----------------
// Исправлено: сохраняет составные названия (United Kingdom) и маппит синонимы (UK -> United Kingdom)
QString TransformClass::NormalizeCountryField(const QString& line)
{
    if (line.isEmpty()) return "N/A";
    QString result = line;

    // Убираем мусорные символы в начале/конце
    result.remove(QRegularExpression(R"(^[\s,:;./&+\-]+)"));
    result.remove(QRegularExpression(R"([\s,:;./&+\-]+$)"));

    if (result.count(QRegularExpression(R"([^-\s])")) == 0) return "N/A";

    result.replace(QRegularExpression(R"(\.{2,})"), " ");
    result.replace(QRegularExpression(R"(-{2,})"), "-");
    result.replace(QRegularExpression(R"(\s*\(\([^)]*\)\)\s*)"), " ");
    result.replace(QRegularExpression(R"(\s*\([^)]*\)\s*)"), " ");

    // Удаляем упоминания флагов
    result.remove(QRegularExpression(R"(\s*Flag\s*)", QRegularExpression::CaseInsensitiveOption));
    result.replace(QRegularExpression(R"(\s+)"), " ");
    result = Trim(result);

    // Режем строго по знакам препинания, если указано несколько стран (пробелы не трогаем!)
    int pos = result.indexOf(QRegularExpression(R"([,;/])"));
    if (pos != -1) result = result.left(pos);
    result = Trim(result);

    if (result.isEmpty() || result == ":" || result == "-") return "N/A";

    // Унификация стран через словарь алиасов
    QString lowerCountry = result.toLower();
    for (const auto& country : COUNTRY_ALIASES) {
        for (const QString& alias : country.aliases) {
            if (lowerCountry == alias || lowerCountry == alias.toLower()) {
                return country.canonical;
            }
        }
    }

    return result;
}

// ---------------- IS NUMERIC OR MEASUREMENT ----------------
bool TransformClass::IsNumericOrMeasurement(const QString& line)
{
    if (line.isEmpty()) return true;

    QString trimmed = Trim(line);
    QStringList patterns = {
        R"(^\s*\d+\s*[-–]?\s*(stroke|cylinder|engine|cyl|strokes?)\s*$)",
        R"(^\s*\d+\s*(stroke|cylinder|engine|cyl|strokes?)\s*$)",
        R"(^\s*\d+(?:[.,]\d+)?\s*(?:m|ft|mm|cm|km|meters?|metres?|feet?)\s*$)",
        R"(^\s*\d+(?:[.,]\d+)?\s*[xх]\s*\d+(?:[.,]\d+)?(?:\s*[xх]\s*\d+(?:[.,]\d+)?)?\s*$)",
        R"(^\s*\d+(?:[.,]\d+)?\s*(?:tonnes?|t|kg|mt|ton)\s*$)",
        R"(^\s*\d+\s*[/]\s*[a-z]+\s*$)",
        R"(^\s*\d+\s*[/]\s*[a-z]+\s*\d*\s*$)",
        R"(^\s*\d+\s*[,;]\s*[A-Za-z])",
        R"(^\s*\d+\s+[A-Za-z].*\d*\s*$)",
        R"(^\s*\d+(?:[.,]\d+)?\s*[-–]\s*\d+(?:[.,]\d+)?\s*$)",
        R"(^\s*\d+(?:[.,;]\s*\d+)+\s*)",
        R"(^\s*[\d\.,]+\s*$)",
        R"(^\s*\d+(?:[.,]\d+)?\s*[xх]\s*\d+(?:[.,]\d+)?(?:\s+\d+(?:[.,]\d+)?\s*[xх]\s*\d+(?:[.,]\d+)?)+\s*$)",
        R"(^\s*\d+\s*,\s+[A-Za-z])",
        R"(^\s*\d+\s*/\s*[a-z]+\s*\d*\s*$)",
    };

    for (const QString& pattern : patterns) {
        QRegularExpression r(pattern, QRegularExpression::CaseInsensitiveOption);
        if (r.match(trimmed).hasMatch()) return true;
    }

    if (!trimmed.isEmpty() && trimmed[0].isDigit()) {
        int letterCount = 0;
        bool hasDigit = false;
        bool hasSlash = false;

        for (QChar c : trimmed) {
            if (c.isLetter()) letterCount++;
            if (c.isDigit()) hasDigit = true;
            if (c == '/') hasSlash = true;
        }

        if (hasSlash && letterCount > 0) return true;
        if (letterCount <= 3 && hasDigit) return true;
        if ((trimmed.contains('x')) && hasDigit) return true;
    }
    return false;
}

// ---------------- DATE NORMALIZATION ----------------
QString TransformClass::MonthToNumber(const QString& month)
{
    QString lowerMonth = month.toLower();
    static QHash<QString, QString> mm;

    if (mm.isEmpty()) {
        mm["january"] = "01"; mm["jan"] = "01";
        mm["february"] = "02"; mm["feb"] = "02";
        mm["march"] = "03"; mm["mar"] = "03";
        mm["april"] = "04"; mm["apr"] = "04";
        mm["may"] = "05";
        mm["june"] = "06"; mm["jun"] = "06";
        mm["july"] = "07"; mm["jul"] = "07";
        mm["august"] = "08"; mm["aug"] = "08";
        mm["september"] = "09"; mm["sep"] = "09";
        mm["october"] = "10"; mm["oct"] = "10";
        mm["november"] = "11"; mm["nov"] = "11";
        mm["december"] = "12"; mm["dec"] = "12";
    }
    return mm.value(lowerMonth, "");
}

QString TransformClass::NormalizeDate(const QString& line)
{
    if (line.isEmpty() || line == "N/A") return "";

    QString cleaned = line;
    cleaned.remove(QRegularExpression(R"(completed|delivered|built)", QRegularExpression::CaseInsensitiveOption));
    cleaned.replace(QRegularExpression(R"((\d{1,2})(st|nd|rd|th))"), "\\1");
    cleaned.replace(QRegularExpression(R"(([A-Za-z]+)(\d{4}))"), "\\1 \\2");
    cleaned.replace(QRegularExpression(R"((\d{1,2})([A-Za-z]+))"), "\\1 \\2");
    cleaned.replace(QRegularExpression(R"((\d{4})([A-Za-z]+))"), "\\2 \\1");
    cleaned.replace(QRegularExpression(R"(([A-Za-z]+)and([A-Za-z]+)\s*(\d{4}))", QRegularExpression::CaseInsensitiveOption), "\\1 \\3");

    QRegularExpressionMatch match;

    QRegularExpression r1(R"((\d{4})[-./](\d{2})[-./](\d{2}))");
    if ((match = r1.match(cleaned)).hasMatch()) {
        return match.captured(3) + "." + match.captured(2) + "." + QString::number(FixYear(match.captured(1).toInt()));
    }

    QRegularExpression r2(R"((\d{2})[-./](\d{2})[-./](\d{4}))");
    if ((match = r2.match(cleaned)).hasMatch()) {
        return match.captured(1) + "." + match.captured(2) + "." + QString::number(FixYear(match.captured(3).toInt()));
    }

    QRegularExpression r3(R"((\d{1,2})\s+([A-Za-z]+)\s+(\d{4}))", QRegularExpression::CaseInsensitiveOption);
    if ((match = r3.match(cleaned)).hasMatch()) {
        QString mm = MonthToNumber(match.captured(2));
        if (!mm.isEmpty()) {
            QString dd = match.captured(1);
            if (dd.length() == 1) dd = "0" + dd;
            return dd + "." + mm + "." + QString::number(FixYear(match.captured(3).toInt()));
        }
    }

    QRegularExpression r4(R"(([A-Za-z]+)\s+(\d{1,2})\s+(\d{4}))", QRegularExpression::CaseInsensitiveOption);
    if ((match = r4.match(cleaned)).hasMatch()) {
        QString mm = MonthToNumber(match.captured(1));
        if (!mm.isEmpty()) {
            QString dd = match.captured(2);
            if (dd.length() == 1) dd = "0" + dd;
            return dd + "." + mm + "." + QString::number(FixYear(match.captured(3).toInt()));
        }
    }

    QRegularExpression r_late(R"(late\s*(\d{4}))", QRegularExpression::CaseInsensitiveOption);
    if ((match = r_late.match(cleaned)).hasMatch()) {
        return "01.10." + QString::number(FixYear(match.captured(1).toInt()));
    }

    QRegularExpression r_month_year(R"(([A-Za-z]+)\s+(\d{4}))", QRegularExpression::CaseInsensitiveOption);
    if ((match = r_month_year.match(cleaned)).hasMatch()) {
        QString mm = MonthToNumber(match.captured(1));
        if (!mm.isEmpty()) return "01." + mm + "." + QString::number(FixYear(match.captured(2).toInt()));
    }

    QRegularExpression r_year(R"(^\s*(\d{4})\s*$)");
    if ((match = r_year.match(cleaned)).hasMatch()) {
        return "01.01." + QString::number(FixYear(match.captured(1).toInt()));
    }

    return "";
}

// ---------------- LENGTH ----------------
double TransformClass::TransformationToMeters(double number, const QString& type)
{
    if (type == "ft") return number * 0.3048;
    return number;
}

QString TransformClass::NormalizeLength(const QString& line)
{
    QString cleaned = line;
    cleaned.replace(QRegularExpression(R"(,(\d+))"), "\\1");
    cleaned.replace(QRegularExpression(R"((\d+),(\d+))"), "\\1.\\2");

    QRegularExpression r(R"((\d+(\.\d+)?)\s*(m|ft))", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match;

    if ((match = r.match(cleaned)).hasMatch()) {
        double number = match.captured(1).toDouble();
        QString type = ToLower(match.captured(3));
        return QString::number(TransformationToMeters(number, type), 'f', 2);
    }
    return ExtractNumber(cleaned);
}

// ---------------- SPEED ----------------
double TransformClass::TransformationToKnots(double number, const QString& type)
{
    if (type == "km/h") return number * 0.539957;
    if (type == "mph") return number * 0.868976;
    return number;
}

QString TransformClass::NormalizeSpeed(const QString& line)
{
    QRegularExpression r(R"((\d+(\.\d+)?)\s*(kt|kn|kts|km\/h|mph))", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match;

    if ((match = r.match(line)).hasMatch()) {
        double number = match.captured(1).toDouble();
        QString type = ToLower(match.captured(3));
        return QString::number(TransformationToKnots(number, type), 'f', 2);
    }
    return ExtractNumber(line);
}

// ---------------- GROSS ----------------
QString TransformClass::NormalizeGross(const QString& line)
{
    QStringList nums;
    QRegularExpression r(R"(\d+)");
    QRegularExpressionMatchIterator it = r.globalMatch(line);

    while (it.hasNext()) {
        nums.append(it.next().captured(0));
    }

    if (nums.isEmpty()) return "";
    if (nums.size() >= 3) return nums[1];

    return nums[0];
}

// ---------------- DISPLACEMENT ----------------
double TransformClass::TransformationToTonnes(double number, const QString& type)
{
    if (type == "kg") return number / 1000.0;
    return number;
}

QString TransformClass::NormalizeDisplacement(const QString& line)
{
    QRegularExpression r(R"((\d+(\.\d+)?)\s*(kg|t|tonne|tonnes)?)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match;

    if ((match = r.match(line)).hasMatch()) {
        double number = match.captured(1).toDouble();
        QString type = match.captured(3).isEmpty() ? "t" : ToLower(match.captured(3));
        return QString::number(TransformationToTonnes(number, type), 'f', 2);
    }
    return ExtractNumber(line);
}

// ---------------- ENGINE ----------------

bool TransformClass::IsDesignerGarbage(const QString& designer)
{
    QString d = designer.toLower();
    if (d.contains("kw") || d.contains("%")) return true;

    QRegularExpression rx(R"(^\d+\s*(kw|hp)?$)");
    if (rx.match(d).hasMatch()) return true;

    return false;
}

QString TransformClass::DetectEngineDesigner(const QString& model)
{
    QString lower = ToLower(model);
    QString collapsedModel = lower;
    collapsedModel.remove(" ").remove("-");

    for (const auto& brand : ENGINE_BRAND_ALIASES) {
        for (const QString& alias : brand.aliases) {
            QString cleanAlias = alias.toLower();
            cleanAlias.remove(" ").remove("-");

            if (collapsedModel.contains(cleanAlias)) {
                return brand.canonical;
            }
        }
    }
    return "";
}

// Исправлено: комплексная очистка модели подстроками и регулярками
QString TransformClass::CleanEngineModel(QString model, const QString& designer)
{
    for (const auto& brand : ENGINE_BRAND_ALIASES) {
        if (brand.canonical == designer) {
            for (const QString& alias : brand.aliases) {
                // 1. Стираем по границам слов (где это возможно)
                model.remove(QRegularExpression(R"(\b)" + QRegularExpression::escape(alias) + R"(\b)", QRegularExpression::CaseInsensitiveOption));
                // 2. Стираем прямой подстрокой (для спецсимволов вроде '&')
                model.replace(alias, "", Qt::CaseInsensitive);

                // 3. Стираем слитно написанный бренд без пробелов
                QString clearAlias = alias;
                clearAlias.remove(" ");
                model.replace(clearAlias, "", Qt::CaseInsensitive);
            }
        }
    }

    // Чистим артефакты разделителей по краям и лишние пробелы
    model.remove(QRegularExpression(R"(^[-\s/]+)"));
    model.remove(QRegularExpression(R"([-\s/]+$)"));
    model.replace(QRegularExpression(R"(\s+)"), " ");

    return Trim(model);
}

// Исправлено: Бренд из модели теперь имеет наивысший приоритет над старым дизайнером
void TransformClass::FixEngineFields(QString& designer, QString& model)
{
    if (IsDesignerGarbage(designer)) {
        designer = "";
    }

    QString brandInModel = DetectEngineDesigner(model);
    QString brandInDesigner = DetectEngineDesigner(designer);

    // Если нашли бренд в модели — он принудительно становится дизайнером и вырезается из модели
    if (!brandInModel.isEmpty()) {
        designer = brandInModel;
        model = CleanEngineModel(model, brandInModel);
    }
    // Если в модели пусто, но бренд определился в дизайнере — стандартизируем его и чистим модель
    else if (!brandInDesigner.isEmpty()) {
        designer = brandInDesigner;
        model = CleanEngineModel(model, brandInDesigner);
    }
    else if (designer.isEmpty()) {
        designer = "";
    }

    // Дополнительная перестраховка удаления бренда
    if (!designer.isEmpty()) {
        model = CleanEngineModel(model, designer);
    }

    designer = Trim(designer);
    model = Trim(model);
}

// ---------------- MAIN TRANSFORM FUNCTION ----------------
bool TransformClass::TransformCSVFile(const QString& filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for reading:" << filename;
        return false;
    }

    QVector<QStringList> data;
    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty()) continue;
        data.append(ParseCSVLine(line));
    }
    file.close();

    if (data.isEmpty()) return false;

    QHash<QString, int> col;
    for (int i = 0; i < data[0].size(); ++i) {
        col[data[0][i].trimmed()] = i;
    }

    for (int r = 1; r < data.size(); ++r) {
        QStringList& row = data[r];
        if (row.isEmpty() || row.size() < col.size()) continue;

        // Если VesselName пустой — удаляем строку из вектора (требование ТЗ)
        if (col.contains("VesselName")) {
            QString vName = Trim(row[col["VesselName"]]).remove('"');
            if (vName.isEmpty()) {
                data.remove(r);
                r--;
                continue;
            }
            row[col["VesselName"]] = NormalizeTextField(row[col["VesselName"]]);
        }

        if (col.contains("Builder"))
            row[col["Builder"]] = NormalizeTextField(row[col["Builder"]]);

        if (col.contains("Designer")) {
            QString designer = row[col["Designer"]];
            if (designer.isEmpty() || designer == "-" || designer == "--" || designer == "---") {
                row[col["Designer"]] = "N/A";
            } else if (IsNumericOrMeasurement(designer)) {
                row[col["Designer"]] = "N/A";
            } else {
                QString cleaned = NormalizeTextField(designer);
                row[col["Designer"]] = (cleaned == "N/A" || IsNumericOrMeasurement(cleaned)) ? "N/A" : cleaned;
            }
        }

        if (col.contains("OwnerOperator"))
            row[col["OwnerOperator"]] = NormalizeTextField(row[col["OwnerOperator"]]);

        if (col.contains("Country"))
            row[col["Country"]] = NormalizeCountryField(row[col["Country"]]);

        if (col.contains("DeliveryDate"))
            row[col["DeliveryDate"]] = NormalizeDate(row[col["DeliveryDate"]]);

        if (col.contains("Length(m)"))
            row[col["Length(m)"]] = NormalizeLength(row[col["Length(m)"]]);

        if (col.contains("MaxSpeed(knots)"))
            row[col["MaxSpeed(knots)"]] = NormalizeSpeed(row[col["MaxSpeed(knots)"]]);

        if (col.contains("Gross"))
            row[col["Gross"]] = NormalizeGross(row[col["Gross"]]);

        if (col.contains("Displacement(kg)"))
            row[col["Displacement(kg)"]] = NormalizeDisplacement(row[col["Displacement(kg)"]]);

        if (col.contains("ImoNumber")) {
            QRegularExpression r(R"(\b\d{7}\b)");
            QRegularExpressionMatch match = r.match(row[col["ImoNumber"]]);
            row[col["ImoNumber"]] = match.hasMatch() ? match.captured(0) : "";
        }

        if (col.contains("NumberOfEngines")) {
            int num = ExtractFirstNumber(row[col["NumberOfEngines"]]);
            row[col["NumberOfEngines"]] = (num <= 0 || num > 20) ? "1" : QString::number(num);
        }

        if (col.contains("SisterShip")) {
            QString v = ExtractNumber(row[col["SisterShip"]]);
            row[col["SisterShip"]] = v.isEmpty() ? "0" : v;
        }

        if (col.contains("MainEngineDesigner") && col.contains("MainEngineModel")) {
            QString des = NormalizeTextField(row[col["MainEngineDesigner"]]);
            if (des == "N/A") des = "";

            QString mod = CleanGarbage(row[col["MainEngineModel"]]);
            if (mod.count(QRegularExpression(R"([^-\s])")) == 0 || mod == "N/A") mod = "";

            FixEngineFields(des, mod);

            row[col["MainEngineDesigner"]] = des;
            row[col["MainEngineModel"]] = mod;
        }
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&file);

    // Запись заголовков
    for (int i = 0; i < data[0].size(); ++i) {
        QString headerName = data[0][i];
        if (headerName == "Displacement(kg)") {
            headerName = "Displacement(t)";
        }
        out << headerName;
        if (i + 1 < data[0].size()) out << ",";
    }
    out << "\n";

    // Сохранение строк: абсолютно все элементы экранируются кавычками
    for (int r = 1; r < data.size(); ++r) {
        for (int c = 0; c < data[r].size(); ++c) {
            out << "\"" << ReplacementDotToComma(data[r][c]).remove('"') << "\"";
            if (c + 1 < data[r].size()) out << ",";
        }
        out << "\n";
    }

    file.close();
    return true;
}
