#ifndef RUTASREPORTES_H
#define RUTASREPORTES_H

#include <cstdlib>
#include <direct.h>
#include <string>

namespace rutasReportes {

inline std::string directorio()
{
#ifdef REPORTES_DIR
    std::string ruta(REPORTES_DIR);
#else
    std::string ruta("reportes");
#endif
    _mkdir(ruta.c_str());
    return ruta;
}

inline int convertirAPng(const std::string& dot, const std::string& png)
{
    const std::string comando = "dot -Tpng \"" + dot + "\" -o \"" + png + "\"";
    return std::system(comando.c_str());
}

}

#endif // RUTASREPORTES_H
