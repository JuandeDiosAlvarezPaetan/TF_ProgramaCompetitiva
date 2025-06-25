// ======================== src/main.cpp ========================
// PNTD Sistema - Archivo principal
// Implementa arquitectura modular con namespace según modelo de solución

#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
#include <conio.h>

// Incluir estructuras de datos compartidas
#include "modulos/GestionCursos.h"
#include "estructuras/Usuario.h"
#include "estructuras/Curso.h"
#include "estructuras/Certificado.h"

// Incluir módulos según arquitectura del modelo
#include "modulos/RegistroUsuarios.h"
#include "utilidades/ValidacionDatos.h"
#include "utilidades/PersistenciaCSV.h"
#include "modulos/GestorCertificados.h"

using namespace std;

// Variables globales para sesión
string usuarioActual = "";
string dniUsuarioActual = "";
bool sesionActiva = false;
PNTD::Certificados::GestorCertificados gestorCert;

// Declaraciones de funciones
void mostrarMenuPrincipal();
void mostrarMenuUsuario();
void procesarEmitirCertificado();
void procesarVerificarCertificado();
void procesarMisCertificados();
void procesarRegistro();
void procesarLogin();
void procesarBusquedaCursos();
void procesarVerificarCertificado();
void procesarMisCertificados();
void procesarMisInscripciones();
void procesarEstadisticas();
void limpiarPantalla();
void pausar();
void mostrarBanner();
void inicializarSistema();
void procesarEmitirCertificado();
void procesarMisInscripciones();
string leerPasswordConAsteriscos(const string& prompt);

int main() {
    cout << "🚀 Iniciando PNTD Sistema..." << endl;
    
    // Inicializar sistema según arquitectura del modelo
    inicializarSistema();
    
    // Banner de bienvenida
    mostrarBanner();

    
    
    int opcion = 0;
    bool salir = false;

    do {
            try {
                if (!sesionActiva) {
                    mostrarMenuPrincipal();
                } else {
                    mostrarMenuUsuario();
                }

                cout << "\nSeleccione una opcion: ";
                cin >> opcion;

                // Limpiar buffer de entrada
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (!sesionActiva) {
                    switch (opcion) {
                        case 1:
                            procesarRegistro();
                            break;
                        case 2:
                            procesarLogin();
                            break;
                        case 3:
                            procesarEmitirCertificado();
                            break;
                        case 4:
                            procesarVerificarCertificado();
                            break;
                        case 5:
                            cout << "⚠️  Inicia sesión para ver tus certificados.\n";
                            break;
                        case 9:
                            procesarEstadisticas();
                            break;
                        case 6:
                            cout << "\n🇵🇪 ¡Gracias por usar la PNTD!" << endl;
                            cout << "Cerrando sistema..." << endl;
                            salir = true;  // ⬅️ Marca que el sistema debe cerrarse
                            break;
                        default:
                            cout << "\n❌ Opcion no valida. Intente nuevamente." << endl;
                            break;
                    }
                } else {
                    switch (opcion) {
                        case 1:
                            procesarBusquedaCursos();
                            break;
                        case 2:
                            procesarMisInscripciones();
                            break;
                        case 3:
                            procesarEmitirCertificado();
                            break;
                        case 4:
                            procesarVerificarCertificado();
                            break;
                        case 5:
                            procesarMisCertificados();
                            break;
                        case 6:
                            sesionActiva = false;
                            usuarioActual = "";
                            dniUsuarioActual = "";
                            cout << "\n✅ Sesion cerrada exitosamente." << endl;
                            cout << "Volviendo al menu principal..." << endl;
                            break;  // ⬅️ No se sale del sistema, solo vuelve al menú principal
                        case 9:
                            procesarEstadisticas();
                            break;
                        default:
                            cout << "\n❌ Opcion no valida. Intente nuevamente." << endl;
                            break;
                    }
                }

                if (opcion != 4 && !(opcion == 2 && sesionActiva)) {
                    pausar();
                }

            } catch (const exception& e) {
                cout << "\n❌ Error del sistema: " << e.what() << endl;
                pausar();
            }

        } while (!salir);
        
        return 0;
    }

void inicializarSistema() {
    cout << "🔧 Inicializando arquitectura modular..." << endl;
    
    // Verificar y crear directorios si es necesario
    PNTD::Persistencia::CSV::crearDirectorios();
    
    // Inicializar módulo de Registro (crítico para otros módulos)
    PNTD::Registro::inicializarModulo();
    
    // TODO: Inicializar otros módulos cuando estén implementados
    PNTD::Cursos::inicializarModulo();
    // PNTD::Certificados::inicializarModulo();
    
    cout << "✅ Sistema PNTD inicializado correctamente" << endl;
}

void mostrarBanner() {
    cout << "\n";
    cout << "===============================================" << endl;
    cout << "    PLATAFORMA NACIONAL DE TALENTO DIGITAL    " << endl;
    cout << "===============================================" << endl;
    cout << "         🇵🇪 Gobierno del Peru 🇵🇪            " << endl;
    cout << "    Sistema de Gestion de Capacitaciones      " << endl;
    cout << "      📊 Algoritmos Optimizados 📊           " << endl;
    cout << "===============================================" << endl;
}

void mostrarMenuPrincipal() {
    limpiarPantalla();
    mostrarBanner();
    cout << "\n";
    cout << "============= MENU PRINCIPAL =============" << endl;
    cout << "1. 📝 Registrarse como nuevo usuario" << endl;
    cout << "2. 🔐 Iniciar Sesion" << endl;
    cout << "3. 🏆 Emitir Certificado" << endl;       // ⟵ NUEVO
    cout << "4. 🎓 Verificar Certificado" << endl;
    cout << "5. 📜 Ver mis Certificados"<< endl;
    cout << "6. ❌ Salir del sistema" << endl;
    cout << "9. 📊 Ver estadisticas del sistema" << endl;
    cout << "==========================================" << endl;
}

void mostrarMenuUsuario() {
    limpiarPantalla();
    mostrarBanner();
    cout << "\n======== BIENVENIDO " << usuarioActual << " ========" << endl;
    cout << "1. 🔍 Buscar y inscribirse a cursos" << endl;
    cout << "2. 📋 Ver mis inscripciones" << endl;
    cout << "3. 🏆 Emitir Certificado" << endl;
    cout << "4. 🎓 Verificar Certificado" << endl;
    //cout << "5. 📚 Ver historial academico" << endl;
    cout << "5. 📜 Ver mis Certificados" << endl;
    cout << "6. 🚪 Cerrar sesion" << endl;
    cout << "9. 📊 Ver estadisticas del sistema" << endl;
    cout << "============================================" << endl;
}

void procesarRegistro() {
    cout << "\n=== REGISTRO DE NUEVO USUARIO ===" << endl;
    cout << "🔐 Usando algoritmos de validación específicos para Perú" << endl;
    
    Usuario nuevoUsuario;
    
    cout << "\n📝 Ingrese sus datos:" << endl;
    
    // Recopilar datos con validación en tiempo real
    do {
        cout << "DNI (8 dígitos peruanos): ";
        getline(cin, nuevoUsuario.dni);
        
        if (!PNTD::Utils::Validador::validarDNI(nuevoUsuario.dni)) {
            cout << "❌ DNI inválido. Debe ser exactamente 8 dígitos." << endl;
        } else {
            // Verificar si ya existe usando Hash Table O(1)
            if (PNTD::Registro::buscarUsuario(nuevoUsuario.dni) != nullptr) {
                cout << "❌ Ya existe un usuario con ese DNI." << endl;
                nuevoUsuario.dni = "";
            }
        }
    } while (nuevoUsuario.dni.empty());
    
    cout << "Nombres: ";
    getline(cin, nuevoUsuario.nombre);
    
    cout << "Apellidos: ";
    getline(cin, nuevoUsuario.apellidos);
    
    do {
        cout << "Email: ";
        getline(cin, nuevoUsuario.email);
        
        if (!PNTD::Utils::Validador::validarEmail(nuevoUsuario.email)) {
            cout << "❌ Email inválido. Ingrese un email válido." << endl;
            nuevoUsuario.email = "";
        }
    } while (nuevoUsuario.email.empty());
    
    do {
        cout << "Teléfono (9 dígitos): ";
        getline(cin, nuevoUsuario.telefono);
        
        if (!PNTD::Utils::Validador::validarTelefono(nuevoUsuario.telefono)) {
            cout << "❌ Teléfono inválido. Debe ser 9 dígitos." << endl;
            nuevoUsuario.telefono = "";
        }
    } while (nuevoUsuario.telefono.empty());
    
    do {
        cout << "Contraseña (mínimo 8 caracteres): ";
        getline(cin, nuevoUsuario.passwordHash);
        
        if (!PNTD::Utils::Validador::validarPassword(nuevoUsuario.passwordHash)) {
            cout << "❌ Contraseña debe tener mínimo 8 caracteres." << endl;
            nuevoUsuario.passwordHash = "";
        }
    } while (nuevoUsuario.passwordHash.empty());
    
    // Registrar usando módulo con algoritmos optimizados
    cout << "\n🔄 Procesando registro con algoritmos del modelo..." << endl;
    if (PNTD::Registro::registrarUsuario(nuevoUsuario)) {
        cout << "\n🎉 ¡REGISTRO COMPLETADO EXITOSAMENTE!" << endl;
        cout << "✅ Usuario agregado a Hash Table O(1)" << endl;
        cout << "✅ Usuario agregado a Vector ordenado para búsqueda binaria" << endl;
        cout << "✅ Contraseña encriptada con SHA-256" << endl;
        cout << "✅ Datos persistidos en CSV" << endl;
        cout << "\n🔐 Ya puede iniciar sesión con su DNI: " << nuevoUsuario.dni << endl;
    } else {
        cout << "\n❌ Error en el registro. Verifique los datos ingresados." << endl;
    }
}

void procesarLogin() {
    cout << "\n=== INICIO DE SESIÓN ===" << endl;
    cout << "🔍 Usando búsqueda O(1) con Hash Table" << endl;
    
    string dni, password;
    
    cout << "\nDNI: ";
    getline(cin, dni);
    
    // cout << "Contraseña: ";
    // getline(cin, password);
    password = leerPasswordConAsteriscos("Contraseña: ");

    cout << "\n🔄 Autenticando usuario..." << endl;

    
    if (PNTD::Registro::autenticarUsuario(dni, password)) {
        Usuario* user = PNTD::Registro::buscarUsuario(dni);
        if (user && user->activo) {
            usuarioActual = user->nombre + " " + user->apellidos;
            dniUsuarioActual = dni;
            sesionActiva = true;
            
            cout << "\n🎉 ¡BIENVENIDO AL SISTEMA PNTD!" << endl;
            cout << "👤 Usuario: " << usuarioActual << endl;
            cout << "📧 Email: " << user->email << endl;
            cout << "📱 Teléfono: " << user->telefono << endl;
            cout << "📅 Registrado: " << user->fechaRegistro << endl;

            cout << "\n⏸️  Presione Enter para continuar...";
            cin.get(); 
            
        } else {
            cout << "❌ Usuario inactivo" << endl;
        }
    } else {
        cout << "❌ Credenciales incorrectas o usuario no encontrado" << endl;
    }
}

void procesarBusquedaCursos() {
    cout << "\n=== BUSQUEDA Y GESTION DE CURSOS ===" << endl;
    PNTD::Cursos::CriteriosBusqueda criterios;

    cout << "\nFiltrar por categoría (o presione Enter para omitir): ";
    getline(cin, criterios.categoria);

    cout << "Filtrar por modalidad (o presione Enter para omitir): ";
    getline(cin, criterios.modalidad);

    vector<Curso> resultados = PNTD::Cursos::buscarCursos(criterios);

    if (resultados.empty()) {
        cout << "\n❌ No se encontraron cursos con esos filtros.\n";
        return;
    }

    cout << "\n🎯 Resultados encontrados:\n";
    for (const auto& c : resultados) {
        cout << "ID: " << c.id << " | " << c.nombre
             << " | " << c.categoria << " | " << c.modalidad
             << " | Cupos: " << c.cuposDisponibles << "/" << c.cuposTotal
             << (c.activo ? " | Activo\n" : " | Inactivo\n");
    }

    string opcion;
    cout << "\n¿Desea inscribirse a un curso? (s/n): ";
    getline(cin, opcion);

    if (opcion == "s" || opcion == "S") {
        int idCurso;
        cout << "Ingrese el ID del curso: ";
        cin >> idCurso;
        cin.ignore();

        if (!PNTD::Cursos::inscribirUsuario(dniUsuarioActual, idCurso)) {
            cout << "⚠ No se pudo completar la inscripción.\n";
      }
   }
}

void procesarVerificarCertificado() {
    system("cls");
    cout << "\n=== VERIFICAR CERTIFICADO ===\n";

    string hash;
    cout << "Pegue o escriba el hash: ";
    cin  >> hash;

    bool ok = gestorCert.verificarCertificado(hash);
    cout << (ok ? "✅  Certificado válido y vigente.\n"
                : "❌  Hash inexistente, revocado o alterado.\n");
    system("pause");
}

void procesarMisCertificados() {
    if (!sesionActiva) {
        cout << "⚠️  Debes iniciar sesión.\n";
        return;
    }

    system("cls");
    cout << "\n=== MIS CERTIFICADOS ===\n";

    auto lista = gestorCert.listarPorUsuario(dniUsuarioActual);
    if (lista.empty()) {
        cout << "Aún no tienes certificados.\n";
    } else {
        for (const auto& c : lista) {
            cout << (c.valido ? "🟢" : "🔴") << ' '                 // vigente / revocado
                 << c.hash.substr(0,8) << "…  "                    // primeros 8 chars
                << c.nombreCurso << "  "
                << c.fechaEmision << '\n';
        }
    }
}


void procesarEstadisticas() {
    cout << "\n=== ESTADÍSTICAS DEL SISTEMA PNTD ===" << endl;
    cout << "📊 Análisis de rendimiento algorítmico" << endl;
    
    // Mostrar estadísticas del módulo de Registro
    PNTD::Registro::GestorUsuarios gestorTemp;
    gestorTemp.inicializar();
    gestorTemp.mostrarEstadisticas();
    
    cout << "\n🏗️ ARQUITECTURA IMPLEMENTADA:" << endl;
    cout << "✅ Módulo Registro: Hash Tables O(1) + Búsqueda Binaria O(log n)" << endl;
    cout << "⏳ Módulo Cursos: Priority Queues + Búsqueda Multidimensional" << endl;
    cout << "⏳ Módulo Certificados: Merkle Trees + Listas Enlazadas" << endl;
    cout << "✅ Persistencia CSV: Simulación de base de datos" << endl;
    
    cout << "\n🔧 TECNOLOGÍAS:" << endl;
    cout << "• Lenguaje: C++17" << endl;
    cout << "• Compilador: MinGW/g++" << endl;
    cout << "• Estructuras: STL (unordered_map, vector, list, queue)" << endl;
    cout << "• Persistencia: Archivos CSV estructurados" << endl;
    cout << "\n🔐 Raíz Merkle actual (integridad del historial):\n";
    cout << gestorCert.getMerkleRoot() << endl;
}
void procesarEmitirCertificado() {
    system("cls");
    std::cout << "\n=== EMITIR CERTIFICADO ===\n";

    std::string dni;
    int idCurso;

    std::cout << "DNI del usuario : ";
    std::cin >> dni;
    std::cout << "ID del curso    : ";
    std::cin >> idCurso;

    // 🔒 Validaciones ANTES de emitir el certificado
    Usuario* usuario = PNTD::Registro::buscarUsuario(dni);
    if (!usuario || !usuario->activo) {
        std::cout << "❌ El usuario no existe o está inactivo.\n";
        system("pause");
        return;
    }

    if (!PNTD::Cursos::verificarInscripcionActiva(dni, idCurso)) {
        std::cout << "❌ El usuario no está inscrito activamente en ese curso.\n";
        system("pause");
        return;
    }

    // ✅ Emitir certificado si todo está bien
    std::string hash = gestorCert.generarCertificado(dni, idCurso);
    if (!hash.empty()) {
        std::cout << "\n🏆  Hash generado: " << hash << '\n';
    } else {
        std::cout << "\n❌  No se pudo emitir el certificado.\n";
    }

    system("pause");
}

void procesarMisInscripciones() {
    cout << "\n=== MIS INSCRIPCIONES ACTIVAS ===\n";
    cout << "👤 Usuario: " << usuarioActual << " (DNI: " << dniUsuarioActual << ")\n";

    auto inscripciones = PNTD::Cursos::obtenerMisInscripciones(dniUsuarioActual);
    cout << "🔎 Total encontradas: " << inscripciones.size() << "\n";

    if (inscripciones.empty()) {
        system("cls");
        cout << "\n📭 No tienes inscripciones activas.\n";
        pausar();
        return;
    }
    cout << "\n📋 Lista de inscripciones:\n";
    for (const auto& ins : inscripciones) {
        cout << "📘 Curso ID: " << ins.idCurso
             << " | Estado: " << ins.estado
             << " | Prioridad: " << ins.prioridad << "\n";
    }

    pausar();
}

void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pausar() {
    cout << "\n⏸️  Presione Enter para continuar...";
    cin.get();
}

string leerPasswordConAsteriscos(const string& prompt) {
    cout << prompt;
    string password;
    char ch;
    while ((ch = _getch()) != '\r') { // '\r' es Enter
        if (ch == '\b') { // Backspace
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        } else if (isprint(ch)) {
            password += ch;
            cout << '*';
        }
    }
    cout << endl;
    return password;
}