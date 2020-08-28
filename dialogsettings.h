#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>

namespace Ui {
class DialogSettings;
}

class DialogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettings(QWidget *parent = nullptr);
    ~DialogSettings();

protected:
    void resizeEvent(QResizeEvent *);
    void showEvent(QShowEvent *);

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::DialogSettings *ui;

    void updateGridWidth();
};

#endif // DIALOGSETTINGS_H
