#pragma once

enum Mode
{
	AUTO_LOCAL,
	AUTO_REMOT,
	MANU_REMOT
};

String ModeStrings[] = { "AUTO_LOCAL","AUTO_REMOT","MANU_REMOT" };

// NOTE
// we can get the string representation of the enum by doing 
// ModeStrings[AUTO_LOCAL]
// this mean that to add a value you need to add it two times