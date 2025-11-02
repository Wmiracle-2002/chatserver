#ifndef MODIFYWIDGET_H
#define MODIFYWIDGET_H

#include <QWidget>
#include "clientcore.h"
#include "picturemanager.h"

namespace Ui {
class ModifyWidget;
}

class ModifyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ModifyWidget(ClientCore* clientCore, QWidget *parent = nullptr);
    ~ModifyWidget();
    void modifyname(QString new_name);
    void modifypassword(QString old_password, QString new_password);
    void onNameModified(const json &js);
    void onPwdModified(const json &js);
    void onPictureUploaded(bool success, const QString& message);
    void onPictureDownloaded(const QString& userid, const QPixmap& picture);
private slots:
    void on_modifynameButton_clicked();
    void on_modifypwdButton_clicked();
    void on_selectpictureButton_clicked();
    void on_modifypictureButton_clicked();

private:
    Ui::ModifyWidget *ui;
    ClientCore* m_clientCore;
    PictureManager* m_pm;
};

#endif // MODIFYWIDGET_H
