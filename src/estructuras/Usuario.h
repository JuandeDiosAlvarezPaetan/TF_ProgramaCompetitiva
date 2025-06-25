#ifndef USUARIO_H
#define USUARIO_H

#include <string>
using namespace std;

struct Usuario {
    string dni;
    string nombre;
    string apellidos;
    string email;
    string passwordHash;
    string telefono;
    string fechaRegistro;
    bool activo;
    
    Usuario() : activo(true) {}
    
    Usuario(string d, string n, string a, string e, string p, string t) 
        : dni(d), nombre(n), apellidos(a), email(e), passwordHash(p), telefono(t), activo(true) {}
};

#endif