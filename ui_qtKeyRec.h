/********************************************************************************
** Form generated from reading UI file 'qtKeyRec.ui'
**
** Created: Thu Oct 6 13:24:35 2011
**      by: Qt User Interface Compiler version 4.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTKEYREC_H
#define UI_QTKEYREC_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtKeyRec
{
public:
    QGridLayout *gridLayout;
    QPushButton *startButton;
    QPushButton *quitButton;
    QComboBox *comboBox;

    void setupUi(QWidget *QtKeyRec)
    {
        if (QtKeyRec->objectName().isEmpty())
            QtKeyRec->setObjectName(QString::fromUtf8("QtKeyRec"));
        QtKeyRec->resize(480, 600);
        gridLayout = new QGridLayout(QtKeyRec);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        startButton = new QPushButton(QtKeyRec);
        startButton->setObjectName(QString::fromUtf8("startButton"));

        gridLayout->addWidget(startButton, 0, 0, 1, 2);

        quitButton = new QPushButton(QtKeyRec);
        quitButton->setObjectName(QString::fromUtf8("quitButton"));

        gridLayout->addWidget(quitButton, 2, 0, 1, 2);

        comboBox = new QComboBox(QtKeyRec);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setMinimumContentsLength(5);

        gridLayout->addWidget(comboBox, 1, 0, 1, 2);


        retranslateUi(QtKeyRec);

        QMetaObject::connectSlotsByName(QtKeyRec);
    } // setupUi

    void retranslateUi(QWidget *QtKeyRec)
    {
        QtKeyRec->setWindowTitle(QApplication::translate("QtKeyRec", "Form", 0, QApplication::UnicodeUTF8));
        startButton->setText(QApplication::translate("QtKeyRec", "Start", 0, QApplication::UnicodeUTF8));
        quitButton->setText(QApplication::translate("QtKeyRec", "Quit", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("QtKeyRec", "KeyRec", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtKeyRec", "culatone", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class QtKeyRec: public Ui_QtKeyRec {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTKEYREC_H
