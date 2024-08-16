// Include the Winsock library (lib) file
#pragma comment(lib, "ws2_32.lib")

#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdio>

#include "include/MainController.h"
#include "include/EventsBus.h"


std::mutex g_terminationMtx;
std::condition_variable g_terminationCV;
bool g_shouldStop = false;

void userInputThread() {
  std::string input;
  while (std::getline(std::cin, input)) {
    if (input == "STOP") {
      {
        std::lock_guard<std::mutex> lk(g_terminationMtx);
        g_shouldStop = true;
      }
      g_terminationCV.notify_one();
      break;
    }
  }
}


int main() {
    std::string raw_input; // C:\Users\bogus\Documents\myProjects\UAV_PW\Cyprian\GCSServer\GCSServer\Cw3c_10.txt
    std::string raw_verbosity;
    std::string raw_port;

    std::cout << "Please specify the path for the flight configuration file: ";
    std::getline(std::cin, raw_input);
    std::filesystem::path p(raw_input);
    std::cout << "\n";

    std::cout << "Please specify the port for UAV (example: COM4): ";
    std::getline(std::cin, raw_port);

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
      MainController mc = MainController(p, eventsBus, raw_port, verbosity);
      std::jthread inputThread(userInputThread);
      std::jthread runThread([&mc]() {
        try {
          mc.run();
        } catch (const std::runtime_error &e) {
          std::cerr << "Critical error in MainController::run(): " << e.what()
                    << std::endl;
          {
            std::lock_guard<std::mutex> lk(g_terminationMtx);
            g_shouldStop = true;
          }
          g_terminationCV.notify_one();
          std::fclose(stdin); // used to shutdown inputThread, otherwise it causes deadlock
        }
      });
     

      std::cout << "Application is running, type STOP to terminate: ";
      {
        std::unique_lock<std::mutex> lk(g_terminationMtx);
        g_terminationCV.wait(lk, [] { return g_shouldStop; });
      }
      
      if (mc.shutdown()) {
        std::cout << "Application finished without issues\n";
      }

    } // scope of life for MainController

    return 0;
}
