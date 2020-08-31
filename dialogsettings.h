#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include "mainwindow.h"
#include <QDialog>
#include <QTableWidgetItem>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>

namespace Ui {
class DialogSettings;
}

struct Changes {
    bool deleted;
    bool checked;
    QString name;
};

class DeletePushButton : public QPushButton
{
    Q_OBJECT
    uint64_t _profileRange;
public:
    DeletePushButton(uint64_t profileRange) : _profileRange(profileRange) {
        setText("");
        setIcon(QIcon(":/icons/res/delete.png"));
        setIconSize(QSize(24, 24));
        connect(this, &DeletePushButton::clicked, this, [&]{ emit deleteEntry(_profileRange); });
    }
signals:
    void deleteEntry(uint64_t range);
};

class ProfileCheckBox : public QCheckBox
{
    Q_OBJECT
    uint64_t _profileRange;
public:
    ProfileCheckBox(uint64_t profileRange) : _profileRange(profileRange) {
        connect(this, &ProfileCheckBox::stateChanged, this, [&](int state){ emit checkEntry(_profileRange, state == Qt::Checked); });
    }
signals:
    void checkEntry(uint64_t range, bool checked);
};

class ProfileTextItem : public QLineEdit
{
    Q_OBJECT
    uint64_t _profileRange;
public:
    ProfileTextItem(uint64_t profileRange, QString label) : QLineEdit(label), _profileRange(profileRange) {
        connect(this, &ProfileTextItem::textEdited, this, [&]{ emit textChanged(_profileRange, text()); });
    }
signals:
    void textChanged(uint64_t range, QString text);
};

class DialogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettings(MainWindow *main);
    ~DialogSettings();

protected:
    void resizeEvent(QResizeEvent *);
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *event);

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

    void on_checkBoxDarkTheme_stateChanged(int arg1);

    void on_horizontalSliderSnap_sliderMoved(int position);

    void on_spinBoxSnap_valueChanged(int arg1);

private:
    Ui::DialogSettings *ui;
    MainWindow *main;
    bool accepted = false;

    QMap<uint64_t, Changes> profileChanges;

    /// The index of the column containing checkboxes
    const int COL_CHECKBOX = 0;
    /// The index of the column containing the ip ranges
    const int COL_IP_RANGE = 1;
    /// The index of the column containing the names
    const int COL_NAME = 2;
    bool originalDarkTheme;

    void refreshTable(bool firstTime = false);
    void updateGridWidth();
};

#endif // DIALOGSETTINGS_H
