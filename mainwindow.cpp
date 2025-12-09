#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <stdexcept>
#include <QRandomGenerator>

#include <QString>
#include <QDebug>
#include <QTextEdit>
#include <QFile>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QFont globalFont;
    globalFont.setFamily("Arial");   // choose any font you like
    globalFont.setPointSize(10);     // your global font size

    QApplication::setFont(globalFont);


    ui->stackedWidget->setCurrentIndex(0);

    ui->labelWelcome->setText(
        "Welcome to MathWorld!\n"
        "\n"
        "This program is designed to help you practice fundamental\n"
        "Calculus Topics through interactive questions.\n"
        "\n"
        "Click “Start” to begin your learning journey.\n"
        );

    ui->labelMainMenu->setText(
        "Main Menu\n"
        "\n"
        "Please choose one of the following options:\n"
        "• Question to practice.\n"
        "• Formula to review calculus formula.\n"
        "• Back to return.\n"
        "\n"
        "Use the buttons below to continue."
        );

    ui->labelTopic->setText(
        "Select a Calculus topic you would like to practice:\n"
        "• Limits\n"
        "• Derivatives\n"
        "• Integrals\n"
        );

    ui->labelQuestion->setText(
        "\n"
        "Read the question displayed below and enter your answer in the text box.\n"
        "\n"
        "When you are ready, click “Submit Answer.”\n"
        );

    ui->labelFormulas->setText(
        "Select a  topic you would like to review the formulas:\n"
        "• Limits\n"
        "• Derivatives\n"
        "• Integrals\n"
        );


    integralBank = IntegralQuestion::loadFromFile("IntegralQuestion.txt");
    derivativeBank = DerivativeQuestion::loadFromFile("DerivativeQuestion.txt");
    limitBank = LimitQuestion::loadFromFile("LimitQuestion.txt");

    qDebug() << "Integral loaded:" << integralBank.size();
    qDebug() << "Derivative loaded:" << derivativeBank.size();
    qDebug() << "Limit loaded:" << limitBank.size();

}



MainWindow::~MainWindow()
{
    delete ui;
}


// ---------------------------------------------
// BASIC NAVIGATION
// ---------------------------------------------

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

void MainWindow::on_btnFormulas_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_BtnMainBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btnTopicBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_btnQuestionBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_btnFeedBackBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_btnExitScreen_clicked()
{
    close();

}

void MainWindow::on_btnExitBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_btnFormulaBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


// ----------------------------------------------------
// TOPIC SELECTION
// ----------------------------------------------------

void MainWindow::startTopic(const QString &topic)
{
    currentTopic = topic;
    if (topic == "Integral" && integralBank.empty()) {
        QMessageBox::critical(this, "Error", "Integral question bank is empty.");
        return;
    }
    if (topic == "Derivative" && derivativeBank.empty()) {
        QMessageBox::critical(this, "Error", "Derivative question bank is empty.");
        return;
    }
    if (topic == "Limit" && limitBank.empty()) {
        QMessageBox::critical(this, "Error", "Limit question bank is empty.");
        return;
    }


    if (topic == "Integral") {
        currentIndex = QRandomGenerator::global()->bounded(integralBank.size());
        ui->label_2->setText(QString::fromStdString(integralBank[currentIndex].getPrompt()));
    }
    else if (topic == "Derivative") {
        currentIndex = QRandomGenerator::global()->bounded(derivativeBank.size());
        ui->label_2->setText(QString::fromStdString(derivativeBank[currentIndex].getPrompt()));
    }
    else if (topic == "Limit") {
        currentIndex = QRandomGenerator::global()->bounded(limitBank.size());
        ui->label_2->setText(QString::fromStdString(limitBank[currentIndex].getPrompt()));
    }

    ui->lineEdit->clear();
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_btnIntegralQuestion_clicked()
{
    startTopic("Integral");
}

void MainWindow::on_btnDerivativeQuestion_clicked()
{
    startTopic("Derivative");
}

void MainWindow::on_btnLimitQuestion_clicked()
{
    startTopic("Limit");
}


// ----------------------------------------------------
// ANSWER SUBMISSION
// ----------------------------------------------------

void MainWindow::on_btnSubmit_clicked()
{
    try {
        QString userInput = ui->lineEdit->text().trimmed();
        // 1. Empty input → throw exception
        if (userInput.isEmpty()) {
            throw std::invalid_argument("Answer cannot be empty.");
        }


        bool correct = false;
        QString correctAns;
        QString explanationText;

        if (currentTopic == "Integral") {
            correct = integralBank[currentIndex].checkAnswer(userInput.toStdString());
            correctAns = QString::fromStdString(integralBank[currentIndex].getCorrectAnswer());
            explanationText = QString::fromStdString(integralBank[currentIndex].getExplanation());
        }
        else if (currentTopic == "Derivative") {
            correct = derivativeBank[currentIndex].checkAnswer(userInput.toStdString());
            correctAns = QString::fromStdString(derivativeBank[currentIndex].getCorrectAnswer());
            explanationText = QString::fromStdString(derivativeBank[currentIndex].getExplanation());
        }
        else if (currentTopic == "Limit") {
            correct = limitBank[currentIndex].checkAnswer(userInput.toStdString());
            correctAns = QString::fromStdString(limitBank[currentIndex].getCorrectAnswer());
            explanationText = QString::fromStdString(limitBank[currentIndex].getExplanation());
        }

        if (correct) {
            // If the user is correct: just say "Correct" and congratulate them
            ui->label_3->setText("Correct!");
            ui->label_4->setText("Congratulations!");
        }
        else {
            // If the user is wrong: show incorrect + correct answer + explanation
            ui->label_3->setText("Incorrect!");

            QString feedbackText = "Correct answer: " + correctAns;
            if (!explanationText.isEmpty()) {
                feedbackText += "\nExplanation: " + explanationText;
            }

            ui->label_4->setText(feedbackText);
        }

        ui->stackedWidget->setCurrentIndex(4);// go to feedback page
    }
    catch (const std::invalid_argument &e) {
        QMessageBox::warning(this, "Input Error", e.what());
    }
}



// ----------------------------------------------------
// NEXT QUESTION
// ----------------------------------------------------

void MainWindow::on_btnNextQuestion_clicked()
{
    currentIndex++;

    if (currentTopic == "Integral") {
        if (currentIndex >= integralBank.size()) currentIndex = 0;
        ui->label_2->setText(QString::fromStdString(integralBank[currentIndex].getPrompt()));
    }
    else if (currentTopic == "Derivative") {
        if (currentIndex >= derivativeBank.size()) currentIndex = 0;
        ui->label_2->setText(QString::fromStdString(derivativeBank[currentIndex].getPrompt()));
    }
    else if (currentTopic == "Limit") {
        if (currentIndex >= limitBank.size()) currentIndex = 0;
        ui->label_2->setText(QString::fromStdString(limitBank[currentIndex].getPrompt()));
    }

    ui->lineEdit->clear();
    ui->stackedWidget->setCurrentIndex(3);
}


// ----------------------------------------------------
// FORMULA SHEET LOADING
// ----------------------------------------------------

void MainWindow::on_btnIntegralFormulas_clicked()
{
    loadFormulaSheet("Integral");
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::on_btnDerivativeFormulas_clicked()
{
    loadFormulaSheet("Derivative");
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::on_btnLimitFormulas_clicked()
{
    loadFormulaSheet("Limit");
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::loadFormulaSheet(const QString &topic)
{
    QString filename;

    if (topic == "Integral") filename = "IntegralFormulaSheet.txt";
    if (topic == "Derivative") filename = "DerivativeFormulaSheet.txt";
    if (topic == "Limit") filename = "LimitFormulaSheet.txt";

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->FormulaBrowser->setText("Cannot load formula sheet.");
        return;
    }

    QString content = file.readAll();
    ui->FormulaBrowser->setText(content);
}


