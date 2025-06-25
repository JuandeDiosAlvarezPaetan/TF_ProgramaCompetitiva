#ifndef EMISION_CERTIFICADOS_H
#define EMISION_CERTIFICADOS_H

#include <list>
#include <unordered_map>
#include <vector>
#include <string>
#include "../estructuras/Certificado.h"

using namespace std;

namespace PNTD {
namespace Certificados {

class GestorCertificados {
private:
    std::list<Certificado> historial; // orden cronológico
    std::unordered_map<std::string, Certificado*> tabla; // hash -> ptr
    std::string merkleRoot;
public:
    string generarCertificado(const string& dniUsuario, int idCurso);
    bool verificarCertificado(const string& hashCertificado);
    vector<Certificado> obtenerHistorial(const string& dniUsuario);
    bool exportarCertificado(const string& hashCertificado, const string& formato);
    string generarHashUnico(const string& dniUsuario, int idCurso, const string& timestamp);
    bool cargarCertificados();
    bool guardarCertificados();
    void mostrarEstadisticas();
    void inicializar();
};

// Funciones públicas del namespace
extern string generarCertificado(const string& dni, int idCurso);
extern bool verificarCertificado(const string& hash);
extern vector<Certificado> obtenerHistorial(const string& dni);
extern void inicializarModulo();

} // namespace Certificados
} // namespace PNTD

#endif