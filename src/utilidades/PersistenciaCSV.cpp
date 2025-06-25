#include "PersistenciaCSV.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <sys/stat.h>

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::cout;
using std::getline;

namespace PNTD {
namespace Persistencia {

// ---------------------------------------------------------------------
// Utilidades genéricas
// ---------------------------------------------------------------------
vector<string> CSV::parsearLinea(const string& linea) {
    vector<string> campos;
    stringstream ss(linea);
    string campo;

    while (getline(ss, campo, ',')) {
        // Trim
        campo.erase(0, campo.find_first_not_of(" \t"));
        campo.erase(campo.find_last_not_of(" \t") + 1);
        campos.push_back(campo);
    }
    return campos;
}

string CSV::escaparCSV(const string& campo) {
    string r = campo;
    if (r.find(',') != string::npos || r.find('"') != string::npos || r.find('\n') != string::npos) {
        size_t pos = 0;
        while ((pos = r.find('"', pos)) != string::npos) {
            r.replace(pos, 1, "\"\"");
            pos += 2;
        }
        r = "\"" + r + "\"";
    }
    return r;
}

// Crea carpeta data/ si falta
bool CSV::crearDirectorios() {
#ifdef _WIN32
    system("if not exist data mkdir data");
#else
    system("mkdir -p data");
#endif
    return true;
}

// Chequeo rápido de existencia de CSV
bool CSV::verificarArchivos() {
    return ifstream("data/usuarios.csv").good() &&
           ifstream("data/cursos.csv").good()   &&
           ifstream("data/certificados.csv").good() &&
           ifstream("data/inscripciones.csv").good();
}

// ---------------------------------------------------------------------
// USUARIOS (8 columnas)
// ---------------------------------------------------------------------
vector<Usuario> CSV::cargarUsuarios() {
    vector<Usuario> v;
    ifstream f("data/usuarios.csv");
    if (!f.is_open()) { cout << "❌ No se pudo abrir usuarios.csv\n"; return v; }

    string line; getline(f, line);                    // cabecera
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto c = parsearLinea(line);
        if (c.size() >= 8) {
            Usuario u;
            u.dni           = c[0];
            u.nombre        = c[1];
            u.apellidos     = c[2];
            u.email         = c[3];
            u.passwordHash  = c[4];
            u.telefono      = c[5];
            u.fechaRegistro = c[6];
            u.activo        = (c[7] == "1");
            v.push_back(u);
        }
    }
    cout << "✅ Cargados " << v.size() << " usuarios\n";
    return v;
}

bool CSV::guardarUsuarios(const vector<Usuario>& v) {
    ofstream f("data/usuarios.csv", std::ios::trunc);
    if (!f.is_open()) { cout << "❌ No se pudo crear usuarios.csv\n"; return false; }

    f << "DNI,Nombre,Apellidos,Email,PasswordHash,Telefono,FechaRegistro,Activo\n";
    for (const auto& u : v) {
        f << escaparCSV(u.dni)           << ','
          << escaparCSV(u.nombre)        << ','
          << escaparCSV(u.apellidos)     << ','
          << escaparCSV(u.email)         << ','
          << escaparCSV(u.passwordHash)  << ','
          << escaparCSV(u.telefono)      << ','
          << escaparCSV(u.fechaRegistro) << ','
          << (u.activo ? "1" : "0")      << '\n';
    }
    cout << "✅ Guardados " << v.size() << " usuarios\n";
    return true;
}

// ---------------------------------------------------------------------
// CURSOS (10 columnas)
// Id,Nombre,Categoria,Modalidad,Descripcion,CuposDisp,CuposTot,FechaInicio,FechaFin,Activo
// ---------------------------------------------------------------------


bool CSV::guardarCursos(const vector<Curso>& v) {
    ofstream f("data/cursos.csv", std::ios::trunc);
    if (!f.is_open()) { cout << "❌ No se pudo crear cursos.csv\n"; return false; }

    f << "Id,Nombre,Categoria,Modalidad,Descripcion,CuposDisp,CuposTot,FechaInicio,FechaFin,Activo\n";
    for (const auto& cu : v) {
        f << cu.id                       << ','
          << escaparCSV(cu.nombre)       << ','
          << escaparCSV(cu.categoria)    << ','
          << escaparCSV(cu.modalidad)    << ','
          << escaparCSV(cu.descripcion)  << ','
          << cu.cuposDisponibles         << ','
          << cu.cuposTotal               << ','
          << escaparCSV(cu.fechaInicio)  << ','
          << escaparCSV(cu.fechaFin)     << ','
          << (cu.activo ? "1" : "0")     << '\n';
    }
    cout << "✅ Guardados " << v.size() << " cursos\n";
    return true;
}

// ---------------------------------------------------------------------
// CERTIFICADOS (8 columnas)
// Hash,DNI,IdCurso,NombreCurso,NombreUsuario,FechaEmision,FechaExpiracion,Valido
// ---------------------------------------------------------------------
vector<Certificado> CSV::cargarCertificados() {
    vector<Certificado> v;
    ifstream f("data/certificados.csv");
    if (!f.is_open()) { cout << "❌ No se pudo abrir certificados.csv\n"; return v; }

    string line; getline(f, line);
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto c = parsearLinea(line);
        if (c.size() >= 8) {
            Certificado ce;
            ce.hash            = c[0];
            ce.dniUsuario      = c[1];
            ce.idCurso         = stoi(c[2]);
            ce.nombreCurso     = c[3];
            ce.nombreUsuario   = c[4];
            ce.fechaEmision    = c[5];
            ce.fechaExpiracion = c[6];
            ce.valido          = (c[7] == "1");
            v.push_back(ce);
        }
    }
    cout << "✅ Cargados " << v.size() << " certificados\n";
    return v;
}

bool CSV::guardarCertificados(const vector<Certificado>& v) {
    ofstream f("data/certificados.csv", std::ios::trunc);
    if (!f.is_open()) { cout << "❌ No se pudo crear certificados.csv\n"; return false; }

    f << "Hash,DNI,IdCurso,NombreCurso,NombreUsuario,FechaEmision,FechaExpiracion,Valido\n";
    for (const auto& c : v) {
        f << c.hash                       << ','
          << escaparCSV(c.dniUsuario)     << ','
          << c.idCurso                    << ','
          << escaparCSV(c.nombreCurso)    << ','
          << escaparCSV(c.nombreUsuario)  << ','
          << escaparCSV(c.fechaEmision)   << ','
          << escaparCSV(c.fechaExpiracion)<< ','
          << (c.valido ? "1" : "0")       << '\n';
    }
    cout << "✅ Guardados " << v.size() << " certificados\n";
    return true;
}

// ---------------------------------------------------------------------
// INSCRIPCIONES (5 columnas)
// DNI,IdCurso,FechaInscripcion,Estado,Prioridad
// ---------------------------------------------------------------------
vector<Inscripcion> CSV::cargarInscripciones() {
    vector<Inscripcion> v;
    ifstream f("data/inscripciones.csv");
    if (!f.is_open()) { cout << "❌ No se pudo abrir inscripciones.csv\n"; return v; }

    string line; getline(f, line);
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto c = parsearLinea(line);
            if (c.size() < 5) {
                std::cerr << "⚠️ Línea inválida (menos de 5 campos): " << line << "\n";
                continue;
            }
            try {
                string dni = c[0];                  // DNIUsuario
                int idCurso = std::stoi(c[1]);      // IDCurso
                string fecha = c[2];                // FechaInscripcion
                string estado = c[3];               // Estado
                int prioridad = std::stoi(c[4]);    // Prioridad

                Inscripcion i(dni, idCurso, prioridad);
                i.fechaInscripcion = fecha;
                i.estado = estado;
                v.push_back(i);
                cout << "📌 Inscripción cargada: " << i.dniUsuario 
                        << " | Curso ID: " << i.idCurso 
                        << " | Fecha: " << i.fechaInscripcion 
                        << " | Estado: " << i.estado 
                        << " | Prioridad: " << i.prioridad << '\n';
            } catch (const std::exception& e) {
                std::cerr << "❌ Error al convertir datos numéricos en línea: " << line << "\n";
                std::cerr << "   → Motivo: " << e.what() << "\n";
                continue;
            }
    }
    cout << "✅ Cargadas " << v.size() << " inscripciones\n";
    return v;
}

bool CSV::guardarInscripciones(const vector<Inscripcion>& v) {
    ofstream f("data/inscripciones.csv", std::ios::trunc);
    if (!f.is_open()) { cout << "❌ No se pudo crear inscripciones.csv\n"; return false; }

    f << "DNI,IdCurso,FechaInscripcion,Estado,Prioridad\n";
    for (const auto& i : v) {
        f << i.dniUsuario                  << ','
          << i.idCurso                     << ','
          << escaparCSV(i.fechaInscripcion)<< ','
          << escaparCSV(i.estado)          << ','
          << i.prioridad                   << '\n';
    }
    cout << "✅ Guardadas " << v.size() << " inscripciones\n";
    return true;
}

vector<Curso> CSV::cargarCursos() {
    vector<Curso> cursos;
    ifstream archivo("data/cursos.csv");

    if (!archivo.is_open()) {
        cout << "❌ No se pudo abrir cursos.csv\n";
        return cursos;
    }

    string linea;
    getline(archivo, linea); // Cabecera

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        vector<string> campos = parsearLinea(linea);

        if (campos.size() >= 10) {
            Curso curso;
            curso.id = stoi(campos[0]);
            curso.nombre = campos[1];
            curso.categoria = campos[2];
            curso.modalidad = campos[3];
            curso.descripcion = campos[4];
            curso.cuposDisponibles = stoi(campos[5]);
            curso.cuposTotal = stoi(campos[6]);
            curso.fechaInicio = campos[7];
            curso.fechaFin = campos[8];
            curso.activo = (campos[9] == "1");
            cursos.push_back(curso);
        }
    }

    cout << "✅ Cargados " << cursos.size() << " cursos\n";
    return cursos;
}




} // namespace Persistencia
} // namespace PNTD
