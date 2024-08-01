#include <thread>

#include "include/MainController.h"
#include "include/EventsBus.h"


int main() {
    std::string raw_input; // C:\Users\bogus\Documents\myProjects\UAV_PW\Cyprian\GCSServer\GCSServer\Cw3c_10.txt
    std::string raw_verbosity;

    std::cout << "Please specify the path for the flight configuration file: ";
    std::getline(std::cin, raw_input);
    std::filesystem::path p(raw_input);

    std::cout << "\n";
    bool isVerbositySetCorrect = false;
    bool verbosity = false;
    while (!isVerbositySetCorrect) {
      std::cout << "Do you want the output to be verbose?[yes/no]: ";
      std::getline(std::cin, raw_verbosity);

      if (raw_verbosity == "yes") {
        verbosity = true;
        isVerbositySetCorrect = true;
      } else if (raw_verbosity == "no") {
        isVerbositySetCorrect = true;
      } else {
        std::cout << "Type yes or no !" << std::endl;
      }
    }
    
    EventsBus eventsBus = EventsBus();
    {
      MainController mc = MainController(p, eventsBus, verbosity);

      std::jthread runThread([&mc]() { mc.run(); });
      std::cout << "Application is running, type STOP to terminate: ";

      std::string input;
      while (std::getline(std::cin, input)) {
        if (input == "STOP") {
          if (mc.shutdown()) {
            break;
          }
        } else {
          std::cout << "Type STOP to terminate" << std::endl;
        }
      }
    } // scope of life for MainController

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
