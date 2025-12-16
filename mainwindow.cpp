#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <algorithm>
#include <random>


#include <QRandomGenerator>
#include <QMessageBox>
#include <QString>
#include <QFile>
#include <QIODevice>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->stackedWidget->setStyleSheet(R"(
    QWidget {
        background-color: #d9f7fa;   /* your blue */
    }

    QPushButton {
        font-family: "Segoe UI";
        font-size: 20pt;
        min-height: 45px;
    }

    QRadioButton {
        font-family: "Segoe UI";
        font-size: 18pt;
    }

    QLabel {
        font-family: "Segoe UI";
        font-size: 20pt;
    }

    QTextBrowser {
        background-color: transparent;
        border: none;
    }
)");



    derivativeBank = DerivativeQuestion::loadFromFile("DerivativeQuestion.txt");
    integralBank   = IntegralQuestion::loadFromFile("IntegralQuestion.txt");
    limitBank      = LimitQuestion::loadFromFile("LimitQuestion.txt");

    ui->stackedWidget->setCurrentIndex(0);


    // ---------------- WELCOME PAGE ----------------
    ui->textBrowserWelcome->setHtml(
        "<div style='font-family: Segoe UI; font-size: 18pt;'>"

        "<h1 style='font-size: 30pt;'>Welcome to MathWorld!</h1>"

        "<p style='font-size: 20pt;'>"
        "MathWorld is an interactive application designed to help you practice "
        "Calculus concepts through multiple-choice questions and formula review."
        "</p>"

        "<h2 style='font-size: 26pt;'>How to use MathWorld:</h2>"

        "<ul style='font-size: 20pt;'>"
        "<li>Click Start to begin</li>"
        "<li>Choose Practice Questions or Formula Sheets</li>"
        "<li>Select a topic and answer the questions</li>"
        "</ul>"

        "<h2 style='font-size: 26pt;'>Goal:</h2>"
        "<p style='font-size: 20pt;'>Build confidence in Calculus through guided practice.</p>"

        "</div>"
        );




    // ---------------- MAIN MENU ----------------
    ui->labelMainMenu->setText(
        "Main Menu\n\n"
        "Choose one of the following options:\n"
        "• Practice Questions – Test your knowledge\n"
        "• Formula Sheets – Review calculus formulas\n"
        "• Exit – Close the application"
        );

    // ---------------- TOPIC SELECTION ----------------
    ui->labelTopic->setText(
        "Select a Calculus topic to practice:\n\n"
        "• Limits\n"
        "• Derivatives\n"
        "• Integrals\n\n"
        "A random multiple-choice question will be shown."
        );

    // ---------------- QUESTION PAGE ----------------
    ui->labelQuestionInstruction->setText(
        "Select the correct answer from the options below,\n"
        "then click “Submit Answer”."
        );

    // ---------------- FEEDBACK PAGE ----------------
    ui->labelFeedbackInstruction->setText(
        "Review the feedback below.\n"
        "Click “Next Question” to continue or “Back” to choose another topic."
        );

    // ---------------- FORMULA TOPIC PAGE ----------------
    ui->labelFormula->setText(
        "Select a topic to review its calculus formulas:\n\n"
        "• Limits\n"
        "• Derivatives\n"
        "• Integrals"
        );

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnStart_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_btnExit_clicked()
{
    close();
}


void MainWindow::on_btnQuestion_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_btnFormula_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}


void MainWindow::on_btnMenuBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::startTopic(const QString& topic)
{
    currentTopic = topic;

    if (topic == "Derivative")
        currentIndex = QRandomGenerator::global()->bounded(derivativeBank.size());
    else if (topic == "Integral")
        currentIndex = QRandomGenerator::global()->bounded(integralBank.size());
    else
        currentIndex = QRandomGenerator::global()->bounded(limitBank.size());

    showCurrentQuestion();
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_btnDerivativeQuestion_clicked()
{
    startTopic("Derivative");
}


void MainWindow::on_btnIntegralQuestion_clicked()
{
    startTopic("Integral");
}


void MainWindow::on_btnLimitQuestion_clicked()
{
    startTopic("Limit");
}


void MainWindow::on_btnTopicBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}



void MainWindow::showCurrentQuestion()
{
    Question* q = nullptr;

    if (currentTopic == "Derivative")
        q = &derivativeBank[currentIndex];
    else if (currentTopic == "Integral")
        q = &integralBank[currentIndex];
    else
        q = &limitBank[currentIndex];

    ui->label_2->setText(QString::fromStdString(q->getPrompt()));

    // ---- SHUFFLE ANSWERS ----
    std::vector<std::pair<std::string, int>> shuffled;

    for (int i = 0; i < 4; ++i) {
        shuffled.push_back({ q->getChoices()[i], i });
    }

    std::shuffle(shuffled.begin(), shuffled.end(),
                 std::mt19937(std::random_device{}()));

    // Track new correct index
    int newCorrectIndex = -1;
    for (int i = 0; i < 4; ++i) {
        if (shuffled[i].second == q->getCorrectIndex())
            newCorrectIndex = i;
    }

    // Save updated correct index
    q->setCorrectIndex(newCorrectIndex);

    // Update UI text
    ui->radioA->setText(QString::fromStdString(shuffled[0].first));
    ui->radioB->setText(QString::fromStdString(shuffled[1].first));
    ui->radioC->setText(QString::fromStdString(shuffled[2].first));
    ui->radioD->setText(QString::fromStdString(shuffled[3].first));

    ui->radioA->setChecked(false);
    ui->radioB->setChecked(false);
    ui->radioC->setChecked(false);
    ui->radioD->setChecked(false);

}


int MainWindow::selectedIndex() const
{
    if (ui->radioA->isChecked()) return 0;
    if (ui->radioB->isChecked()) return 1;
    if (ui->radioC->isChecked()) return 2;
    if (ui->radioD->isChecked()) return 3;
    return -1;
}

void MainWindow::on_btnSubmit_clicked()
{
    try {
        int choice = selectedIndex();

        if (choice == -1) {
            throw std::runtime_error("No answer selected");
        }

        const Question* q = nullptr;
        if (currentTopic == "Derivative") q = &derivativeBank[currentIndex];
        else if (currentTopic == "Integral") q = &integralBank[currentIndex];
        else q = &limitBank[currentIndex];

        if (q->checkAnswer(choice)) {
            ui->label_3->setText("Correct!");
            ui->label_4->setText("Great job!");
        } else {
            QString correct =
                QString::fromStdString(q->getChoices()[q->getCorrectIndex()]);
            ui->label_3->setText("Incorrect!");
            ui->label_4->setText("Correct answer: " + correct +
                                 "\nExplanation: " +
                                 QString::fromStdString(q->getExplanation()));
        }

        ui->stackedWidget->setCurrentIndex(4);
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}


void MainWindow::on_btnQuestionBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_radioA_clicked()
{

}


void MainWindow::on_radioB_clicked()
{

}


void MainWindow::on_radioD_clicked()
{

}


void MainWindow::on_radioC_clicked()
{

}


void MainWindow::on_btnNext_clicked()
{
    if (currentTopic == "Derivative") {
        currentIndex++;
        if (currentIndex >= derivativeBank.size())
            currentIndex = 0;
    }
    else if (currentTopic == "Integral") {
        currentIndex++;
        if (currentIndex >= integralBank.size())
            currentIndex = 0;
    }
    else if (currentTopic == "Limit") {
        currentIndex++;
        if (currentIndex >= limitBank.size())
            currentIndex = 0;
    }

    showCurrentQuestion();
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_btnFeedbackBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::loadFormulaSheet(const QString& topic)
{
    try {
        QString filename;

        if (topic == "Derivative")
            filename = "DerivativeFormulaSheet.txt";
        else if (topic == "Integral")
            filename = "IntegralFormulaSheet.txt";
        else if (topic == "Limit")
            filename = "LimitFormulaSheet.txt";
        else
            throw std::runtime_error("Invalid topic");

        QFile file(filename);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            throw std::runtime_error("Cannot open formula file");
        }

        ui->BrowserFormula->setHtml(
            "<pre style='font-size:22pt; font-family: Consolas;'>"
            + file.readAll() +
            "</pre>"
            );



    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "File Error", e.what());
    }
}


void MainWindow::on_btnDerivativeFormula_clicked()
{
    loadFormulaSheet("Derivative");
    ui->stackedWidget->setCurrentIndex(6);
}


void MainWindow::on_btnIntegralFormula_clicked()
{
    loadFormulaSheet("Integral");
    ui->stackedWidget->setCurrentIndex(6);
}


void MainWindow::on_btnLimitFormula_clicked()
{
    loadFormulaSheet("Limit");
    ui->stackedWidget->setCurrentIndex(6);
}


void MainWindow::on_btnFormulaBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_btnExitScreen_clicked()
{
    close();
}


void MainWindow::on_btnOptionBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

