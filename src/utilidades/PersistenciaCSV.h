#ifndef PERSISTENCIA_CSV_H
#define PERSISTENCIA_CSV_H

#include <vector>
#include <string>
#include "../estructuras/Usuario.h"
#include "../estructuras/Curso.h"
#include "../estructuras/Certificado.h"

using namespace std;

namespace PNTD {
namespace Persistencia {

class CSV {
public:
    // Operaciones con Usuarios
    static bool guardarUsuarios(const vector<Usuario>& usuarios);
    static vector<Usuario> cargarUsuarios();
    
    // Operaciones con Cursos
    static bool guardarCursos(const vector<Curso>& cursos);
    static vector<Curso> cargarCursos();
    
    // Operaciones con Certificados
    static bool guardarCertificados(const vector<Certificado>& certificados);
    static vector<Certificado> cargarCertificados();
    
    // Operaciones con Inscripciones
    static bool guardarInscripciones(const vector<Inscripcion>& inscripciones);
    static vector<Inscripcion> cargarInscripciones();
    
    // Utilidades
    static vector<string> parsearLinea(const string& linea);
    static string escaparCSV(const string& campo);
    static bool crearDirectorios();
    static bool verificarArchivos();
};

} // namespace Persistencia
} // namespace PNTD

#endif