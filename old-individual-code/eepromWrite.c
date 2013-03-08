#include <EEPROM.h>
//rappel : byte : 2^8 = 256
void writeEeprom(byte utilisateur, byte size, byte weight, float temperature, byte frequence)
{
	int address = 0;

	while(address < 1000 && EEPROM.read(address) != 0)
		address++;

	if(address == 1000 || EEPROM.read(address + 5) == 0)
		;//TODO PRINT ON LCD NO MORE SPACE TO SAVE DATA
	else{
		EEPROM.write(address, utilisateur);
		EEPROM.write(address + 1, size);//Il faut q'un check de bonne valeur ai été fait -> 0 pas une bonne valeur
		EEPROM.write(address + 2, weight);
		EEPROM.write(address + 3, byte((temperature - 35.0)*10));//sur pc ca à l'effet désiré mais faut tester
		EEPROM.write(address + 4, frequence);
	}
}
