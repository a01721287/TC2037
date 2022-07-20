#include <regex>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
    vector<string> filenames(argv, argv+argc);
    for (int i = 1; i < argc; i++){
        string tmp = filenames[i];
        tmp = tmp.replace(0,filenames[i].find('/')+1,"");
        tmp = tmp.replace((tmp.size()-4),4,"");
        filenames[i] = tmp;
    }

    for (int i = 1; i < argc; i++){
        fstream arch;
        arch.open(argv[i], ios::in);
        string code = "";
        string line;
        do{
            getline(arch,line);
            code.append(line + "<br>");
        } while (!arch.eof());
        arch.close();
        cout<<code<<endl;

        fstream arch2;
        arch2.open("output_files/" + filenames[i] + ".html", ios::out);
        string boilerplate = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n\t<meta charset=\"UTF-8\">\n\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n<title>"+ filenames[i] +"</title>\n<link rel=\"stylesheet\" href=\"styles.css\">\n</head>\n<body>\n";
        cout<<boilerplate<<endl;
        arch2<<boilerplate + code + "\n</body>\n</html>";
        arch2.close();
    }
}
