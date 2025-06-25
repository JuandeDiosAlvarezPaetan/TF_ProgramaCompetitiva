#include "GestorCertificados.h"
#include "../utilidades/PersistenciaCSV.h"
#include "../utilidades/ValidacionDatos.h"
#include "../utilidades/MerkleTree.hpp"
#include <functional>   // std::hash
#include <iostream>

/* ==== Helpers locales ================================================= */
namespace {
    std::string fechaISO() { return "2025-06-12T00:00:00Z"; }               // TODO: reemplazar
}

/* ==== TODAS las funciones quedan realmente en el namespace correcto === */
namespace PNTD::Certificados {

GestorCertificados::GestorCertificados() {
    cargarDesdeCSV("");  // ✅ carga automática desde certificados.csv
}

/* ---------- PERSISTENCIA ---------- */
void GestorCertificados::cargarDesdeCSV(const std::string&) {
    auto vec = PNTD::Persistencia::CSV::cargarCertificados();
    historial.assign(vec.begin(), vec.end());
    tabla.clear();
    for (auto& c : historial) tabla[c.hash] = &c;
    reconstruirMerkleRoot();
}
void GestorCertificados::guardarEnCSV(const std::string&) const {
    std::vector<Certificado> tmp(historial.begin(), historial.end());
    PNTD::Persistencia::CSV::guardarCertificados(tmp);
}

/* ---------- GENERAR ---------- */
std::string GestorCertificados::generarCertificado(const std::string& dni,int idCurso){
    if (dni.empty() || idCurso<=0) return "";
    std::string fecha = fechaISO();
    std::string seed  = dni + "|" + std::to_string(idCurso) + "|" + fecha;
    std::string hash  = PNTD::Utils::Validador::encriptarSHA256(seed);


    Certificado c;
    c.hash            = hash;
    c.dniUsuario      = dni;
    c.idCurso         = idCurso;
    c.nombreCurso     = "Curso " + std::to_string(idCurso);
    c.nombreUsuario   = dni;
    c.fechaEmision    = fecha;
    c.fechaExpiracion = "";
    c.valido          = true;

    historial.push_back(c);
    historial.sort([](const Certificado& a, const Certificado& b) {
    return a.fechaEmision < b.fechaEmision;
    });
    tabla[hash] = &historial.back();
    guardarEnCSV();
    reconstruirMerkleRoot();
    return hash;
}

/* ---------- VERIFICAR ---------- */
bool GestorCertificados::verificarCertificado(const std::string& hash) const {
    auto it = tabla.find(hash);
    if (it==tabla.end() || !it->second->valido) return false;

    const Certificado* c = it->second;
    std::string seed = c->dniUsuario + "|" + std::to_string(c->idCurso) + "|" + c->fechaEmision;
    return PNTD::Utils::Validador::encriptarSHA256(seed) == hash;
}

/* ---------- LISTAR ---------- */
std::list<Certificado>
GestorCertificados::listarPorUsuario(const std::string& dni) const {
    std::list<Certificado> out;
    for (const auto& c: historial)
        if (c.dniUsuario==dni && c.valido) out.push_back(c);
    return out;
}

/* ---------- MERKLE (stub) ---------- */
void GestorCertificados::reconstruirMerkleRoot() {
    std::vector<std::string> hojas;
    for (const auto& c : historial) {
        if (c.valido) {
            hojas.push_back(c.hash);
        }
    }

    MerkleTree arbol(hojas);
    merkleRoot = arbol.getRoot();
}

std::string GestorCertificados::getMerkleRoot() const {
    return merkleRoot;
}

// Implementación faltante de obtenerHistorial
std::vector<Certificado> GestorCertificados::obtenerHistorial(const std::string& dni) {
    std::vector<Certificado> resultado;
    for (const auto& c : historial) {
        if (c.dniUsuario == dni && c.valido) {
            resultado.push_back(c);
        }
    }
    return resultado;
}

// Implementación faltante de inicializar
void GestorCertificados::inicializar() {
    std::cout << "✅ Módulo Certificados inicializado correctamente.\n";
}

} // namespace PNTD::Certificados
