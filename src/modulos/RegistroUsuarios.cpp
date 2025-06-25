#include "RegistroUsuarios.h"
#include "../utilidades/ValidacionDatos.h"
#include "../utilidades/PersistenciaCSV.h"
#include <iostream>
#include <algorithm>

namespace PNTD {
namespace Registro {

// Instancia global del gestor
GestorUsuarios gestorGlobal;

bool GestorUsuarios::registrarUsuario(const Usuario& usuario) {
    cout << "\n🔍 Iniciando proceso de registro..." << endl;
    
    // 1. VALIDACIONES usando algoritmos específicos del modelo
    if (!PNTD::Utils::Validador::validarDNI(usuario.dni)) {
        cout << "❌ DNI inválido (debe ser 8 dígitos peruanos)" << endl;
        return false;
    }
    
    if (!PNTD::Utils::Validador::validarEmail(usuario.email)) {
        cout << "❌ Email inválido" << endl;
        return false;
    }
    
    if (!PNTD::Utils::Validador::validarTelefono(usuario.telefono)) {
        cout << "❌ Teléfono inválido (debe ser 9 dígitos)" << endl;
        return false;
    }
    
    if (!PNTD::Utils::Validador::validarPassword(usuario.passwordHash)) {
        cout << "❌ Contraseña debe tener mínimo 8 caracteres" << endl;
        return false;
    }
    
    // 2. VERIFICACIÓN con HASH TABLE O(1) - según modelo
    cout << "🔍 Verificando usuario existente (Hash Table O(1))..." << endl;
    if (tablaUsuarios.find(usuario.dni) != tablaUsuarios.end()) {
        cout << "❌ Usuario con DNI " << usuario.dni << " ya existe" << endl;
        return false;
    }
    
    // 3. CREAR USUARIO con encriptación según modelo
    Usuario nuevoUsuario = usuario;
    nuevoUsuario.passwordHash = PNTD::Utils::Validador::encriptarSHA256(usuario.passwordHash);
    nuevoUsuario.fechaRegistro = PNTD::Utils::Validador::generarTimestamp();
    nuevoUsuario.activo = true;
    
    cout << "🔐 Contraseña encriptada con SHA-256 simulado" << endl;
    
    // 4. AGREGAR A HASH TABLE - O(1) según modelo
    tablaUsuarios[nuevoUsuario.dni] = nuevoUsuario;
    cout << "✅ Usuario agregado a Hash Table" << endl;
    
    // 5. AGREGAR A VECTOR ORDENADO para búsqueda binaria - según modelo
    usuariosOrdenados.push_back(nuevoUsuario);
    
    // ALGORITMO DE ORDENACIÓN - mantener vector ordenado para búsqueda binaria
    sort(usuariosOrdenados.begin(), usuariosOrdenados.end(),
         [](const Usuario& a, const Usuario& b) { return a.dni < b.dni; });
    
    cout << "✅ Usuario agregado a vector ordenado (búsqueda binaria)" << endl;
    
    // 6. PERSISTENCIA según modelo
    vector<Usuario> todosUsuarios;
    for (const auto& pair : tablaUsuarios) {
        todosUsuarios.push_back(pair.second);
    }
    
    if (PNTD::Persistencia::CSV::guardarUsuarios(todosUsuarios)) {
        cout << "✅ Usuario persistido en CSV" << endl;
    }
    
    cout << "🎉 Usuario registrado exitosamente en el sistema PNTD" << endl;
    return true;
}

bool GestorUsuarios::autenticarUsuario(const string& dni, const string& password) {
    cout << "\n🔐 Iniciando autenticación..." << endl;
    
    // BÚSQUEDA O(1) con HASH TABLE - según modelo de solución
    cout << "🔍 Buscando usuario en Hash Table O(1)..." << endl;
    auto it = tablaUsuarios.find(dni);
    if (it == tablaUsuarios.end()) {
        cout << "❌ Usuario con DNI " << dni << " no encontrado" << endl;
        return false;
    }
    
    cout << "✅ Usuario encontrado en Hash Table" << endl;
    
    // VERIFICACIÓN DE CONTRASEÑA con encriptación
    string passwordHash = PNTD::Utils::Validador::encriptarSHA256(password);
    if (it->second.passwordHash != passwordHash) {
        cout << "❌ Contraseña incorrecta" << endl;
        return false;
    }
    
    // Verificar que esté activo
    if (!it->second.activo) {
        cout << "❌ Usuario inactivo" << endl;
        return false;
    }
    
    cout << "✅ Autenticación exitosa para " << it->second.nombre << " " << it->second.apellidos << endl;
    return true;
}

Usuario* GestorUsuarios::buscarUsuario(const string& dni) {
    // BÚSQUEDA O(1) con HASH TABLE - algoritmo principal del modelo
    auto it = tablaUsuarios.find(dni);
    if (it != tablaUsuarios.end()) {
        return &(it->second);
    }
    return nullptr;
}

Usuario* GestorUsuarios::buscarUsuarioBinario(const string& dni) {
    // BÚSQUEDA BINARIA O(log n) - algoritmo alternativo del modelo
    Usuario temp;
    temp.dni = dni;
    
    auto it = lower_bound(usuariosOrdenados.begin(), usuariosOrdenados.end(), temp,
                         [](const Usuario& a, const Usuario& b) { return a.dni < b.dni; });
    
    if (it != usuariosOrdenados.end() && it->dni == dni) {
        return &(*it);
    }
    return nullptr;
}

void GestorUsuarios::inicializar() {
    cout << "\n🚀 Inicializando módulo de Registro..." << endl;
    
    // Cargar usuarios existentes del CSV
    vector<Usuario> usuariosCargados = PNTD::Persistencia::CSV::cargarUsuarios();
    
    // Poblar estructuras de datos según modelo
    tablaUsuarios.clear();
    usuariosOrdenados.clear();
    
    for (const auto& user : usuariosCargados) {
        // Hash table para O(1)
        tablaUsuarios[user.dni] = user;
        // Vector para búsqueda binaria
        usuariosOrdenados.push_back(user);
    }
    
    // Ordenar vector para búsqueda binaria
    sort(usuariosOrdenados.begin(), usuariosOrdenados.end(),
         [](const Usuario& a, const Usuario& b) { return a.dni < b.dni; });
    
    cout << "✅ Hash table poblada: " << tablaUsuarios.size() << " usuarios" << endl;
    cout << "✅ Vector ordenado: " << usuariosOrdenados.size() << " usuarios" << endl;
    cout << "✅ Módulo de Registro inicializado correctamente" << endl;
}

void GestorUsuarios::mostrarEstadisticas() {
    cout << "\n📊 ESTADÍSTICAS DEL MÓDULO REGISTRO:" << endl;
    cout << "• Usuarios en Hash Table: " << tablaUsuarios.size() << endl;
    cout << "• Usuarios en Vector Ordenado: " << usuariosOrdenados.size() << endl;
    cout << "• Algoritmos implementados: Hash O(1), Búsqueda Binaria O(log n)" << endl;
}

// FUNCIONES PÚBLICAS DEL NAMESPACE (para otros módulos)
bool registrarUsuario(const Usuario& usuario) {
    return gestorGlobal.registrarUsuario(usuario);
}

Usuario* buscarUsuario(const string& dni) {
    return gestorGlobal.buscarUsuario(dni);
}

bool autenticarUsuario(const string& dni, const string& password) {
    return gestorGlobal.autenticarUsuario(dni, password);
}

void inicializarModulo() {
    gestorGlobal.inicializar();
}

} // namespace Registro
} // namespace PNTD