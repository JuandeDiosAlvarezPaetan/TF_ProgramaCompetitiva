#include "../estructuras/Curso.h"
#include "../estructuras/Usuario.h"
#include "../modulos/GestionCursos.h"
#include "../utilidades/PersistenciaCSV.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <ctime> 
#include <algorithm>  
namespace PNTD {
namespace Cursos {

// Placeholder implementations para que compile
GestorCursos gestorGlobal;

bool GestorCursos::inscribirUsuario(const string& dniUsuario, int idCurso) {
    // Buscar el curso por ID
    auto it = find_if(catalogoCursos.begin(), catalogoCursos.end(),
                      [idCurso](const Curso& c) { return c.id == idCurso && c.activo; });

    if (it == catalogoCursos.end()) {
        cout << "❌ Curso no encontrado o inactivo.\n";
        return false;
    }

    Curso& curso = *it;

    // Validar si ya está inscrito
if (verificarInscripcionActiva(dniUsuario, idCurso)) {
    cout << "⚠ Ya estás inscrito en este curso.\n";
    return false;
}

    // Validar disponibilidad
    if (curso.cuposDisponibles <= 0) {
        cout << "⚠ No hay cupos disponibles para este curso.\n";
        cout << "📥 Se agregará a la lista de espera.\n";
        Inscripcion espera(dniUsuario, idCurso, 5); // Prioridad baja
        espera.estado = "espera"; //
        colaInscripciones.push(espera);
        colaNotificaciones.push(Notificacion{dniUsuario, "Estás en la lista de espera del curso: " + curso.nombre, "espera"});
        return false;
    }
     
    // Crear inscripción con prioridad alta (puedes ajustar esto más adelante)
    Inscripcion nueva(dniUsuario, idCurso, 1);
    colaInscripciones.push(nueva);
    historialInscripciones.push_back(nueva);

    // Actualizar cupos
    curso.cuposDisponibles--;

    // Simular notificación
    string mensaje = "✅ Te has inscrito al curso: " + curso.nombre;
    enviarNotificacion(dniUsuario, mensaje, "confirmacion");

    // TODO: PersistenciaCSV::guardarInscripcion(nueva); (lo implementará el Integrante 4)
    
    cout << "✅ Inscripción realizada exitosamente.\n";
    return true;
}

vector<Curso> GestorCursos::buscarCursos(const CriteriosBusqueda& criterios) {
    vector<Curso> resultados;

    for (const auto& curso : catalogoCursos) {
        // Si solo queremos cursos activos y este no lo estÃ¡, lo descartamos
        if (criterios.soloActivos && !curso.activo) continue;

        // Coincidencia por categorÃ­a y modalidad (si se especifican)
        if ((criterios.categoria.empty() || curso.categoria == criterios.categoria) &&
            (criterios.modalidad.empty() || curso.modalidad == criterios.modalidad)) {
            resultados.push_back(curso);
        }
    }

    return resultados;
}

bool GestorCursos::verificarDisponibilidad(int idCurso) {
    auto it = find_if(catalogoCursos.begin(), catalogoCursos.end(),
                      [idCurso](const Curso& c) { return c.id == idCurso && c.activo; });

    if (it != catalogoCursos.end() && it->cuposDisponibles > 0) {
        return true;
    }

    return false;
}

void GestorCursos::procesarListaEspera(int idCurso) {
    queue<Inscripcion> temporal;

    while (!colaInscripciones.empty()) {
        Inscripcion actual = colaInscripciones.top(); colaInscripciones.pop();

        if (actual.idCurso == idCurso && verificarDisponibilidad(idCurso)) {
            // Inscribir automÃ¡ticamente
            actual.estado = "inscrito";
            historialInscripciones.push_back(actual);

            auto it = find_if(catalogoCursos.begin(), catalogoCursos.end(),
                              [idCurso](const Curso& c) { return c.id == idCurso; });

            if (it != catalogoCursos.end()) {
                it->cuposDisponibles--;
                string mensaje = "ðŸŽ‰ Cupo liberado: te inscribimos al curso " + it->nombre;
                enviarNotificacion(actual.dniUsuario, mensaje, "confirmacion");
            }
        } else {
            // Reencolar si no se puede inscribir todavÃ­a
            temporal.push(actual);
        }
    }

    // Restaurar la cola original con los que no fueron inscritos
    while (!temporal.empty()) {
        colaInscripciones.push(temporal.front());
        temporal.pop();
    }
}

void GestorCursos::enviarNotificacion(const string& dniUsuario, const string& mensaje, const string& tipo) {
    Notificacion notif(dniUsuario, mensaje, tipo);
    colaNotificaciones.push(notif);

    cout << "ðŸ”” Notificacion enviada a " << dniUsuario << ": " << mensaje << " [" << tipo << "]" << endl;
}

bool GestorCursos::marcarCursoCompletado(const string& dniUsuario, int idCurso) {
    for (auto& inscripcion : historialInscripciones) {
        if (inscripcion.dniUsuario == dniUsuario && inscripcion.idCurso == idCurso) {
            inscripcion.estado = "completado";
            enviarNotificacion(dniUsuario, "ðŸŽ“ Has completado el curso ID: " + to_string(idCurso), "completado");
            PNTD::Persistencia::CSV::guardarInscripciones(historialInscripciones);
            return true;
        }
    }
    cout << "âš ï¸ No se encontro una inscripciÃ³n activa para completar." << endl;
    return false;
}

vector<Inscripcion> GestorCursos::obtenerInscripciones(const string& dniUsuario) {
    vector<Inscripcion> resultado;

    for (const auto& insc : historialInscripciones) {
        string estado = insc.estado;
        transform(estado.begin(), estado.end(), estado.begin(), ::tolower);
        estado.erase(remove_if(estado.begin(), estado.end(), ::isspace), estado.end());

        if (insc.dniUsuario == dniUsuario && estado == "inscrito") {
            resultado.push_back(insc);
        }
    }
    return resultado;
}

bool GestorCursos::cargarCursos() {
    vector<Curso> cursosCargados = PNTD::Persistencia::CSV::cargarCursos();
    
    if (cursosCargados.empty()) {
        cout << "âš ï¸ No se encontraron cursos en el archivo CSV.\n";
        return false;
    }

    catalogoCursos = cursosCargados;
    cout << "âœ… Cursos cargados correctamente (" << catalogoCursos.size() << " encontrados).\n";
    return true;
}

bool GestorCursos::guardarCursos() {
    if (catalogoCursos.empty()) {
        cout << "âš ï¸ No hay cursos para guardar.\n";
        return false;
    }

    return PNTD::Persistencia::CSV::guardarCursos(catalogoCursos);
}

void GestorCursos::mostrarEstadisticas() {
    cout << "\nðŸ“Š ESTADISTICAS DEL MODULO CURSOS\n";

    cout << "ðŸ”¹ Total de cursos disponibles: " << catalogoCursos.size() << "\n";

    int cursosActivos = count_if(catalogoCursos.begin(), catalogoCursos.end(), [](const Curso& c) {
        return c.activo;
    });
    cout << "ðŸ”¹ Cursos activos: " << cursosActivos << "\n";

    int totalInscripciones = historialInscripciones.size();
    cout << "ðŸ”¹ Total de inscripciones realizadas: " << totalInscripciones << "\n";

    // Buscar el curso mÃ¡s popular
    map<int, int> contadorPorCurso;
    for (const auto& insc : historialInscripciones) {
        contadorPorCurso[insc.idCurso]++;
    }

    if (!contadorPorCurso.empty()) {
        auto maxIt = max_element(contadorPorCurso.begin(), contadorPorCurso.end(),
                                 [](const pair<int, int>& a, const pair<int, int>& b) {
                                     return a.second < b.second;
                                 });

        auto itCurso = find_if(catalogoCursos.begin(), catalogoCursos.end(), [&](const Curso& c) {
            return c.id == maxIt->first;
        });

        if (itCurso != catalogoCursos.end()) {
            cout << "ðŸ”¹ Curso mÃ¡s demandado: " << itCurso->nombre << " (" << maxIt->second << " inscripciones)\n";
        }
    }

    cout << "ðŸ“Œ Algoritmos utilizados:\n";
    cout << "â€¢ Priority Queue para inscripciones\n";
    cout << "â€¢ Filtro multidimensional por categoria y modalidad\n";
    cout << "â€¢ Estructura FIFO para notificaciones (en desarrollo)\n\n";
}

bool GestorCursos::verificarInscripcionActiva(const string& dniUsuario, int idCurso) {
    for (const auto& insc : historialInscripciones) {
        if (insc.dniUsuario == dniUsuario && insc.idCurso == idCurso) {
            return true;
        }
    }
    return false;
}

void GestorCursos::inicializar() {
    cout << "📦 Inicializando modulo de Cursos..." << endl;
    bool exitoCursos = cargarCursos();
    bool exitoInsc = cargarInscripciones(); 

    if (!exitoCursos || !exitoInsc) {
        cout << "⚠️ No se pudieron cargar todos los datos correctamente.\n";
    } else {
        cout << "✅ Modulo de Cursos listo para usarse.\n";
    }
}

// Funciones públicas del namespace
vector<Curso> buscarCursos(const CriteriosBusqueda& criterios) {
    return gestorGlobal.buscarCursos(criterios);
}
vector<Inscripcion> obtenerMisInscripciones(const string& dniUsuario) {
    return gestorGlobal.obtenerInscripciones(dniUsuario);
}

bool inscribirUsuario(const string& dni, int idCurso) {
    return gestorGlobal.inscribirUsuario(dni, idCurso);
}

bool verificarInscripcionActiva(const string& dni, int idCurso) {
    auto inscripciones = gestorGlobal.obtenerInscripciones(dni);
    for (const auto& ins : inscripciones) {
        if (ins.idCurso == idCurso && ins.estado == "inscrito") {
            return true;
        }
    }
    return false;
}

void inicializarModulo() {
    gestorGlobal.inicializar();
}

bool GestorCursos::cargarInscripciones() {
    vector<Inscripcion> insc = PNTD::Persistencia::CSV::cargarInscripciones();
    if (insc.empty()) {
        cout << "⚠️ No se encontraron inscripciones en el archivo CSV.\n";
        return false;
    }
    historialInscripciones = insc;
    cout << "✅ Inscripciones cargadas correctamente (" << insc.size() << " registros).\n";
    return true;
}

} // namespace Cursos
} // namespace PNTD