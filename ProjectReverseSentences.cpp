// Author: Aayush Keshari
// M Number: M15039880
// Email: kesharah@mail.uc.edu
// ------------------------------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <stack>
#include <algorithm>

class StackError : public std::exception {
public:
    const char* what() const noexcept override {
        return "Stack Error: Stack is empty or full."; // Output when an error in the stack is encountered
    }
};

class QueueError : public std::exception {
public:
    const char* what() const noexcept override {
        return "Queue Error: Queue is empty or full."; // Output when an error in the queue is encountered
    }
};

template <typename T>
// Class for stack
class Stack {
private:
    T** data;
    int size;
    int top;

public:
    Stack(int stackSize) {
        size = stackSize;
        data = new T * [size];
        top = -1;
    }

    ~Stack() {
        makeEmpty();
        delete[] data;
    }

    void push(T* item) {
        if (isFull()) {
            throw StackError();
        }
        data[++top] = item;
    }

    void pop() {
        if (isEmpty()) {
            throw StackError();
        }
        delete data[top--];
    }

    T* topElement() {
        if (isEmpty()) {
            throw StackError();
        }
        return data[top];
    }

    int length() const {
        return top + 1;
    }

    void makeEmpty() {
        while (!isEmpty()) {
            pop();
        }
    }

    bool isEmpty() const {
        return top == -1;
    }

    bool isFull() const {
        return top == size - 1;
    }
};

template <typename T>
// Class for queue
class Queue {
private:
    T** data;
    int size;
    int front;
    int rear;

public:
    Queue(int queueSize) {
        size = queueSize;
        data = new T * [size];
        front = rear = -1;
    }

    ~Queue() {
        makeEmpty();
        delete[] data;
    }

    // Enqueue
    void enqueue(T* item) {
        if (isFull()) {
            throw QueueError();
        }
        if (isEmpty()) {
            front = rear = 0;
        }
        else {
            rear = (rear + 1) % size;
        }
        data[rear] = item;
    }

    // Dequeue 
    void dequeue() {
        if (isEmpty()) {
            throw QueueError();
        }
        delete data[front];
        if (front == rear) {
            front = rear = -1;
        }
        else {
            front = (front + 1) % size;
        }
    }

    int length() const {
        if (isEmpty()) {
            return 0;
        }
        if (front <= rear) {
            return rear - front + 1;
        }
        else {
            return size - front + rear + 1;
        }
    }

    void makeEmpty() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    bool isEmpty() const {
        return front == -1;
    }

    bool isFull() const {
        return (rear + 1) % size == front;
    }

    T* frontElement() {
        if (isEmpty()) {
            throw QueueError();
        }
        return data[front];
    }
};

void reverseString(std::string& str) {
    int left = 0;
    int right = str.length() - 1;
    while (left < right) {
        std::swap(str[left], str[right]);
        left++;
        right--;
    }
}

// Function to reverse the words in a line
std::string reverseLine(const std::string& line) {
    std::istringstream iss(line);
    std::ostringstream oss;
    std::string word;
    std::string sentencePunctuation = ""; // Stores the punctuation at the end of the sentence
    bool firstWord = true;

    while (iss >> word) {
        // Extracts and stores the punctuation from the beginning and end of the word
        std::string wordPunctuation = "";
        while (!word.empty() && !std::isalnum(word[0])) {
            wordPunctuation += word[0];
            word.erase(0, 1);
        }
        while (!word.empty() && !std::isalnum(word[word.length() - 1])) {
            wordPunctuation = word[word.length() - 1] + wordPunctuation;
            word.pop_back();
        }

        // Reverses the word
        reverseString(word);

        // Adds the reversed word and its punctuation back to the sentence
        if (!firstWord) {
            oss << ' '; // Adds a space between words
        }

        oss << word << wordPunctuation;
        firstWord = false;
    }

    return oss.str() + sentencePunctuation; // Adds the punctuation at the end of the sentence
}


void reverseWordsInFile(std::ifstream& inputFile, std::ofstream& outputFile) {
    std::string line;

    while (std::getline(inputFile, line)) {
        std::string reversedLine = reverseLine(line);
        outputFile << reversedLine << '\n';
    }
}

int main() {
    char choice;
    do {
        std::cout << "Choose an option:\n";
        std::cout << "1. Process data from a file\n";
        std::cout << "2. Process data from the command line\n";
        std::cout << "3. Quit\n";
        std::cin >> choice;

        if (choice == '1') { // Checks the choice netered by the user
            std::string filename;
            std::ifstream inputFile;

            while (true) {
                std::cout << "Enter the filename: ";
                std::cin >> filename;
                inputFile.open(filename);

                if (!inputFile) {
                    std::cout << "File not found. Please try again.\n";
                }
                else {
                    break;
                }
            }

            std::ofstream outputFile("output.txt");

            try {
                reverseWordsInFile(inputFile, outputFile);
                std::cout << "Data processed and saved to output.txt\n";
            }
            catch (const StackError& e) {
                std::cerr << e.what() << std::endl;
            }
            catch (const QueueError& e) {
                std::cerr << e.what() << std::endl;
            }

            inputFile.close(); // Closes the input file
            outputFile.close(); // Closes the output file
        }
        else if (choice == '2') {
            std::cin.ignore(); // Consumes newline character
            std::string inputLine;

            std::cout << "Enter a line of text: ";
            std::getline(std::cin, inputLine);

            std::string reversedLine = reverseLine(inputLine);

            std::cout << "Reversed text: " << reversedLine << std::endl;
            std::cout << "\n";
        }
        else if (choice != '3') {
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != '3');

    return 0;
}
