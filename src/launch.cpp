#include <iostream>
#include <string>
#include <spibus.h>
#include <st_accel_dsh.h>
#include <unistd.h>
#include <csignal>

using namespace std;
using namespace Sensor;

static bool loop = false;

void signal_handler(int signal)
{
    if (signal == SIGINT && loop)
        loop = false;
    else{
        cout << "death by signal..." << endl;
        exit(1);
    }
}

bool SpiGetCmd(Communication::SpiBus & spibus, string arg){

    if (arg == "mode"){
        Communication::spi_mode spiarg;
        if (spibus.GetMode(spiarg))
            cout << "Mode " << (unsigned int)spiarg << " ("<< Communication::GetSpiModeDescription(spiarg) << ")" << endl;
        else
            cout << "An error occured!" << endl;
    } else if (arg == "speed"){
        Hertz spiarg;
        if (spibus.GetSpeed(spiarg))
            cout << spiarg << " Hz" << endl;
        else
            cout << "An error occured!" << endl;
    } else if (arg == "bpw") {
        BitsPerWord spiarg;
        if (spibus.GetBitsPerWord(spiarg))
            cout << (unsigned int)spiarg << " bits per word" << endl;
        else
            cout << "An error occured!" << endl;
    } else if (arg == "endian") {
        Endianess spiarg;
        if (spibus.GetEndianness(spiarg))
            cout << ((bool)spiarg?"LSB":"MSB") << endl;
        else
            cout << "An error occured!" << endl;
    } else if (arg == "delay") {
        MicroSeconds spiarg;
        if (spibus.GetDelay(spiarg))
            cout << spiarg << " us" << endl;
        else
            cout << "An error occured!" << endl;
    } else
        return false;
    return true;
}
bool SpiSetCmd(Communication::SpiBus & spibus, string arg){
    string subarg;
    if (arg.substr(0,4) == "mode"){
        Communication::spi_mode spiarg;
        subarg = arg.erase(0,5);

        try {
            unsigned int tmp = stoul(subarg, NULL);
            spiarg = (Communication::spi_mode)tmp;
        } catch (...) {
            return false;
        }

        if (spibus.SetMode(spiarg))
            cout << "Mode " << (unsigned int)spiarg << " ("<< Communication::GetSpiModeDescription(spiarg) << ")" << endl;
        else
            cout << "An error occured!" << endl;
    } else if (arg.substr(0,5) == "speed"){
        Hertz spiarg;
        subarg = arg.erase(0,6);

        try {
            spiarg = stoul(subarg, NULL);
        } catch (...) {
            return false;
        }

        if (spibus.SetSpeed(spiarg))
            cout << spiarg << " Hz" << endl;
        else
            cout << "An error occured!" << endl;
    } else if (arg.substr(0,3) == "bpw") {
        BitsPerWord spiarg;
        subarg = arg.erase(0,4);

        try {
            unsigned int tmp = stoul(subarg, NULL);
            spiarg = (BitsPerWord)tmp;
        } catch (...) {
            return false;
        }

        if (spibus.SetBitsPerWord(spiarg))
            cout << (unsigned int)spiarg << " bits per word" << endl;
        else
            cout << "An error occured!" << endl;
    } else if (arg.substr(0,6) == "endian") {
        Endianess spiarg;
        subarg = arg.erase(0,7);

        try {
            unsigned int tmp = stoul(subarg, NULL);
            spiarg = (tmp==0 ? MSB : LSB );
        } catch (...) {
            return false;
        }

        if (spibus.SetEndianness(spiarg))
            cout << ((bool)spiarg?"LSB":"MSB") << endl;
        else
            cout << "An error occured!" << endl;
    } else if (arg.substr(0,5) == "delay") {
        MicroSeconds spiarg;
        subarg = arg.erase(0,6);

        try {
            spiarg = stoul(subarg, NULL);
        } catch (...) {
            return false;
        }

        if (spibus.SetSpeed(spiarg))
            cout << spiarg << " Hz" << endl;
        else
            cout << "An error occured!" << endl;
    } else
        return false;
    return true;
}
void PrintSpiHelp(){
    cout << "--- Spi Test Tool ---" << endl << endl;
    cout << "Commands:" << endl;
    cout << "\tread\tReads whatever valid argument is passed from the spi device" << endl;
    cout << "\twrite\tAttemts to write a valid argument to the spi device" << endl;
    cout << "\test\tWrites to the SPI device and recieves the same number of byts written" << endl;
    cout << "\thelp\tShow the help message" << endl;
    cout << "\tquit\tQuits the program" << endl << endl;
    cout << "Arguments:" << endl;
    cout << "\tmode (0-3), speed (Hz), bpw (bits), delay (us), endian (1 = LSB, 0 = MSB)" << endl << endl;
}
int SpiTester(){
    string cmd;
    Communication::SpiBus spi0("/dev/spidev1.0");

    PrintSpiHelp();

    if(spi0.Open()){
        while(spi0.IsOpen()){
            cout << "> ";
            getline (std::cin,cmd);

            if(cmd == "quit" || cmd == "exit"){
                spi0.Close();
            }else if(cmd == "help"){
                PrintSpiHelp();
            }else if(cmd.substr(0,4) == "read"){
                string arg = cmd.erase(0,5);
                if (!SpiGetCmd(spi0,arg))
                    cout << "Invalid arguement" << endl;
            }else if(cmd.substr(0,5) == "write"){
                string arg = cmd.erase(0,6);
                if (!SpiSetCmd(spi0,arg))
                    cout << "Invalid arguement" << endl;
            }else if(cmd.substr(0,4) == "test"){

            }
        }
    }else{
        cout << "could not open spi device" << endl;
    }

    return 0;
}

void PrintAccelHelp(){
    cout << "--- ST LIS3DSH Accelerometer Test Tool---" << endl << endl;
    cout << "Commands:" << endl;
    cout << "\thelp\tShow the help message" << endl;
    cout << "\tquit\tQuits the program" << endl;
    cout << "\tcall\tcalls a method of the object" << endl;
}
int AccelerometerTest(){
    string cmd;
    StAccel_dsh * accel = NULL;
    bool loop = true;

    PrintAccelHelp();

    try {
        accel = new StAccel_dsh();
    } catch (int e) {
        cout << "Exception: " << e << endl;
        if(accel != NULL)
            delete accel;
        return 1;
    }

    while(loop){
        cout << "> ";
        getline (std::cin, cmd);

        if(cmd == "quit" || cmd == "exit"){
            loop = false;
            break;
        }
        else if(cmd == "help"){
            PrintAccelHelp();
        }
        else if(cmd.substr(0,9) == "SoftReset" || cmd.substr(0,9) == "softreset"){
            cout << (accel->SoftReset() ? "o " : "x " );
        }
        else  if(cmd.substr(0,6) == "Reboot" || cmd.substr(0,6) == "reboot"){
            cout << (accel->Reboot() ? "o " : "x " );
        }
        else if(cmd.substr(0,8) == "ReadInfo" || cmd.substr(0,8) == "readinfo"){
            unsigned short val;
            bool rtrn = accel->ReadInfomation(val);
            cout << "\t" << val << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,6) == "WhoAmI" || cmd.substr(0,6) == "whoami"){
            Word val;
            bool rtrn = accel->WhoAmI(val);
            cout << "\t" << (unsigned int)val << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,6) == "GetODR" || cmd.substr(0,6) == "getodr"){
            StAccel_dsh::ODR val;
            bool rtrn = accel->GetODR(val);
            cout << "\t" << (unsigned int)val << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,6) == "SetODR" || cmd.substr(0,6) == "setodr"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,7), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->SetODR((StAccel_dsh::ODR)val) ? "o " : "x " );
        }
        else if(cmd.substr(0,9) == "GetRange" || cmd.substr(0,9) == "getrange"){
            StAccel_dsh::MeasureRange val;
            bool rtrn = accel->GetRange(val);
            cout << "\t" << (unsigned int)val << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,8) == "SetRange" || cmd.substr(0,8) == "setrange"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,9), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->SetRange((StAccel_dsh::MeasureRange)val) ? "o " : "x " );
        }
        else if(cmd.substr(0,13) == "IsFifoEnabled" || cmd.substr(0,13) == "isfifoenabled"){
            bool val;
            bool rtrn = accel->IsFifoEnabled(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,7) == "UseFifo" || cmd.substr(0,7) == "usefifo"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,7), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->UseFifo((val==0?false:true)) ? "o " : "x " );
        }
        else if(cmd.substr(0,11) == "IsFifoEmpty" || cmd.substr(0,11) == "isfifoempty"){
            bool val;
            bool rtrn = accel->IsFifoEmpty(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,13) == "IsFifoOverrun" || cmd.substr(0,13) == "isfifooverrun"){
            bool val;
            bool rtrn = accel->IsFifoOverrun(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,14) == "DataOverrunXYZ" || cmd.substr(0,14) == "dataoverrunxyz"){
            bool val;
            bool rtrn = accel->DataOverrunXYZ(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,12) == "DataOverrunX" || cmd.substr(0,12) == "dataoverrunx"){
            bool val;
            bool rtrn = accel->DataOverrunX(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,12) == "DataOverrunY" || cmd.substr(0,12) == "dataoverruny"){
            bool val;
            bool rtrn = accel->DataOverrunY(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,12) == "DataOverrunZ" || cmd.substr(0,12) == "dataoverrunz"){
            bool val;
            bool rtrn = accel->DataOverrunZ(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,16) == "DataAvailableXYZ" || cmd.substr(0,16) == "dataavailablexyz"){
            bool val;
            bool rtrn = accel->DataAvailableXYZ(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,14) == "DataAvailableX" || cmd.substr(0,14) == "dataavailablex"){
            bool val;
            bool rtrn = accel->DataAvailableX(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,14) == "DataAvailableY" || cmd.substr(0,14) == "dataavailabley"){
            bool val;
            bool rtrn = accel->DataAvailableY(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,14) == "DataAvailableZ" || cmd.substr(0,14) == "dataavailablez"){
            bool val;
            bool rtrn = accel->DataAvailableZ(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,14) == "IsXAxisEnabled" || cmd.substr(0,14) == "isxaxisenabled"){
            bool val;
            bool rtrn = accel->IsXAxisEnabled(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,14) == "IsYAxisEnabled" || cmd.substr(0,14) == "isyaxisenabled"){
            bool val;
            bool rtrn = accel->IsYAxisEnabled(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,14) == "IsZAxisEnabled" || cmd.substr(0,14) == "iszaxisenabled"){
            bool val;
            bool rtrn = accel->IsZAxisEnabled(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,8) == "UseAxisX" || cmd.substr(0,8) == "useaxisx"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,9), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->UseAxisX((val==0?false:true)) ? "o " : "x " );
        }
        else if(cmd.substr(0,8) == "UseAxisY" || cmd.substr(0,8) == "useaxisy"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,9), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->UseAxisY((val==0?false:true)) ? "o " : "x " );
        }
        else if(cmd.substr(0,8) == "UseAxisZ" || cmd.substr(0,8) == "useaxisz"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,9), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->UseAxisZ((val==0?false:true)) ? "o " : "x " );
        }
        else if(cmd.substr(0,5) == "IsBDU" || cmd.substr(0,5) == "isbdu"){
            bool val;
            bool rtrn = accel->IsBDU(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,6) == "UseBDU" || cmd.substr(0,6) == "usebdu"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,7), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->UseBDU((val==0?false:true)) ? "o " : "x " );
        }
        else if(cmd.substr(0,9) == "IsReadInc" || cmd.substr(0,9) == "isreadinc"){
            bool val;
            bool rtrn = accel->IsReadInc(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,10) == "UseReadInc" || cmd.substr(0,10) == "usereadinc"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,11), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->UseReadInc((val==0?false:true)) ? "o " : "x " );
        }
        else if(cmd.substr(0,15) == "ReadAccel" || cmd.substr(0,15) == "readaccel"){
            Sensor::RawAcceleromterData val;
            bool rtrn = accel->ReadSensorDataOnce(val);
            GForce * out = accel->ConvertToSIUnit(val);
            cout << "\t" << "X: "<< out[0] << " g" << endl;
            cout << "\t" << "Y: "<< out[1] << " g" << endl;
            cout << "\t" << "Z: "<< out[2] << " g" << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,14) == "ReadTemp" || cmd.substr(0,14) == "readtemp"){
            Sensor::RawThermometerData val;
            bool rtrn = accel->ReadSensorDataOnce(val);
            cout << "\t" << accel->ConvertToSIUnit(val) << "°" << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,9) == "GetOffset" || cmd.substr(0,9) == "getoffset"){
            Sensor::RawAcceleromterData val;
            bool rtrn = accel->GetOffset(val);
            cout << "\t" << "X:" << val.nX_Data << endl;
            cout << "\t" << "Y:" << val.nY_Data << endl;
            cout << "\t" << "Z:" << val.nZ_Data << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,9) == "SetOffset" || cmd.substr(0,9) == "setoffset"){
            Sensor::RawAcceleromterData val;
            cout << "X: ";
            cin >> val.nX_Data;
            cout << "Y: ";
            cin >> val.nY_Data;
            cout << "Z: ";
            cin >> val.nZ_Data;
            bool rtrn = accel->SetOffset(val);
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,15) == "GetSelfTestMode" || cmd.substr(0,15) == "getselftestmode"){
            StAccel_dsh::SelfTestMode val;
            bool rtrn = accel->GetSelfTestMode(val);
            cout << "\t" << (unsigned int)val << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,15) == "SetSelfTestMode" || cmd.substr(0,15) == "setselftestmode"){
            StAccel_dsh::SelfTestMode val;

            try {
                val = (StAccel_dsh::SelfTestMode)stoul(cmd.erase(0,16), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            bool rtrn = accel->SetSelfTestMode(val);
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,12) == "GetAntiAlias" || cmd.substr(0,12) == "getantialias"){
            StAccel_dsh::AntiAliasingBandwidth val;
            bool rtrn = accel->GetAntiAliasFilterBandwidth(val);
            cout << "\t" << (unsigned int)val << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,12) == "SetAntiAlias" || cmd.substr(0,12) == "setantialias"){
            StAccel_dsh::AntiAliasingBandwidth val;

            try {
                val = (StAccel_dsh::AntiAliasingBandwidth)stoul(cmd.erase(0,13), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            bool rtrn = accel->SetAntiAliasFilterBandwidth(val);
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,14) == "IsSPI3WireMode" || cmd.substr(0,14) == "isspi3wiremode"){
            bool val;
            bool rtrn = accel->IsSPI3WireMode(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,15) == "UseSPI3WireMode" || cmd.substr(0,15) == "usespi3wiremode"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,16), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->UseSPI3WireMode((val==0?false:true)) ? "o " : "x " );
        }
        else if(cmd.substr(0,11) == "GetFifoMode" || cmd.substr(0,11) == "getfifomode"){
            StAccel_dsh::FifoMode val;
            bool rtrn = accel->GetFifoMode(val);
            cout << "\t" << (unsigned int)val << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,11) == "SetFifoMode" || cmd.substr(0,11) == "setfifomode"){
            StAccel_dsh::FifoMode val;

            try {
                val = (StAccel_dsh::FifoMode)stoul(cmd.erase(0,12), NULL);
            } catch (...) {
                cout << "exception" << endl;
                cout << "x ";
                continue;
            }

            cout << (accel->SetFifoMode((StAccel_dsh::FifoMode)val) ? "o " : "x " );
        }
        else if(cmd.substr(0,18) == "IsWatermarkEnabled" || cmd.substr(0,18) == "iswatermarkenabled"){
            bool val;
            bool rtrn = accel->IsFifoWatermark(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,12) == "UseWatermark" || cmd.substr(0,12) == "usewatermark"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,13), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->UseFifoWatermark((val==0?false:true)) ? "o " : "x " );
        }
        else if(cmd.substr(0,18) == "GetWatermarkStatus" || cmd.substr(0,18) == "getwatermarkstatus"){
            bool val;
            bool rtrn = accel->GetFifoWatermarkStatus(val);
            cout << "\t" << (val?"Higher or equal":"Lower") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,19) == "GetFifoFilledLength" || cmd.substr(0,19) == "getfifofilledlength"){
            Word val;
            bool rtrn = accel->GetFifoFilledLength(val);
            cout << "\t" << (unsigned int)val << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,19) == "GetWatermarkPointer" || cmd.substr(0,19) == "getwatermarkpointer"){
            Word val;
            bool rtrn = accel->GetFifoWatermarkPointer(val);
            cout << "\t" << (unsigned int)val << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,19) == "SetWatermarkPointer" || cmd.substr(0,19) == "setwatermarkpointer"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,20), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->SetFifoWatermarkPointer((char)val) ? "o " : "x " );
        }
        else if(cmd.substr(0,9) == "IsIntDrdy" || cmd.substr(0,9) == "isintdrdy"){
            bool val;
            bool rtrn = accel->IsIntDrdy(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,10) == "UseIntDrdy" || cmd.substr(0,10) == "useintdrdy"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,11), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->UseIntDrdy((val==0?false:true)) ? "o " : "x " );
        }
        else if(cmd.substr(0,14) == "GetIntPolarity" || cmd.substr(0,14) == "getintpolarity"){
            LogicState val;
            bool rtrn = accel->GetIntPolarity(val);
            cout << "\t" << (val==0?"Low":"High") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,14) == "SetIntPolarity" || cmd.substr(0,14) == "setintpolarity"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,15), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->SetIntPolarity((LogicState)val) ? "o " : "x " );
        }
        else if(cmd.substr(0,10) == "GetIntType" || cmd.substr(0,10) == "getinttype"){
            InterruptSignalType val;
            bool rtrn = accel->GetIntType(val);
            cout << "\t" << (val==0?"Latched":"Pulsed") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,10) == "SetIntType" || cmd.substr(0,10) == "setinttype"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,11), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->SetIntType((InterruptSignalType)val) ? "o " : "x " );
        }
        else if(cmd.substr(0,12) == "IsInterrupt1" || cmd.substr(0,12) == "isinterrupt1"){
            bool val;
            bool rtrn = accel->IsInterrupt1(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,13) == "UseInterrupt1" || cmd.substr(0,13) == "useinterrupt1"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,14), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->UseInterrupt1((val==0?false:true)) ? "o " : "x " );
        }
        else if(cmd.substr(0,12) == "IsInterrupt2" || cmd.substr(0,12) == "isinterrupt2"){
            bool val;
            bool rtrn = accel->IsInterrupt2(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,13) == "UseInterrupt2" || cmd.substr(0,13) == "useinterrupt2"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,14), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->UseInterrupt2((val==0?false:true)) ? "o " : "x " );
        }
        else if(cmd.substr(0,9) == "IsIntBoot" || cmd.substr(0,9) == "isintboot"){
            bool val;
            bool rtrn = accel->IsIntBoot(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,10) == "UseIntBoot" || cmd.substr(0,10) == "useintboot"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,11), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->UseIntBoot((val==0?false:true)) ? "o " : "x " );
        }
        else if(cmd.substr(0,14) == "IsIntFifoEmpty" || cmd.substr(0,14) == "isintfifoempty"){
            bool val;
            bool rtrn = accel->IsIntFifoEmpty(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,15) == "UseIntFifoEmpty" || cmd.substr(0,15) == "useintfifoempty"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,16), NULL);
            } catch (...) {
                cout << "exception";
                cout << "x ";
                continue;
            }

            cout << (accel->UseIntFifoEmpty((val==0?false:true)) ? "o " : "x " );
        }
        else if(cmd.substr(0,14) == "IsIntFifoWtmrk" || cmd.substr(0,14) == "isintfifowtmrk"){
            bool val;
            bool rtrn = accel->IsIntFifoWtmrk(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,15) == "UseIntFifoWtmrk" || cmd.substr(0,15) == "useintfifowtmrk"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,16), NULL);
            } catch (...) {
                cout << "x ";
                continue;
            }

            cout << (accel->UseIntFifoWtmrk((val==0?false:true)) ? "o " : "x " );
        }
        else if(cmd.substr(0,16) == "IsIntFifoOverrun" || cmd.substr(0,16) == "isintfifooverrun"){
            bool val;
            bool rtrn = accel->IsIntFifoOverrun(val);
            cout << "\t" << (val?"Yes":"No") << endl;
            cout << (rtrn ? "o " : "x " );
        }
        else if(cmd.substr(0,17) == "UseIntFifoOverrun" || cmd.substr(0,17) == "useintfifooverrun"){
            char val;

            try {
                val = (char)stoul(cmd.erase(0,18), NULL);
            } catch (...) {

                cout << "x ";
                continue;
            }


            cout << (accel->UseIntFifoOverrun((val==0?false:true)) ? "o " : "x " );
        }
        else if(cmd.substr(0,17) == "Loop" || cmd.substr(0,17) == "loop"){
            RawAcceleromterData val;
            loop = true;
            while (loop) {
                if (!accel->ReadSensorDataOnce(val)){
                    cout << "x ";
                    continue;
                }
                GForce * out = accel->ConvertToSIUnit(val);
                cout << "\t" << "X: " << out[0] << " g";
                cout << "\t" << "Y: "<< out[1] << " g";
                cout << "\t" << "Z: "<< out[2] << " g" << endl;
                delete[] out;
                usleep(5000);
            }

            cout << "o ";
        }
        else
            cout << "invalid command" << endl;
    }

    delete accel;
    return 0;
}

int main(){
    std::signal(SIGINT, signal_handler);

    int selection;
    cout << "---Skywalker Prototype---" << endl;
    cout << "Select what to test:" << endl;
    cout << "\t[0] spi bus" << endl;
    cout << "\t[1] test ST LIS3DSH (Accelerometer)" << endl;
    cout << "> ";
    cin >> selection;



    switch (selection) {
    case 0:
        return SpiTester();
    case 1:
        return AccelerometerTest();
    default:
        cout << "Invalid Option!"<< endl;
        return 1;
    }

}
