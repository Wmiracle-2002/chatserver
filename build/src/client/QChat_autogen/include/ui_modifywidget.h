/********************************************************************************
** Form generated from reading UI file 'modifywidget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODIFYWIDGET_H
#define UI_MODIFYWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModifyWidget
{
public:
    QLineEdit *modifynameEdit;
    QLineEdit *oldpwdEdit;
    QLineEdit *newpwdEdit;
    QLabel *pictureLabel;
    QLabel *nameLabel;
    QLineEdit *againpwdEdit;
    QLabel *oldpwdLabel;
    QLabel *newpwdLabel;
    QLabel *againpwdLabel;
    QPushButton *modifynameButton;
    QPushButton *modifypwdButton;
    QPushButton *selectpictureButton;
    QPushButton *modifypictureButton;
    QLabel *modifyinfoLabel;

    void setupUi(QWidget *ModifyWidget)
    {
        if (ModifyWidget->objectName().isEmpty())
            ModifyWidget->setObjectName("ModifyWidget");
        ModifyWidget->resize(400, 300);
        modifynameEdit = new QLineEdit(ModifyWidget);
        modifynameEdit->setObjectName("modifynameEdit");
        modifynameEdit->setGeometry(QRect(220, 80, 113, 25));
        oldpwdEdit = new QLineEdit(ModifyWidget);
        oldpwdEdit->setObjectName("oldpwdEdit");
        oldpwdEdit->setGeometry(QRect(20, 160, 113, 25));
        newpwdEdit = new QLineEdit(ModifyWidget);
        newpwdEdit->setObjectName("newpwdEdit");
        newpwdEdit->setGeometry(QRect(140, 160, 113, 25));
        pictureLabel = new QLabel(ModifyWidget);
        pictureLabel->setObjectName("pictureLabel");
        pictureLabel->setGeometry(QRect(110, 230, 64, 64));
        nameLabel = new QLabel(ModifyWidget);
        nameLabel->setObjectName("nameLabel");
        nameLabel->setGeometry(QRect(30, 80, 191, 25));
        nameLabel->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);
        againpwdEdit = new QLineEdit(ModifyWidget);
        againpwdEdit->setObjectName("againpwdEdit");
        againpwdEdit->setGeometry(QRect(260, 160, 113, 25));
        oldpwdLabel = new QLabel(ModifyWidget);
        oldpwdLabel->setObjectName("oldpwdLabel");
        oldpwdLabel->setGeometry(QRect(50, 140, 67, 17));
        newpwdLabel = new QLabel(ModifyWidget);
        newpwdLabel->setObjectName("newpwdLabel");
        newpwdLabel->setGeometry(QRect(170, 140, 67, 17));
        againpwdLabel = new QLabel(ModifyWidget);
        againpwdLabel->setObjectName("againpwdLabel");
        againpwdLabel->setGeometry(QRect(260, 140, 121, 17));
        modifynameButton = new QPushButton(ModifyWidget);
        modifynameButton->setObjectName("modifynameButton");
        modifynameButton->setGeometry(QRect(150, 110, 95, 25));
        modifypwdButton = new QPushButton(ModifyWidget);
        modifypwdButton->setObjectName("modifypwdButton");
        modifypwdButton->setGeometry(QRect(150, 190, 95, 25));
        selectpictureButton = new QPushButton(ModifyWidget);
        selectpictureButton->setObjectName("selectpictureButton");
        selectpictureButton->setGeometry(QRect(210, 230, 95, 25));
        modifypictureButton = new QPushButton(ModifyWidget);
        modifypictureButton->setObjectName("modifypictureButton");
        modifypictureButton->setGeometry(QRect(210, 260, 95, 25));
        modifyinfoLabel = new QLabel(ModifyWidget);
        modifyinfoLabel->setObjectName("modifyinfoLabel");
        modifyinfoLabel->setGeometry(QRect(140, 20, 111, 41));
        QFont font;
        font.setPointSize(20);
        modifyinfoLabel->setFont(font);

        retranslateUi(ModifyWidget);

        QMetaObject::connectSlotsByName(ModifyWidget);
    } // setupUi

    void retranslateUi(QWidget *ModifyWidget)
    {
        ModifyWidget->setWindowTitle(QCoreApplication::translate("ModifyWidget", "Form", nullptr));
        pictureLabel->setText(QString());
        nameLabel->setText(QString());
        oldpwdLabel->setText(QCoreApplication::translate("ModifyWidget", "\345\216\237\345\257\206\347\240\201\357\274\232", nullptr));
        newpwdLabel->setText(QCoreApplication::translate("ModifyWidget", "\346\226\260\345\257\206\347\240\201\357\274\232", nullptr));
        againpwdLabel->setText(QCoreApplication::translate("ModifyWidget", "\345\206\215\346\254\241\350\276\223\345\205\245\346\226\260\345\257\206\347\240\201\357\274\232", nullptr));
        modifynameButton->setText(QCoreApplication::translate("ModifyWidget", "\344\277\256\346\224\271\346\230\265\347\247\260", nullptr));
        modifypwdButton->setText(QCoreApplication::translate("ModifyWidget", "\344\277\256\346\224\271\345\257\206\347\240\201", nullptr));
        selectpictureButton->setText(QCoreApplication::translate("ModifyWidget", "\351\200\211\346\213\251\345\233\276\347\211\207", nullptr));
        modifypictureButton->setText(QCoreApplication::translate("ModifyWidget", "\344\277\256\346\224\271\345\244\264\345\203\217", nullptr));
        modifyinfoLabel->setText(QCoreApplication::translate("ModifyWidget", "\344\277\256\346\224\271\344\277\241\346\201\257", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModifyWidget: public Ui_ModifyWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODIFYWIDGET_H
