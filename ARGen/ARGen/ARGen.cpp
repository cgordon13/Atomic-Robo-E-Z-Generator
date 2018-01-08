/*
Copyright Chris Gordon 2017

Shared under the GPLv3 license - https://www.gnu.org/licenses/gpl-3.0.en.html
*/

#include "stdafx.h"

#include <string>
#include <iostream>

#include <map>
#include <vector>
#include <algorithm>


static const std::string Skill_Athletics = "Athletics";
static const std::string Skill_Burglary = "Burglary";
static const std::string Skill_Combat = "Combat";
static const std::string Skill_Contacts = "Contacts";
static const std::string Skill_Deceive = "Deceive";
static const std::string Skill_Empathy = "Empathy";
static const std::string Skill_Notice = "Notice";
static const std::string Skill_Physique = "Physique";
static const std::string Skill_Provoke = "Provoke";
static const std::string Skill_Rapport = "Rapport";
static const std::string Skill_Science = "Science";
static const std::string Skill_Stealth = "Stealth";
static const std::string Skill_Vehicles = "Vehicles";
static const std::string Skill_Will = "Will";

static const std::string Mode_Action = "Action";
static const std::string Mode_Banter = "Banter";
static const std::string Mode_Intrigue = "Intrigue";
static const std::string Mode_Science = "Science";

static const std::vector<std::string> Mode_Skills_Action    { Skill_Athletics, Skill_Combat, Skill_Notice, Skill_Physique, Skill_Provoke, Skill_Vehicles };
static const std::vector<std::string> Mode_Skills_Banter    { Skill_Contacts, Skill_Deceive, Skill_Empathy, Skill_Provoke, Skill_Rapport, Skill_Will };
static const std::vector<std::string> Mode_Skills_Intrigue  { Skill_Athletics, Skill_Burglary, Skill_Contacts, Skill_Deceive, Skill_Notice, Skill_Stealth };
static const std::vector<std::string> Mode_Skills_Science   { Skill_Notice, Skill_Will, Skill_Science };

static const std::map<std::string, const std::vector<std::string>> Modes_Skills { 
	{ Mode_Action, Mode_Skills_Action }, 
	{ Mode_Banter, Mode_Skills_Banter }, 
	{ Mode_Intrigue, Mode_Skills_Intrigue}, 
	{ Mode_Science, Mode_Skills_Science }
};

// The basic allowance for EZ No Math creation is that each mode costs 9 points.  Science costs 3, so there are 6 more points available.  
// When adding new Modes, ensure you don't sum up to 10 points because you will have one extra.  In the case that you have 10 points of value in a Weird mode, use -1 as the value.
// improvement costs are on p. 34
static const std::map<std::string, int> Mode_Improvements{
	{Mode_Science, 6 } //"Specialize one trained skill", "Focus one trained skill", "Specialize one focussed skill"
};

void print_mode(std::vector<std::string> v_modes) {
	int physical_stress = 2;
	int mental_stress = 2;
	int either_stress = 0;

	std::map<std::string, int> skills;

	for (int i = 0; i < 3; i++)
	{
		auto test = Modes_Skills.at(Mode_Action);

		bool physical = false;
		bool mental = false;

		// accumulate skills that are reinforced
		for (auto skill : Modes_Skills.at(v_modes[i])) {

			// check for physical stress box skills
			if ((skill.compare(Skill_Athletics) == 0) || (skill.compare(Skill_Physique) == 0))
			{
				physical = true;
			}

			// check for mental stress box skills
			if ((skill.compare(Skill_Provoke) == 0) || (skill.compare(Skill_Will) == 0))
			{
				mental = true;
			}

			// if it's a new skill, add it at the basic Mode value
			if (skills.find(skill) == skills.end()) {
				skills[skill] = 3 - i;
			}
			// if it's a previously added skill, re-inforce the existing skill instead of adding it again.
			else {
				skills[skill] = skills[skill]++;
			}
		}

		// determine stress boxes.  If you have a skill that has both types of stress, you can allocate those boxes as you see fit.  (Action is the only basic Mode that does this)
		if (physical && mental) {
			if (2 - i > 0)
				either_stress += 2 - i;
		}
		else if (physical) {
			if (2 - i > 0)
				physical_stress += 2 - i;
		}
		else if (mental) {
			if (2 - i > 0)
				mental_stress += 2 - i;
		}

	}

	int improvements = 3; //{ "Specialize one trained skill OR Focus one trained skill and specialize one focused skill OR Focus three trained skills." };
	//std::vector<std::string> improvements = { "Everyone (p.34)" };

	// Pull out skills by Mode and report
	for (int i = 0; i < 3; i++)
	{
		if (Mode_Improvements.find(v_modes[i]) != Mode_Improvements.end()) {
			auto new_improvements = Mode_Improvements.at(v_modes[i]);
			improvements += new_improvements;
		}

		std::cout << v_modes[i] << "(+" << 3 - i << +") - ";
		for (auto skill : Modes_Skills.at(v_modes[i])) {
			if (skills.find(skill) != skills.end()) {
				std::cout << skill << "(+" << skills[skill] << ") ";
				skills.erase(skill);
			}
		}
		std::cout << std::endl;
	}

	// report on stress
	std::cout << "Stress: Physical(" << physical_stress << "), Mental(" << mental_stress << ")";
	if (either_stress > 0) {
		std::cout << ", Either(" << either_stress << ")";
	}
	std::cout << std::endl;

	// report on Improvements
	std::cout << "Improvements: " << improvements << " points" << std::endl;

	std::cout << std::endl;
}

int main()
{

	std::vector<std::string> v_modes{ Mode_Action, Mode_Banter, Mode_Intrigue, Mode_Science };

	std::sort(v_modes.begin(), v_modes.end());

	do {

		print_mode(v_modes);

	} while (std::next_permutation(v_modes.begin(), v_modes.end()));

    return 0;
}

