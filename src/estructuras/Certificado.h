#ifndef CERTIFICADO_H
#define CERTIFICADO_H

#include <string>
using namespace std;

struct Certificado {
    string hash;
    string dniUsuario;
    int idCurso;
    string nombreCurso;
    string nombreUsuario;
    string fechaEmision;
    string fechaExpiracion;
    bool valido;
    
    Certificado() : valido(true) {}
    
    Certificado(string h, string dni, int curso, string nCurso, string nUsuario) 
        : hash(h), dniUsuario(dni), idCurso(curso), nombreCurso(nCurso), 
          nombreUsuario(nUsuario), valido(true) {}
};

struct Notificacion {
    string dniUsuario;
    string mensaje;
    string tipo;
    string fecha;
    bool procesada;
    
    Notificacion(string dni, string msg, string t) 
        : dniUsuario(dni), mensaje(msg), tipo(t), procesada(false) {}
};

#endif