#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "derivativequestion.h"
#include "integralquestion.h"
#include "limitquestion.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_btnStart_clicked();

    void on_btnExit_clicked();

    void on_btnQuestion_clicked();

    void on_btnFormula_clicked();

    void on_btnMenuBack_clicked();

    void on_btnDerivativeQuestion_clicked();

    void on_btnIntegralQuestion_clicked();

    void on_btnLimitQuestion_clicked();

    void on_btnTopicBack_clicked();

    void on_btnSubmit_clicked();

    void on_btnQuestionBack_clicked();

    void on_radioA_clicked();

    void on_radioB_clicked();

    void on_radioD_clicked();

    void on_radioC_clicked();

    void on_btnNext_clicked();

    void on_btnFeedbackBack_clicked();

    void on_btnDerivativeFormula_clicked();

    void on_btnIntegralFormula_clicked();

    void on_btnLimitFormula_clicked();

    void on_btnFormulaBack_clicked();

    void on_btnExitScreen_clicked();

    void on_btnOptionBack_clicked();

private:
    Ui::MainWindow *ui;

    std::vector<DerivativeQuestion> derivativeBank;
    std::vector<IntegralQuestion>   integralBank;
    std::vector<LimitQuestion>      limitBank;

    QString currentTopic;
    int currentIndex = 0;
    void startTopic(const QString& topic);
    void showCurrentQuestion();
    int selectedIndex() const;
    void loadFormulaSheet(const QString& topic);

};
#endif // MAINWINDOW_H
