// comports.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#using <System.dll>

using namespace System;
using namespace System::IO::Ports;
using namespace System::ComponentModel;

int main()
{
    array<String^>^ serialPorts = nullptr;
    try
    {
        // Get a list of serial port names.
        serialPorts = SerialPort::GetPortNames();
    }
    catch (Win32Exception^ ex)
    {
        Console::WriteLine(ex->Message);
		return 1;
    }
    // Display each port name to the console.
    for each(String^ port in serialPorts)
    {
        Console::WriteLine(port);
    }
	return 0;
}


