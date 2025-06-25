#ifndef CURSO_H
#define CURSO_H

#include <string>
using namespace std;

struct Curso {
    int id;
    string nombre;
    string categoria;
    string modalidad;
    string descripcion;
    int cuposDisponibles;
    int cuposTotal;
    string fechaInicio;
    string fechaFin;
    bool activo;

    Curso() : id(0), cuposDisponibles(0), cuposTotal(0), activo(true) {}

    Curso(int i, string n, string c, string m, int cupos)
        : id(i), nombre(n), categoria(c), modalidad(m), descripcion(""),
          cuposDisponibles(cupos), cuposTotal(cupos), fechaInicio(""), fechaFin(""), activo(true) {}
};

struct Inscripcion {
    string dniUsuario;
    int idCurso;
    string fechaInscripcion;
    string estado;
    int prioridad;

    Inscripcion(string dni, int curso, int prio = 1)
        : dniUsuario(dni), idCurso(curso), fechaInscripcion(""), estado("inscrito"), prioridad(prio) {}

    bool operator<(const Inscripcion& other) const {
        return prioridad > other.prioridad;
    }
};

#endif