//
// MQ2GMChatCC.cpp
// written by Spiken
// https://github.com/spik3n/MQ2Camera
//

#include <mq/Plugin.h>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <set>

const int MAX_STRING_LENGTH = 512;
const int COMMAND_DELAY_MS = 2000;

char Phrases[50][MAX_STRING_LENGTH]; // Array to store phrases
int NumPhrases = 0; // Number of phrases loaded
int CooldownSeconds = 3600; // Default cooldown period
time_t lastUsedTime = 0;

void LoadINI();
void HandleCampCheckRequest();
void HandleGMGreetings(const char* line);

PreSetup("MQ2GMChatCC");
PLUGIN_VERSION(1.0);

#define PLUGIN_MSG "\ag[MQ2GMChatCC]\ax "

void LoadINI() {
	char debugMessage[MAX_STRING_LENGTH];
	sprintf_s(debugMessage, "Looking for INI file at path: %s", INIFileName);
	WriteChatColor(debugMessage, CONCOLOR_YELLOW);

	// Open and read the INI file
	std::ifstream iniFile(INIFileName);
	if (!iniFile.is_open()) {
		WriteChatColor("Failed to open INI file.", CONCOLOR_RED);
		return;
	}

	std::string line;
	while (std::getline(iniFile, line)) {
		// Check if the line starts with a digit (indicating a phrase number)
		if (isdigit(line[0])) {
			// Split the line into phrase number and phrase text
			int phraseNumber;
			char phraseText[MAX_STRING_LENGTH];
			sscanf_s(line.c_str(), "%d=%[^\r\n]", &phraseNumber, phraseText, MAX_STRING_LENGTH);

			// Check if the phrase number is within the valid range
			if (phraseNumber >= 1 && phraseNumber <= 50) {
				// Copy the phrase text to Phrases array
				strncpy_s(Phrases[phraseNumber - 1], phraseText, MAX_STRING_LENGTH - 1);
				Phrases[phraseNumber - 1][MAX_STRING_LENGTH - 1] = '\0'; // Ensure null-termination
				NumPhrases++;

				// Print debug message
				sprintf_s(debugMessage, "Loaded phrase %d: %s", phraseNumber, Phrases[phraseNumber - 1]);
				WriteChatColor(debugMessage, CONCOLOR_YELLOW);
			}
			else {
				sprintf_s(debugMessage, "Invalid phrase number: %d", phraseNumber);
				WriteChatColor(debugMessage, CONCOLOR_RED);
			}
		}
	}

	CooldownSeconds = GetPrivateProfileInt("Settings", "CooldownSeconds", 3600, INIFileName);
	char message[MAX_STRING_LENGTH];
	sprintf_s(message, PLUGIN_MSG "\ao v%.2f\ax by \agSpiken\ax", MQ2Version);
	WriteChatf(message);
}

void HandleCampCheckRequest() {
	char campCheckResponse[MAX_STRING_LENGTH];
	GetPrivateProfileString("Settings", "CampCheckResponse", "", campCheckResponse, MAX_STRING_LENGTH, INIFileName);

	if (campCheckResponse[0] == '\0') {
		WriteChatColor("No camp check response found in the INI file.", CONCOLOR_RED);
		return;
	}

	Sleep(COMMAND_DELAY_MS);
	EzCommand(campCheckResponse);
}

std::set<std::string> processedSayMessages;
std::set<std::string> processedTellMessages;
time_t lastCooldownResetTime = 0;

// Function to handle GM greetings
void HandleGMGreetings(const char* line, bool isSay) {
	if (NumPhrases == 0) {
		WriteChatColor("No phrases found in the INI file.", CONCOLOR_RED);
		return;
	}

	std::set<std::string>& processedMessages = isSay ? processedSayMessages : processedTellMessages;

	// Check if it's a say or tell message and it hasn't been processed already
	if (processedMessages.find(line) == processedMessages.end()) {
		int randomIndex = rand() % NumPhrases;
		char* selectedPhrase = Phrases[randomIndex];

		char command[MAX_STRING_LENGTH];
		if (isSay) {
			sprintf_s(command, "/say %s", selectedPhrase);
		}
		else {
			sprintf_s(command, "/r %s", selectedPhrase);
		}

		// Add a random delay before sending the message
		int randomDelay = rand() % 3000 + 1000;  // Random delay between 1 and 4 seconds
		Sleep(randomDelay);

		EzCommand(command);

		char reaction1[MAX_STRING_LENGTH];
		char reaction2[MAX_STRING_LENGTH];
		GetPrivateProfileString("Settings", "Reaction1", "/end", reaction1, MAX_STRING_LENGTH, INIFileName);
		GetPrivateProfileString("Settings", "Reaction2", "/unload", reaction2, MAX_STRING_LENGTH, INIFileName);

		Sleep(COMMAND_DELAY_MS);
		EzCommand(reaction1);

		Sleep(COMMAND_DELAY_MS);
		EzCommand(reaction2);

		// Add the processed message to the set
		processedMessages.insert(line);
	}
}

// Function to reset processed messages after cooldown period
void ResetProcessedMessages() {
	processedSayMessages.clear();
	processedTellMessages.clear();
	lastCooldownResetTime = std::time(nullptr);
}

PLUGIN_API VOID InitializePlugin(VOID) {
	DebugSpewAlways("Initializing MQ2GMChatCC");
	WriteChatColor("MQ2GMChatCC is initializing...", USERCOLOR_SYSTEM);
	if (gGameState == GAMESTATE_INGAME) {
		LoadINI();
		ResetProcessedMessages(); // Reset processed messages when plugin initializes
	}
}

PLUGIN_API VOID ShutdownPlugin(VOID) {
	DebugSpewAlways("Shutting down MQ2GMChatCC");
}

PLUGIN_API DWORD OnIncomingChat(PCHAR Line, DWORD Color) {
	time_t currentTime = std::time(nullptr);

	// Check if cooldown period has expired, reset processed messages if necessary
	if (currentTime - lastCooldownResetTime >= CooldownSeconds) {
		ResetProcessedMessages();
	}

	// Check if the line is a say or tell message
	bool isSay = strstr(Line, "says, '") != nullptr;
	bool isTell = strstr(Line, "tells you, '") != nullptr;

	// Check if it's a camp check request
	if (strstr(Line, "says out of character, 'camp check") ||
		strstr(Line, "says out of character, 'cc") ||
		strstr(Line, "says out of character, 'cc pls")) {
		HandleCampCheckRequest();
		return 0;
	}

	// Check for GM greetings in public chat or tells
	if ((isSay || isTell) && (strstr(Line, "Greetings! This is GM ") != nullptr)) {
		std::set<std::string>& processedMessages = isSay ? processedSayMessages : processedTellMessages;
		if (processedMessages.find(Line) == processedMessages.end()) {
			HandleGMGreetings(Line, isSay);
			processedMessages.insert(Line);
		}
	}

	return 0;
}