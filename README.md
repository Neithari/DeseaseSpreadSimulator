# Disease Spread Simulator #

This will simulate the spread of a droplet transmitted disease.

You will be able to measure the difference of containment measures.

Containment measures are as follows:
 - Mask mandate (Mixture of diy masks, medical masks and FFP 2+ masks)
 - Working from home (All jobs that are capable of doing)
 - Closing non essential shops
 - Full lockdown (Except essential jobs)

 You can control the simulation parameters with console parameters.
 - -p 1000 -> Will set the population to the given number.
 - -d 365 -> Will set the days the simulation will run to the given number.
 - -n 1 -> Will set the number of runs to the given number.
 - -f disease.json -> Will use the disease inside the json file. See src/DiseaseSpreadSimulator/sampleDiseaseFile.json for the format.
 - -o -> Will print a daily summary

CMake files do use project options licenced under MIT and available here:
https://github.com/aminya/project_options