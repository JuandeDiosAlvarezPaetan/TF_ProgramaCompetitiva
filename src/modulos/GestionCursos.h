#ifndef GESTION_CURSOS_H
#define GESTION_CURSOS_H

#include <vector>
#include <queue>
#include <string>
#include "../estructuras/Certificado.h"
#include "../estructuras/Curso.h"
#include "../estructuras/Usuario.h"

using namespace std;

namespace PNTD {
namespace Cursos {

struct CriteriosBusqueda {
    string categoria;
    string modalidad;
    string fechaInicio;
    bool soloActivos;
    
    CriteriosBusqueda() : soloActivos(true) {}
};

class GestorCursos {
private:
    vector<Curso> catalogoCursos;
    priority_queue<Inscripcion> colaInscripciones;
    queue<Notificacion> colaNotificaciones;
    vector<Inscripcion> historialInscripciones;
    
public:
    bool inscribirUsuario(const string& dniUsuario, int idCurso);
    vector<Curso> buscarCursos(const CriteriosBusqueda& criterios);
    bool verificarDisponibilidad(int idCurso);
     bool cargarInscripciones();
    void procesarListaEspera(int idCurso);
    void enviarNotificacion(const string& dniUsuario, const string& mensaje, const string& tipo);
    bool marcarCursoCompletado(const string& dniUsuario, int idCurso);
    vector<Inscripcion> obtenerInscripciones(const string& dniUsuario);
    bool cargarCursos();
    bool guardarCursos();
    void mostrarEstadisticas();
    bool verificarInscripcionActiva(const string& dniUsuario, int idCurso);
    void inicializar();
};

// Funciones públicas del namespace
extern vector<Curso> buscarCursos(const CriteriosBusqueda& criterios);
extern bool inscribirUsuario(const string& dni, int idCurso);
extern bool verificarInscripcionActiva(const string& dni, int idCurso);
extern void inicializarModulo();
extern vector<Inscripcion> obtenerMisInscripciones(const string& dniUsuario);

} // namespace Cursos
} // namespace PNTD

#endif
