// ======================== src/utilidades/ValidacionDatos.cpp ========================
#include "ValidacionDatos.h"
#include "picosha2.h"
#include <regex>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace PNTD {
namespace Utils {

bool Validador::validarDNI(const string& dni) {
    // Algoritmo específico para DNI peruano según RENIEC
    if (dni.length() != 8) return false;
    
    // Solo números (puede empezar con 0)
    for (char c : dni) {
        if (!isdigit(c)) return false;
    }
    
    // Validaciones adicionales: no todos iguales
    bool todosIguales = true;
    for (size_t i = 1; i < dni.length(); i++) {
        if (dni[i] != dni[0]) {
            todosIguales = false;
            break;
        }
    }
    
    return !todosIguales; // Rechazar 00000000, 11111111, etc.
}

bool Validador::validarRUC(const string& ruc) {
    // RUC peruano: 11 dígitos según SUNAT
    if (ruc.length() != 11) return false;
    
    // Solo números
    for (char c : ruc) {
        if (!isdigit(c)) return false;
    }
    
    // Debe empezar con 10, 15, 16, 17 o 20
    string prefijo = ruc.substr(0, 2);
    return (prefijo == "10" || prefijo == "15" || prefijo == "16" || 
            prefijo == "17" || prefijo == "20");
}

bool Validador::validarEmail(const string& email) {
    // Regex optimizado para email gubernamental
    regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return regex_match(email, emailRegex);
}

bool Validador::validarTelefono(const string& telefono) {
    // Teléfono peruano: 9 dígitos, puede empezar con 9
    if (telefono.length() != 9) return false;
    
    for (char c : telefono) {
        if (!isdigit(c)) return false;
    }
    
    return true;
}

bool Validador::validarPassword(const string& password) {
    // Mínimo 8 caracteres para seguridad gubernamental
    return password.length() >= 8;
}

string Validador::encriptarSHA256(const string& input) {
    string salt = "PNTD_2025_SALT";
    string combined = salt + input;

    string hash_hex_str;
    picosha2::hash256_hex_string(combined, hash_hex_str);
    return hash_hex_str;
}

string Validador::generarTimestamp() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    stringstream ss;
    ss << (1900 + ltm->tm_year) << "-"
       << setfill('0') << setw(2) << (1 + ltm->tm_mon) << "-"
       << setfill('0') << setw(2) << ltm->tm_mday << "_"
       << setfill('0') << setw(2) << ltm->tm_hour << ":"
       << setfill('0') << setw(2) << ltm->tm_min << ":"
       << setfill('0') << setw(2) << ltm->tm_sec;
    
    return ss.str();
}

bool Validador::esNumerico(const string& str) {
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}

string Validador::limpiarString(const string& str) {
    string resultado = str;
    // Eliminar espacios al inicio y final
    resultado.erase(0, resultado.find_first_not_of(" \t\n\r"));
    resultado.erase(resultado.find_last_not_of(" \t\n\r") + 1);
    return resultado;
}

} // namespace Utils
} // namespace PNTD