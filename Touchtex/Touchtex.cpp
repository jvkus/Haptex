// Touchtex.cpp : Defines the entry point for the console application.
// May rename project to Haptex later

#include "stdafx.h"
#include "steam_api.h"
#include "isteaminput.h"
#include <iostream>
#include <conio.h>
#include <math.h>
#pragma comment(lib, "win/steam_api")

const double SPEED_OF_SOUND = 331;

// Returns a value equal to the total amount of microseconds required to complete one cycle of a given frequency.
int frequencyToCycleTime(double frequency) {

	double wavelength = 1 / frequency;
	return round(wavelength * 1000000 / 2);

}

int main()
{
	int numActive;
	int testInput;
	bool controllerNotFound = true;
	bool continueTest = true;

	SteamAPI_Init();		// Needs to run in order to use any other part of the Steam API
	SteamInput()->Init();	// Needs to run in order to use any part of ISteamInput

	// Refreshes Steam API's stuff... Basically, allows you to see what's connected and its states
	SteamInput()->RunFrame();

	// Initialize the controller ID array and the active controller ID
	InputHandle_t activeController = 0;
	InputHandle_t inputHandles[STEAM_CONTROLLER_MAX_COUNT];

	// Searches for an active controller
	while (controllerNotFound)	{

		std::cout << "Press A..." << std::endl;

		// Part of conio.h. Waits for a key press of some sort.
		getchar();

		SteamInput()->RunFrame();
		numActive = SteamInput()->GetConnectedControllers(inputHandles); //Rewrite into a cleaner function.

		activeController = inputHandles[0];
		std::cout << "Active controller ID: " << activeController << std::endl;

		if (numActive > 0) {

			std::cout << "Controller found." << std::endl;
			controllerNotFound = false;

		}
		else {

			std::cout << "Controller not found. Try again." << std::endl;

		}
	}

	// set of variables to adjust haptic stuff
	char padInput = 'b';
	int pulseDuration = 0;	// store in microseconds
	int pulseDuration2 = 0;
	int pauseDuration = 0;
	int pauseDuration2 = 0;
	int repetition = 0;

	// Vibration tests
	while (continueTest) {

		std::cout << "Enter a test pulse. ";
		std::cin >> testInput;

		switch (testInput) {

		// Triggers a single haptic pulse. Parameters specify pulse duration.
		case 1:
			std::cout << std::endl << "Which pad do you want to trigger? l, r, b (left, right, or both)";
			std::cin >> padInput;

			std::cout << std::endl << "Enter a pulse duration. ";
			std::cin >> pulseDuration;

			if (padInput == 'l') {

				SteamInput()->TriggerHapticPulse(activeController, k_ESteamControllerPad_Left, pulseDuration);

			}
			else if (padInput == 'r') {

				SteamInput()->TriggerHapticPulse(activeController, k_ESteamControllerPad_Right, pulseDuration);

			}
			else if (padInput == 'b') {

				SteamInput()->TriggerHapticPulse(activeController, k_ESteamControllerPad_Left, pulseDuration);
				SteamInput()->TriggerHapticPulse(activeController, k_ESteamControllerPad_Right, pulseDuration);

			}
			else {

				std::cout << "Invalid input." << std::endl;

			}
			break;

		// Triggers repeated haptic pulses. Parameters specify pulse duration, pause duration, and number of repetitions. Last parameter is unused.
		case 2:
			std::cout << std::endl << "Which pad do you want to trigger? l, r, b (left, right, or both)";
			std::cin >> padInput;

			std::cout << std::endl << "Enter a pulse duration. ";
			std::cin >> pulseDuration;

			std::cout << std::endl << "Enter a pause duration. ";
			std::cin >> pauseDuration;

			std::cout << std::endl << "Enter the number of times to repeat the pulse. ";
			std::cin >> repetition;

			if (padInput == 'l') {

				SteamInput()->TriggerRepeatedHapticPulse(activeController, k_ESteamControllerPad_Left, pulseDuration, pauseDuration, repetition, 0);

			}
			else if (padInput == 'r') {

				SteamInput()->TriggerRepeatedHapticPulse(activeController, k_ESteamControllerPad_Right, pulseDuration, pauseDuration, repetition, 0);

			}
			else if (padInput == 'b') {

				SteamInput()->TriggerRepeatedHapticPulse(activeController, k_ESteamControllerPad_Left, pulseDuration, pauseDuration, repetition, 0);
				SteamInput()->TriggerRepeatedHapticPulse(activeController, k_ESteamControllerPad_Right, pulseDuration, pauseDuration, repetition, 0);

			}
			else {

				std::cout << "Invalid input." << std::endl;

			}
			break;

		// Triggers vibration. Parameters specify intensity of left and right motors.
		case 3:
			std::cout << std::endl << "Enter a left pad intensity. ";
			std::cin >> pulseDuration;

			std::cout << std::endl << "Enter a right pad intensity. ";
			std::cin >> pauseDuration;

			SteamInput()->TriggerVibration(activeController, pulseDuration, pauseDuration);
			break;

		case 4:
			std::cout << std::endl << "Enter a tone frequency. ";
			std::cin >> pulseDuration;
			pulseDuration = frequencyToCycleTime(pulseDuration);
			pauseDuration = pulseDuration;
			repetition = 500;

			SteamInput()->TriggerRepeatedHapticPulse(activeController, k_ESteamControllerPad_Left, pulseDuration, pauseDuration, repetition, 0);
			SteamInput()->TriggerRepeatedHapticPulse(activeController, k_ESteamControllerPad_Right, pulseDuration, pauseDuration, repetition, 0);

			break;

		case 5:
			std::cout << std::endl << "Enter a left tone frequency. ";
			std::cin >> pulseDuration;
			pulseDuration = frequencyToCycleTime(pulseDuration);
			pauseDuration = pulseDuration;
			repetition = 5000;

			std::cout << std::endl << "Enter a right tone frequency. ";
			std::cin >> pulseDuration2;
			pulseDuration2 = frequencyToCycleTime(pulseDuration2);
			pauseDuration2 = pulseDuration2;
			repetition = 5000;

			SteamInput()->TriggerRepeatedHapticPulse(activeController, k_ESteamControllerPad_Left, pulseDuration, pauseDuration, repetition, 0);
			SteamInput()->TriggerRepeatedHapticPulse(activeController, k_ESteamControllerPad_Right, pulseDuration2, pauseDuration2, repetition, 0);

			break;

		case 0:
			continueTest = false;
			break;

		default:
			std::cout << std::endl << "No valid input received.";

		}
	}
			
	// Shuts down everything
 	SteamInput()->Shutdown();
	SteamAPI_Shutdown();
    return 0;
}