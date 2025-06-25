#ifndef VALIDACION_DATOS_H
#define VALIDACION_DATOS_H

#include <string>
using namespace std;

namespace PNTD {
namespace Utils {

class Validador {
public:
    static bool validarDNI(const string& dni);
    static bool validarRUC(const string& ruc);
    static bool validarEmail(const string& email);
    static bool validarTelefono(const string& telefono);
    static bool validarPassword(const string& password);
    static string encriptarSHA256(const string& input);
    static string generarTimestamp();
    static bool esNumerico(const string& str);
    static string limpiarString(const string& str);
};

} // namespace Utils
} // namespace PNTD

#endif