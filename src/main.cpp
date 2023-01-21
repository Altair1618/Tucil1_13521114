#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <time.h>

using namespace std;

// Variabel Global
int solCount = 0;
vector<double> cards;
vector<int> isDupe(4, -1);
vector<string> solutions, operators = {"+", "-", "*", "/"};

bool isCardValid(string s) {
    // Fungsi memeriksa apakah s merupakan sebuah nilai kartu yang valid (2-10, J, Q, K, A)
    if (s.length() > 1) {
        if (s == "10") {
            return true;
        }
    } else {
        if (s == "A" || s == "J" || s == "Q" || s == "K" || (s >= "2" && s <= "9")) return true;
    }

    return false;
}

bool isInputValid(string s) {
    // Fungsi memeriksa apakah s merupakan input yang valid yaitu berisi 4 kartu valid
    if (s.length() > 11) return false;
    
    int spaceCount = 0;
    string temp = "";
    for (auto x : s) {
        if (x == ' ') {
            spaceCount++;
            if (!isCardValid(temp)) return false;
            temp.clear();
        } else {
            temp.push_back(x);
        }
    }

    if (!isCardValid(temp)) return false;
    return (spaceCount == 3);
}

double getCardValue(string s) {
    // Mengembalikan nilai kartu pada game 24
    if (s.length() > 1) {
        if (s == "10") return 10.0;
    } else {
        if (s == "A") return 1.0;
        else if (s == "J") return 11.0;
        else if (s == "Q") return 12.0;
        else if (s == "K") return 13.0;
        else if (s >= "2" && s <= "9") return (double) (s[0] - '0');
    }

    return -1;
}

string getCardName(int val) {
    // Mengembalikan nama kartu berdasarkan nilai kartu pada game 24
    if (val == 1) return "A";
    else if (val == 11) return "J";
    else if (val == 12) return "Q";
    else if (val == 13) return "K";
    else return to_string(val);
}

double getOperationResult(string op, double leftval, double rightval) {
    // Mengembalikan hasil operasi aritmatika dengan masukan 2 nilai dan operator
    double ret = 0;
    if (op == "+") {
        ret = leftval + rightval;
    } else if (op == "-") {
        ret = leftval - rightval;
    } else if (op == "*") {
        ret = leftval * rightval;
    } else if (op == "/") {
        ret = leftval / rightval;
    }
    return ret;
}

void assignCard(string s) {
    // Memasukkan kartu hasil input pada vector cards
    cards.clear();

    string temp = "";
    for (auto x : s) {
        if (x == ' ') {
            cards.push_back(getCardValue(temp));
            temp.clear();
        } else {
            temp.push_back(x);
        }
    }

    cards.push_back(getCardValue(temp));
    return;
}

void inputUser() {
    // Prosedur meminta input 4 kartu kepada user
    string s;
    
    cin.ignore();
    while (true) {
        getline(cin, s);
        if (isInputValid(s)) {
            break;
        } else {
            cout << "Masukan tidak sesuai. Ulangi!" << endl;
        }
    }
    assignCard(s);

    return;
}

void generateRandomCards() {
    // Prosedur menghasilkan 4 kartu secara random
    srand(time(0));
    for (int i = 0; i < 4; i++) cards.push_back((double) (rand() % 13 + 1));

    cout << "Kartu yang dihasilkan yaitu: ";
    for (auto nums : cards) cout << getCardName((int) nums) << " ";
    cout << endl;
}

bool isSolution(double val) {
    // Memeriksa apakah val merupakan 24. Untuk menghindari kemungkinan bug compare pada double.
    return abs(val - 24) <= 1e-9;
}

void saveSolution(string a, string b, string c, string d, string e, vector<double> val) {
    // Meng-increment solCount dan memformat serta menyimpan solusi
    solCount++;
    solutions.push_back(a + to_string((int) val[0]) + b + to_string((int) val[1]) + c + to_string((int) val[2]) + d + to_string((int) val[3]) + e);
}

void solve(vector<double> nums, vector<string> op) {
    // Mengecek semua kemungkinan solusi 24
    double op1, op2, op3;

    // ((num1 op1 num2) op2 num3) op3 num4
    op1 = getOperationResult(op[0], nums[0], nums[1]);
    op2 = getOperationResult(op[1], op1, nums[2]);
    op3 = getOperationResult(op[2], op2, nums[3]);
    
    if (isSolution(op3)) {
        saveSolution("((", " " + op[0] + " ", ") " + op[1] + " ", ") " + op[2] + " ", "", nums);
    }

    // (num1 op1 (num2 op2 num3)) op3 num4
    op1 = getOperationResult(op[1], nums[1], nums[2]);
    op2 = getOperationResult(op[0], nums[0], op1);
    op3 = getOperationResult(op[2], op2, nums[3]);
    
    if (isSolution(op3)) {
        saveSolution("(", " " + op[0] + " (", " " + op[1] + " ", ")) " + op[2] + " ", "", nums);
    }

    // (num1 op1 num2) op2 (num3 op3 num4)
    op1 = getOperationResult(op[0], nums[0], nums[1]);
    op2 = getOperationResult(op[2], nums[2], nums[3]);
    op3 = getOperationResult(op[1], op1, op2);
    
    if (isSolution(op3)) {
        saveSolution("(", " " + op[0] + " ", ") " + op[1] + " (", " " + op[2] + " ", ")", nums);
    }

    // num1 op1 ((num2 op2 num3) op4 num4)
    op1 = getOperationResult(op[1], nums[1], nums[2]);
    op2 = getOperationResult(op[2], op1, nums[3]);
    op3 = getOperationResult(op[0], nums[0], op2);
    
    if (isSolution(op3)) {
        saveSolution("", " " + op[0] + " ((", " " + op[1] + " ", ") " + op[2] + " ", ")", nums);
    }

    // num1 op1 (num2 op2 (num3 op3 num4))
    op1 = getOperationResult(op[2], nums[2], nums[3]);
    op2 = getOperationResult(op[1], nums[1], op1);
    op3 = getOperationResult(op[0], nums[0], op2);
    
    if (isSolution(op3)) {
        saveSolution("", " " + op[0] + " (", " " + op[1] + " (", " " + op[2] + " ", "))", nums);
    }

    return;
}

void generatePerm(vector<double> perms, vector<bool> isTaken, int level) {
    // Menghasilkan permutasi dari cards dan memanggil solve untuk setiap kemungkinan operator
    if (level == 4) {
        for (auto op1 : operators) {
            for (auto op2 : operators) {
                for (auto op3 : operators) {
                    solve(perms, {op1, op2, op3});
                }
            }
        }
        return;
    }

    for (int i = 0; i < 4; i++) {
        if (!isTaken[i]) {
            if (isDupe[i] != -1 && !isTaken[isDupe[i]]) continue;
            perms.push_back(cards[i]);
            isTaken[i] = true;
            generatePerm(perms, isTaken, level + 1);
            perms.pop_back();
            isTaken[i] = false;
        }
    }
}

void printSolution() {
    // Prosedur output solusi
    cout << endl;
    if (solCount > 0) {
        cout << "Terdapat " << solCount << " solusi, yaitu:" << endl;
        for (auto x : solutions) cout << x << endl;
    } else {
        cout << "Tidak terdapat solusi untuk kombinasi kartu di atas" << endl;
    }
}

void writeSolutionToFile() {
    // Prosedur menyimpan solusi
    string filename;
    
    cout << "File akan disimpan dalam bentuk file .txt pada folder test" << endl << "Masukkan nama file: ";
    cin >> filename;

    ofstream SaveFile("../test/" + filename + ".txt");
    SaveFile << "Kartu: ";
    for (auto nums : cards) SaveFile << getCardName((int) nums) << " ";
    SaveFile << endl << endl;
    if (solCount > 0) {
        SaveFile << "Terdapat " << solCount << " solusi, yaitu:" << endl;
        for (auto x : solutions) SaveFile << x << endl;
    } else {
        SaveFile << "Tidak terdapat solusi untuk kombinasi kartu di atas" << endl;
    }

    SaveFile.close();
}

int main() {
    // Main Program

    // Meminta masukan dari pengguna berupa opsi input kartu manual atau random
    while (true) {
        cout << "Opsi Input:\n" << "1. Masukan dari Pengguna\n" << "2. Random\n" << "Pilihan Pengguna: ";
        int opt;
        cin >> opt;

        if (opt == 1) {
            inputUser();
        } else if (opt == 2) {
            generateRandomCards();
        } else {
            cout << "Masukan tidak sesuai. Ulangi!" << endl << endl;
            continue;
        }
        break;
    }
    
    // Memulai penghitungan waktu eksekusi
    auto start = chrono::steady_clock::now();

    // Memeriksa kartu yang merupakan duplikat
    for (int i = 1; i < 4; i++) {
        for (int j = 0; j < i; j++) {
            if (cards[i] == cards[j]) isDupe[i] = j;
        }
    }

    // Mencari solusi
    generatePerm({}, {false, false, false, false}, 0);

    // Mengakhiri penghitungan waktu eksekusi
    auto end = chrono::steady_clock::now();

    // Output solusi
    printSolution();
    cout << endl << "Waktu eksekusi (dalam ms) : " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

    // Opsi save solusi pada file
    while (true) {
        string pilihan;
        cout << endl << "Apakah hasil mau disimpan pada file (Y/N)? ";
        cin >> pilihan;

        if (pilihan == "Y") {
            writeSolutionToFile();
            break;
        } else if (pilihan == "N") {
            break;
        } else {
            cout << "Masukan tidak sesuai. Ulangi!" << endl << endl;
        }
    }

    cout << "Program Selesai" << endl;
}
