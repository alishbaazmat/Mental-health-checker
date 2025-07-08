#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
using namespace std;

class User {
protected:
    string name;
    int age;
    string gender;
    string email;
    string password;

public:
    void registerUser() {
        cout << "\n---- User Registration ----\n";
        cin.ignore(); 
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Age: ";
        cin >> age;
        cin.ignore();
        cout << "Enter Gender (Male/Female): ";
        getline(cin, gender);
        cout << "Enter Email: ";
        getline(cin, email);
        cout << "Create Password: ";
        getline(cin, password);

        ofstream file("users.txt", ios::app);
        file << name << "|" << age << "|" << gender << "|" << email << "|" << password << "\n";
        file.close();

        cout << "\n? Registration Successful!\n";
    }

    bool loginUser() {
        string enteredEmail, enteredPassword;
        cout << "\n---- Login ----\n";
        cin.ignore();
        cout << "Enter Email: ";
        getline(cin, enteredEmail);
        cout << "Enter Password: ";
        getline(cin, enteredPassword);

        ifstream file("users.txt");
        string line;
        while (getline(file, line)) {
            size_t pos = 0;
            vector<string> tokens;
            while ((pos = line.find('|')) != string::npos) {
                tokens.push_back(line.substr(0, pos));
                line.erase(0, pos + 1);
            }
            tokens.push_back(line); 

            if (tokens.size() == 5) {
                if (tokens[3] == enteredEmail && tokens[4] == enteredPassword) {
                    name = tokens[0];
                    age = stoi(tokens[1]);
                    gender = tokens[2];
                    email = tokens[3];
                    password = tokens[4];
                    cout << "\n? Login Successful! Welcome, " << name << "!\n";
                    return true;
                }
            }
        }

        cout << "\n? Login Failed! Incorrect Email or Password.\n";
        return false;
    }

    string getUserEmail() {
        return email;
    }

    string getUserName() {
        return name;
    }
};

class MentalHealthQuiz : public User {
private:
    int score;

public:
    MentalHealthQuiz() {
        score = 0;
    }

    void startQuiz() {
        cin.ignore();
        score = 0;
        cout << "\n---- Mental Health Quiz ----\n";
        cout << "Please answer Yes or No to the following questions:\n\n";

        vector<string> questions = {
            "1. Do you often feel anxious or worried?",
            "2. Do you feel sad or hopeless frequently?",
            "3. Do you have trouble sleeping or relaxing?",
            "4. Do you find it hard to concentrate on things?",
            "5. Do you feel lonely or isolated?",
            "6. Do you lose interest in activities you once enjoyed?",
            "7. Do you feel tired or have low energy most days?",
            "8. Do you overthink or feel mentally exhausted?",
            "9. Do you feel guilty or worthless without a reason?",
            "10. Do you get angry or irritated easily?"
        };

        string answer;
        for (const string& q : questions) {
            cout << q << " ";
            getline(cin, answer);
            if (answer == "Yes" || answer == "yes")
                score++;
        }

        showResult();
        saveResult();
    }

    void showResult() {
        cout << "\n---- Quiz Result ----\n";
        cout << "Score: " << score << "/10\n";

        if (score <= 2) {
            cout << "?? Your mental health seems good.\n";
        } else if (score <= 6) {
            cout << "?? You might be experiencing some stress. Take care!\n";
        } else {
            cout << "?? High risk detected. Please consult a professional.\n";
        }

        showRandomTip();
    }

    void showRandomTip() {
        vector<string> tips = {
            "?? Tip: Talk to someone you trust.",
            "?? Tip: Stay physically active.",
            "?? Tip: Take deep breaths and rest your mind.",
            "?? Tip: Stay connected with friends and family.",
            "?? Tip: Take short breaks and reduce screen time.",
            "?? Tip: Listen to music or try meditation.",
            "?? Tip: Maintain a regular sleep schedule."
        };

        srand(time(0));
        int tipIndex = rand() % tips.size();
        cout << tips[tipIndex] << endl;
    }

    void saveResult() {
        ofstream resultFile("results.txt", ios::app);
        resultFile << getUserEmail() << " " << score << "/10\n";
        resultFile.close();
    }

    void viewPreviousResults() {
        ifstream file("results.txt");
        string emailInFile, scoreInFile;
        bool found = false;

        cout << "\n?? Your Previous Quiz Results:\n";
        while (file >> emailInFile >> scoreInFile) {
            if (emailInFile == getUserEmail()) {
                cout << "? Score: " << scoreInFile << endl;
                found = true;
            }
        }

        if (!found) {
            cout << "No previous results found.\n";
        }
    }

    void showResources() {
        cout << "\n?? Mental Health Resources:\n";
        cout << "1. ?? Website: https://www.mentalhealth.gov\n";
        cout << "2. ?? Helpline: Talk to a mental health professional at 1166 (Pakistan Helpline)\n";
        cout << "3. ?? App: Try using apps like Calm, Headspace, or Moodpath\n";
        cout << "4. ?? Book: 'Feeling Good' by David D. Burns (Self-help CBT)\n";
        cout << "5. ?? Local Clinics: Visit nearby clinics or hospitals with mental health departments.\n";
        cout << "6. ?? Tip: Practice yoga, mindfulness, and journaling to reduce stress.\n";
    }

    string getTodayDate() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char dateStr[11];
        sprintf(dateStr, "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
        return string(dateStr);
    }

    bool hasSeenTipToday() {
        ifstream file("tip_dates.txt");
        string line;
        string today = getTodayDate();
        string userEmail = getUserEmail();

        while (getline(file, line)) {
            size_t pos = line.find('|');
            if (pos != string::npos) {
                string emailInFile = line.substr(0, pos);
                string dateInFile = line.substr(pos + 1);
                if (emailInFile == userEmail && dateInFile == today) {
                    return true;
                }
            }
        }
        return false;
    }

    void updateTipDate() {
        string today = getTodayDate();
        string userEmail = getUserEmail();

        ifstream file("tip_dates.txt");
        vector<string> lines;
        bool updated = false;
        string line;

        while (getline(file, line)) {
            size_t pos = line.find('|');
            if (pos != string::npos) {
                string emailInFile = line.substr(0, pos);
                if (emailInFile == userEmail) {
                    line = userEmail + "|" + today;
                    updated = true;
                }
            }
            lines.push_back(line);
        }
        file.close();

        if (!updated) {
            lines.push_back(userEmail + "|" + today);
        }

        ofstream outFile("tip_dates.txt");
        for (const auto& l : lines) {
            outFile << l << "\n";
        }
        outFile.close();
    }

    void showDailyTip() {
        if (!hasSeenTipToday()) {
            cout << "\n?? Daily Mental Health Tip:\n";
            showRandomTip();
            updateTipDate();
        }
    }

    void breathingExercise() {
        cout << "\n?? Guided Breathing Exercise\n";
        for (int i = 0; i < 3; i++) {
            cout << "Breathe in... (4 seconds)\n";
            this_thread::sleep_for(chrono::seconds(4));
            cout << "Hold... (7 seconds)\n";
            this_thread::sleep_for(chrono::seconds(7));
            cout << "Breathe out... (8 seconds)\n";
            this_thread::sleep_for(chrono::seconds(8));
            cout << "----------------------------------\n";
        }
        cout << "Well done! This exercise helps reduce stress.\n";
    }

    void relaxationTips() {
        cout << "\n?? Stress Relief Tips:\n";
        cout << "1. Take a short walk outside.\n";
        cout << "2. Listen to calming music.\n";
        cout << "3. Practice mindfulness or meditation.\n";
        cout << "4. Write down your thoughts in a journal.\n";
        cout << "5. Try progressive muscle relaxation.\n";
        cout << "6. Maintain a healthy sleep routine.\n";
    }

    void stressManagementMenu() {
        int choice;
        do {
            cout << "\n--- Stress Management Tools ---\n";
            cout << "1. Guided Breathing Exercise\n";
            cout << "2. Relaxation Tips\n";
            cout << "3. Back to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> choice;
            switch(choice) {
                case 1: breathingExercise(); break;
                case 2: relaxationTips(); break;
                case 3: break;
                default: cout << "Invalid choice, try again.\n";
            }
        } while (choice != 3);
    }

    void addEmergencyContact() {
        cin.ignore();
        string contactName, contactNumber;
        cout << "\n?? Add Emergency Contact:\n";
        cout << "Enter Contact Name: ";
        getline(cin, contactName);
        cout << "Enter Contact Number: ";
        getline(cin, contactNumber);

        ofstream file("emergency_contacts.txt", ios::app);
        file << getUserEmail() << "|" << contactName << "|" << contactNumber << "\n";
        file.close();
        cout << "? Contact Saved Successfully!\n";
    }

    void viewEmergencyContacts() {
        ifstream file("emergency_contacts.txt");
        string line;
        bool found = false;
        cout << "\n?? Emergency Contacts:\n";
        while (getline(file, line)) {
            size_t pos1 = line.find('|');
            size_t pos2 = line.rfind('|');
            if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
                string emailInFile = line.substr(0, pos1);
                string contactName = line.substr(pos1 + 1, pos2 - pos1 - 1);
                string contactNumber = line.substr(pos2 + 1);
                if (emailInFile == getUserEmail()) {
                    cout << "? " << contactName << ": " << contactNumber << "\n";
                    found = true;
                }
            }
        }
        if (!found) {
            cout << "No emergency contacts found.\n";
        }
    }
};

int main() {
    MentalHealthQuiz app;
    int choice;

    cout << "==== Welcome to Mental Health Support App ====\n";

    while (true) {
        cout << "\n1. Register\n2. Login\n3. Exit\nEnter choice: ";
        cin >> choice;

        if (choice == 1) {
            app.registerUser();
        } else if (choice == 2) {
            if (app.loginUser()) {
                int userChoice;
                do {
                    cout << "\n--- Main Menu ---\n";
                    cout << "1. Take Mental Health Quiz\n";
                    cout << "2. View Previous Results\n";
                    cout << "3. Show Daily Tip\n";
                    cout << "4. Mental Health Resources\n";
                    cout << "5. Stress Management Tools\n";
                    cout << "6. Add Emergency Contact\n";
                    cout << "7. View Emergency Contacts\n";
                    cout << "8. Logout\n";
                    cout << "Enter your choice: ";
                    cin >> userChoice;

                    switch (userChoice) {
                        case 1: app.startQuiz(); break;
                        case 2: app.viewPreviousResults(); break;
                        case 3: app.showDailyTip(); break;
                        case 4: app.showResources(); break;
                        case 5: app.stressManagementMenu(); break;
                        case 6: app.addEmergencyContact(); break;
                        case 7: app.viewEmergencyContacts(); break;
                        case 8: cout << "Logging out...\n"; break;
                  default: cout << "Invalid option.\n";
                    }

                } while (userChoice != 8);
            }
        } else if (choice == 3) {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Invalid input. Try again.\n";
        }
    }

    return 0;
}

