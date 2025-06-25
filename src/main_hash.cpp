#include <iostream>
#include "utilidades/ValidacionDatos.h"

using namespace std;

int main() {
    std::string clave = "italo_arre";
    std::string hash = PNTD::Utils::Validador::encriptarSHA256(clave);
    std::cout << "\n🔑 Hash generado para 'italo_arre': " << hash << std::endl;
    
    string clave02 = "clave123";
    string hash02 = PNTD::Utils::Validador::encriptarSHA256(clave02);
    cout << "\n🔑 Hash generado para 'clave123': " << hash02 << endl;
    return 0;
}
