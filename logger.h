#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include <ctime>
#include <vector>
#include <string>
#include <ctime>
#include <deque>
#include <fstream>
using namespace std;

//log entry struct to hold log info
struct LogEntry {
    std::string timestamp;
    std::string type;
    std::string message;
    ImVec4 color; 
};

class Logger {
    public:
    //get singleton instance of logger
        static Logger& getInstance(){
            static Logger instance;
            return instance;
        }

        //delete copy constructor and assignment operator to prevent multiple instances of logger
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        //destructor to close log file if open
        ~Logger() {
            if (logFile.is_open()) {
                logFile.close();
            }
        }

        //logging functions
        void LogInfo(const std::string& message) {
            addLog("info", message, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); 
        }
        
        void LogWarning(const std::string& message) {
            addLog("warning", message, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); 
        }

        void LogError(const std::string& message) {
            addLog("error", message, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); 
        }

        void LogGameEvent(const std::string& message) {
            addLog("game event", message, ImVec4(0.0f, 0.5f, 1.0f, 1.0f)); 
        }

        void userInput(const std::string& message) {
            addLog("user", message, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
        }

        //render log window in ImGui
        void renderLogWindow(bool* p_open = nullptr){
          
            ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
            ImGui::Begin("Game Log", p_open);
            //test buttons 
            if (ImGui::Button("Test Info"))
            {
                Logger::getInstance().LogInfo("This is an info message!");
            }
            ImGui::SameLine();
            if (ImGui::Button("Test Warning"))
            {
                Logger::getInstance().LogWarning("This is a warning message!");
            }
            ImGui::SameLine();
            if (ImGui::Button("Test Error"))
            {
                Logger::getInstance().LogError("This is an error message!");
            }
            ImGui::SameLine();
            if (ImGui::Button("Test Game Event"))
            {
                Logger::getInstance().LogGameEvent("This is a game event message!");
            }

            ImGui::Separator();
            
            //bool of logging levels 
            static bool show_info = true;
            static bool show_warning = true;
            static bool show_error = true;
            static bool show_game_event = true;
            static bool show_user_input = true;
            
            //create option button to show or hide log entries based on their type
            if (ImGui::Button("Options"))
            {
                ImGui::OpenPopup("Option Menu");
            }

            //create popup menu for options
            if(ImGui::BeginPopup("Option Menu")){
                ImGui::Text("Show Log Types:");
                ImGui::Separator();
                //checkboxes to show or hide log entries based on their type
                ImGui::Checkbox("Show Info", &show_info);
                ImGui::Checkbox("Show Warning", &show_warning);
                ImGui::Checkbox("Show Error", &show_error);
                ImGui::Checkbox("Show Game Event", &show_game_event);
                ImGui::Checkbox("Show User Input", &show_user_input);
                ImGui::EndPopup();
            }
            ImGui::SameLine();

            //clear log button
            if (ImGui::Button("Clear Log"))
            {
                logEntries.clear();
            }

            ImGui::Separator(); 

            //creating space for the input text box at the bottom
            const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
            if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (ImGui::BeginPopupContextWindow())
            {
                ImGui::EndPopup();
                
            }

            //logic for displaying log entries based on their type and the options selected by the user
            for (const auto& entry : logEntries) {
                if ((entry.type == "info" && show_info) ||
                    (entry.type == "warning" && show_warning) ||
                    (entry.type == "error" && show_error) ||
                    (entry.type == "game event" && show_game_event) ||
                    (entry.type == "user" && show_user_input))
                {
                    ImGui::TextColored(entry.color, "%s [%s]: %s", entry.timestamp.c_str(), entry.type.c_str(), entry.message.c_str());
                }
            }

        }
            ImGui::EndChild();

            
            ImGui::Separator();

            //text input box
            static std::string input_text = "";
            //call function to create text input box
            textInput("Console", input_text);
            


            ImGui::SameLine();
            //if user presses enterd, display text input in log window
            if (ImGui::Button("Enter") || ImGui::IsKeyPressed(ImGuiKey_Enter))
            {
                //log user input
                Logger::getInstance().userInput(input_text);
                //clear input text
                input_text = "";
                
            }

            ImGui::End();

            
        }
        //function to create text input box
        void textInput(const std::string& label, std::string& input) {
            //buffer to hold input text, with a max size of 256 characters
            char buffer[256];
            //copy input string to buffer
            strncpy_s(buffer, input.c_str(), sizeof(buffer));
            //create input text box with ImGui, and update input string if the text changes
            if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer))) {
                input = std::string(buffer);
            }
        }

       

    private:
    
        Logger() {
            //open log file in append mode
            logFile.open("log.txt", std::ios::app);
            if (!logFile.is_open()) {
                std::cerr << "failed to open log file" << std::endl;
            }
        }

        //create log entry struct
        std::deque<LogEntry> logEntries;
        //max entries
        static const int MAX_ENTRIES = 1000;
        //create log file stream
        std::ofstream logFile;

        void addLog(const std::string& type, const std::string& message, const ImVec4& color){
            //timestamp
            std::time_t now = std::time(nullptr);
            char buf[20];
            std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&now));
            std::string timestamp(buf);

            //create log entry with timestamp, type, message, and color
            LogEntry entry{timestamp, type, message, color};

            //add log entries
            logEntries.push_back(entry);

            //remove old entries if we exceed the max limit
            if(logEntries.size() > MAX_ENTRIES){
                logEntries.pop_front();
            }   
            
            std::cerr << "[" << timestamp << "] [" << type << "]: " << message << std::endl;
            
            //write to file
            if (logFile.is_open()) {
                logFile << "[" << timestamp << "] [" << type << "]: " << message << std::endl;
                
            }
        }

        
        

        
    
};

#endif // LOGGER_H