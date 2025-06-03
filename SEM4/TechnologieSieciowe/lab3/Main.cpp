#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const string CRC32 = "00000100110000010001110110110111";
const string FLAG = "01111110";

string calculateCRC(const string& input) {
    string data = input + string(CRC32.length() - 1, '0');
    string remainder = data;

    for (size_t i = 0; i <= data.length() - CRC32.length(); ++i) {
        if (remainder[i] == '1') {
            for (size_t j = 0; j < CRC32.length(); ++j) {
                remainder[i + j] = (remainder[i + j] == CRC32[j]) ? '0' : '1';
            }
        }
    }
    return remainder.substr(remainder.length() - (CRC32.length() - 1));
}

string bitStuffing(const string& data) {
    string stuffed;
    int oneCount = 0;

    for (char bit : data) {
        stuffed += bit;
        if (bit == '1') {
            oneCount++;
            if (oneCount == 5) {
                stuffed += '0';
                oneCount = 0;
            }
        } else {
            oneCount = 0;
        }
    }

    return stuffed;
}

string bitUnstuffing(const string& data) {
    string unstuffed;
    int oneCount = 0;

    for (size_t i = 0; i < data.size(); ++i) {
        unstuffed += data[i];
        if (data[i] == '1') {
            oneCount++;
            if (oneCount == 5) {
                i++;
                oneCount = 0;
            }
        } else {
            oneCount = 0;
        }
    }

    return unstuffed;
}

void frame(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile);
    ofstream out(outputFile);

    string bits;
    getline(in, bits);
    int count = 0;

    for(int i=0; i<bits.size(); i+=100){
        std::string message = bits.substr(i, 100);
        string crc = calculateCRC(message);
        string full = message + crc;
        string stuffed = bitStuffing(full);
        string framed = FLAG + stuffed + FLAG;
        out << framed;
        count++;
    }
    
    std::cout<<count<<std::endl;
    in.close();
    out.close();
}

void deframe(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile);
    ofstream out(outputFile);
    int count = 0;

    string line;
    getline(in, line);

    int i=0;

    while(!line.empty()){
        size_t startFlagPos = line.find(FLAG);

        if (startFlagPos > 0) {
            std::cerr << "Ostrzeżenie: Znaleziono śmieci (" << startFlagPos << " bitów) przed flagą startową na pozycji " << startFlagPos << ". Pomijanie.\n";
            line = line.substr(startFlagPos); // Skip junk
            startFlagPos = 0; // The flag is now at the beginning of the new buffer
        }

        if (startFlagPos == string::npos) {
            cerr << "Brak kolejnej ramki lub nieprawidłowy format na końcu.\n";
            break;
        }

        size_t endFlagPos = line.find(FLAG, startFlagPos + FLAG.length());

        if (endFlagPos == string::npos) {
            cerr << "Brak końcowej flagi dla rozpoczętej ramki.\n";
            break;
        }

        string stuffed = line.substr(startFlagPos + FLAG.length(), endFlagPos - (startFlagPos + FLAG.length()));
        string full = bitUnstuffing(stuffed);

        size_t crcLen = CRC32.length() - 1;
        if (full.length() < crcLen) {
            line = line.substr(endFlagPos);
            cerr << "CRC ERROR: dane uszkodzone.\n";
            continue;;
        }
        string data = full.substr(0, full.size() - crcLen);
        string crc = full.substr(full.size() - crcLen);
        line = line.substr(endFlagPos);//przydaloby sie do endflagi zeby moc ewentualnie nie obcinac

        // |FLAG|data+crc|FLAG|FLAG|data+crc|FLAG
        
        if (calculateCRC(data) == crc) {
            out << data << endl;
            line=line.substr(FLAG.length());
            cout << "CRC OK: dane poprawne.\n";
            count++;
        } else {
            cerr << "CRC ERROR: dane uszkodzone.\n";
            size_t potentialEndFlag = line.find(FLAG, FLAG.length());
            if(potentialEndFlag==FLAG.length()){//wtedy znaczy ze data byla zjebana
                line=line.substr(FLAG.length());
            }
        }
        i++;
    }

    std::cout<<count<<std::endl;
    in.close();
    out.close();
}

int main() {
    cout << "Wybierz opcję:\n1. Ramkuj (Z → W)\n2. Odramkuj (W → Z)\n> ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        frame("Z.txt", "W.txt");
        cout << "Zapisano do W.txt\n";
    } else if (choice == 2) {
        deframe("W.txt", "Z_out.txt");
        cout << "Zapisano do Z_out.txt\n";
    } else {
        cerr << "Nieznana opcja.\n";
    }

    return 0;
}
