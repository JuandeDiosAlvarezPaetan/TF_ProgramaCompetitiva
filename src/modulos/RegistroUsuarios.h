#ifndef REGISTRO_USUARIOS_H
#define REGISTRO_USUARIOS_H

#include <unordered_map>
#include <vector>
#include <string>
#include "../estructuras/Usuario.h"

using namespace std;

namespace PNTD {
namespace Registro {

class GestorUsuarios {
private:
    unordered_map<string, Usuario> tablaUsuarios; // Hash table para O(1)
    vector<Usuario> usuariosOrdenados; // Para búsquedas binarias
    
public:
    bool registrarUsuario(const Usuario& usuario);
    bool autenticarUsuario(const string& dni, const string& password);
    Usuario* buscarUsuario(const string& dni);
    Usuario* buscarUsuarioBinario(const string& dni); // Búsqueda binaria específica
    bool validarDNI(const string& dni);
    bool validarEmail(const string& email);
    string encriptarPassword(const string& password);
    bool cargarUsuarios();
    bool guardarUsuarios();
    void mostrarEstadisticas();
    void inicializar(); // Cargar datos al inicio
};

// Funciones públicas del namespace (para otros módulos)
extern bool registrarUsuario(const Usuario& usuario);
extern Usuario* buscarUsuario(const string& dni);
extern bool autenticarUsuario(const string& dni, const string& password);
extern void inicializarModulo();

} // namespace Registro
} // namespace PNTD

#endif
