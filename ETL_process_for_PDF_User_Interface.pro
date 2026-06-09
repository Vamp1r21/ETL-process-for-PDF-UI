QT       += core gui widgets sql concurrent

include(QXlsx/QXlsx.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++19

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AdminFormDB.cpp \
    AdminFormUser.cpp \
    EditorForm.cpp \
    ExtractClass.cpp \
    LoadClass.cpp \
    TransformClass.cpp \
    adminform.cpp \
    main.cpp \
    avtorisation.cpp \
    userform.cpp

HEADERS += \
    AdminFormDB.h \
    AdminFormUser.h \
    EditorForm.h \
    ExtractClass.h \
    LoadClass.h \
    TransformClass.h \
    adminform.h \
    avtorisation.h \
    userform.h

FORMS += \
    AdminFormDB.ui \
    AdminFormUser.ui \
    EditorForm.ui \
    adminform.ui \
    avtorisation.ui \
    userform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    CSVFiles/SS 2002.csv \
    CSVFiles/SS 2008.csv \
    CSVFiles/SS 2009.csv \
    CSVFiles/SS 2010.csv \
    CSVFiles/SS 2012.csv \
    CSVFiles/SS 2013.csv \
    CSVFiles/SS 2014.csv \
    CSVFiles/SS 2015.csv \
    CSVFiles/SS 2016.csv \
    CSVFiles/SS 2018.csv \
    CSVFiles/SS 2019.csv \
    CSVFiles/SS 2020.csv \
    CSVFiles/SS 2021.csv \
    CSVFiles/SS 2022.csv \
    CSVFiles/SSS 1998.csv \
    CSVFiles/SSS 1999.csv \
    CSVFiles/SSS 2000.csv \
    CSVFiles/SSS 2002.csv \
    CSVFiles/SSS 2003.csv \
    CSVFiles/SSS 2004.csv \
    CSVFiles/SSS 2005.csv \
    CSVFiles/SSS 2006.csv \
    CSVFiles/SSS 2007.csv \
    CSVFiles/SSS 2008.csv \
    CSVFiles/SSS 2009.csv \
    CSVFiles/SSS 2010.csv \
    CSVFiles/SSS 2011.csv \
    CSVFiles/SSS 2012.csv \
    CSVFiles/SSS 2013.csv \
    CSVFiles/SSS 2014.csv \
    CSVFiles/SSS 2015.csv \
    CSVFiles/SSS 2016.csv \
    CSVFiles/SSS 2017.csv \
    CSVFiles/SSS 2018.csv \
    CSVFiles/SSS 2019.csv \
    CSVFiles/SSS 2020.csv \
    CSVFiles/SSS 2022.csv \
    CSVFiles/allShip.csv \
    PDFFiles/SS 2002.pdf \
    PDFFiles/SS 2003.PDF \
    PDFFiles/SS 2006.pdf \
    PDFFiles/SS 2007.pdf \
    PDFFiles/SS 2008.pdf \
    PDFFiles/SS 2009.pdf \
    PDFFiles/SS 2010.pdf \
    PDFFiles/SS 2011 (!).pdf \
    PDFFiles/SS 2012.pdf \
    PDFFiles/SS 2013.pdf \
    PDFFiles/SS 2014.pdf \
    PDFFiles/SS 2015.pdf \
    PDFFiles/SS 2016.pdf \
    PDFFiles/SS 2017 (!).pdf \
    PDFFiles/SS 2018.pdf \
    PDFFiles/SS 2019.pdf \
    PDFFiles/SS 2020.pdf \
    PDFFiles/SS 2021.pdf \
    PDFFiles/SS 2022.pdf \
    PDFFiles/SSS 1998.pdf \
    PDFFiles/SSS 1999.pdf \
    PDFFiles/SSS 2000.pdf \
    PDFFiles/SSS 2001.pdf \
    PDFFiles/SSS 2002.pdf \
    PDFFiles/SSS 2003.pdf \
    PDFFiles/SSS 2004.pdf \
    PDFFiles/SSS 2005.pdf \
    PDFFiles/SSS 2006.pdf \
    PDFFiles/SSS 2007.pdf \
    PDFFiles/SSS 2008.pdf \
    PDFFiles/SSS 2009.pdf \
    PDFFiles/SSS 2010.pdf \
    PDFFiles/SSS 2011.pdf \
    PDFFiles/SSS 2012.pdf \
    PDFFiles/SSS 2013.pdf \
    PDFFiles/SSS 2014.pdf \
    PDFFiles/SSS 2015.pdf \
    PDFFiles/SSS 2016.pdf \
    PDFFiles/SSS 2017.pdf \
    PDFFiles/SSS 2018.pdf \
    PDFFiles/SSS 2019.pdf \
    PDFFiles/SSS 2020.pdf \
    PDFFiles/SSS 2022.pdf \
    TXTFiles/SS 1998.txt \
    TXTFiles/SS 1999.txt \
    TXTFiles/SS 2000.txt \
    TXTFiles/SS 2001.txt \
    TXTFiles/SS 2002.txt \
    TXTFiles/SS 2003.txt \
    TXTFiles/SS 2004.txt \
    TXTFiles/SS 2006.txt \
    TXTFiles/SS 2007.txt \
    TXTFiles/SS 2008.txt \
    TXTFiles/SS 2009.txt \
    TXTFiles/SS 2010.txt \
    TXTFiles/SS 2012.txt \
    TXTFiles/SS 2013.txt \
    TXTFiles/SS 2014.txt \
    TXTFiles/SS 2015.txt \
    TXTFiles/SS 2016.txt \
    TXTFiles/SS 2018.txt \
    TXTFiles/SS 2019.txt \
    TXTFiles/SS 2020.txt \
    TXTFiles/SS 2021.txt \
    TXTFiles/SS 2022.txt \
    TXTFiles/SSS 1998.txt \
    TXTFiles/SSS 1999.txt \
    TXTFiles/SSS 2000.txt \
    TXTFiles/SSS 2001.txt \
    TXTFiles/SSS 2002.txt \
    TXTFiles/SSS 2003.txt \
    TXTFiles/SSS 2004.txt \
    TXTFiles/SSS 2005.txt \
    TXTFiles/SSS 2006.txt \
    TXTFiles/SSS 2007.txt \
    TXTFiles/SSS 2008.txt \
    TXTFiles/SSS 2009.txt \
    TXTFiles/SSS 2010.txt \
    TXTFiles/SSS 2011.txt \
    TXTFiles/SSS 2012.txt \
    TXTFiles/SSS 2013.txt \
    TXTFiles/SSS 2014.txt \
    TXTFiles/SSS 2015.txt \
    TXTFiles/SSS 2016.txt \
    TXTFiles/SSS 2017.txt \
    TXTFiles/SSS 2018.txt \
    TXTFiles/SSS 2019.txt \
    TXTFiles/SSS 2020.txt \
    TXTFiles/SSS 2022.txt
