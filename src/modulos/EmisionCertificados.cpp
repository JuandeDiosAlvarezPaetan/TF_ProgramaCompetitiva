#include "../utilidades/ValidacionDatos.h"
#include "../utilidades/PersistenciaCSV.h"
#include "../utilidades/MerkleTree.hpp"
#include "modulos/GestorCertificados.h"
#include <iostream>
#include <vector>

namespace PNTD {
namespace Certificados {

GestorCertificados gestorGlobal;

// Funciones públicas del namespace
string generarCertificado(const string& dni, int idCurso) {
    return gestorGlobal.generarCertificado(dni, idCurso);
}

bool verificarCertificado(const string& hash) {
    return gestorGlobal.verificarCertificado(hash);
}

vector<Certificado> obtenerHistorial(const string& dni) {
    return gestorGlobal.obtenerHistorial(dni);
}

void inicializarModulo() {
    gestorGlobal.inicializar();
}

} // namespace Certificados
} // namespace PNTD