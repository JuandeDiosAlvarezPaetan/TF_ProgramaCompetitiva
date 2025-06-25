#ifndef PNTD_GESTOR_CERTIFICADOS_H
#define PNTD_GESTOR_CERTIFICADOS_H

#include <string>
#include <list>
#include <unordered_map>
#include <vector>
#include "../estructuras/Certificado.h"   // asegúrate de que existe

namespace PNTD::Certificados {

/*-----------------------------------------------------------
 | GestorCertificados
 |  - Carga / guarda certificados en CSV
 |  - Emite (genera) un nuevo certificado
 |  - Verifica su integridad
 |  - Lista todos los certificados de un usuario
 *----------------------------------------------------------*/
class GestorCertificados {
public:
    GestorCertificados();
    /* ==== Persistencia ==================================== */
    void cargarDesdeCSV (const std::string& ruta = "data/certificados.csv");
    void guardarEnCSV   (const std::string& ruta = "data/certificados.csv") const;

    /* ==== Operaciones de alto nivel ======================= */
    std::string generarCertificado(const std::string& dni, int idCurso);
    bool verificarCertificado(const std::string& hash) const;
    std::list<Certificado> listarPorUsuario(const std::string& dni) const;
    std::vector<Certificado> obtenerHistorial(const std::string& dni);
    bool exportarCertificado(const std::string& hashCertificado, const std::string& formato);
    std::string generarHashUnico(const std::string& dniUsuario, int idCurso, const std::string& timestamp);
    bool cargarCertificados();
    bool guardarCertificados();
    void mostrarEstadisticas();
    void inicializar();
    std::string getMerkleRoot() const;
private:
    /* ==== Helpers internos ================================= */
    void reconstruirMerkleRoot();   // (implementación opcional)

    /* ==== Contenedores ===================================== */
    std::list<Certificado>                       historial; // orden cronológico
    std::unordered_map<std::string, Certificado*> tabla;     // hash -> ptr
    std::string                                   merkleRoot;
};

} // namespace PNTD::Certificados
#endif  // PNTD_GESTOR_CERTIFICADOS_H
