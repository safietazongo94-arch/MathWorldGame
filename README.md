# Project Title
MathWorld: An Interactive Calculus Learning Game

# Description
MathWorld is an interactive C++/Qt GUI application designed to help students practice and understand fundamental Calculus concepts such as Limits, Derivatives, and Integrals through a structured question-and-answer interface.
The problem this project solves is that many calculus students struggle to recognize formulas, apply rules, and check their understanding. 
My application includes:
A Welcome Page
A Main Menu
Topic Selection (Limits, Derivatives, Integrals)
A Question Page with randomly generated problems
A Feedback Page showing explanation
A Formula Reference Page
An Exit Page
The program loads question banks from text files, selects questions randomly, and provides correctness checking, explanations.

# How I built it
Qt Widgets for the GUI
QStackedWidget to switch between pages
C++ classes to structure the question system
C++ classes + inheritance 
File I/O for reading questions and formulas
Exception handling in on_btnSubmit_clicked()

# Challenges I ran into
Problem 1: Page navigation issues using QStackedWidget
Solution: Explicitly set correct page indexes and fixed signal-slot connections
Problem 2: Loading and randomizing question banks
Solution: Improved file loading, added error checks, and used proper random generation
Challenge 3: Preventing crashes when question files were empty, missing, or incorrectly formatted
Solution: Added file-loading checks, debug messages, and validation to make sure the program only selects questions when data is available. (Exception handling)

# What I learned
Building a GUI with Qt
Handling file input/output
How to design multi-file C++ projects
